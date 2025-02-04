/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_adxl345_fifo.c
 * @brief     driver adxl345 fifo source file
 * @version   2.0.0
 * @author    Shifeng Li
 * @date      2021-04-20
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/04/20  <td>2.0      <td>Shifeng Li  <td>format the code
 * <tr><td>2020/12/23  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_adxl345_fifo.h"

static adxl345_handle_t gs_handle;                                   /**< adxl345 handle */
static void (*a_callback)(float (*g)[3], uint16_t len) = NULL;       /**< irq callback */
static int16_t gs_raw[32][3];                                        /**< raw data buffer */
static float gs_data[32][3];                                         /**< data buffer */

/**
 * @brief  fifo irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t adxl345_fifo_irq_handler(void)
{
    if (adxl345_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     fifo receive callback
 * @param[in] type irq type
 * @note      none
 */
static void a_adxl345_fifo_receive_callback(uint8_t type)
{
    switch (type)
    {
        case ADXL345_INTERRUPT_WATERMARK :
        {
            uint16_t len;
            
            len = 32;
            if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw, (float (*)[3])gs_data, (uint16_t *)&len) != 0)
            {
                adxl345_interface_debug_print("adxl345: read failed.\n");
                
                return;
            }
            if (a_callback != NULL)
            {
                a_callback(gs_data, len);
            }
            
            break;
        }
        default :
        {
            break;
        }
    }
}

/**
 * @brief     fifo example init
 * @param[in] interface chip interface
 * @param[in] addr_pin iic device address
 * @param[in] *callback pointer to an irq callback address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t adxl345_fifo_init(adxl345_interface_t interface, adxl345_address_t addr_pin,
                          void (*callback)(float (*g)[3], uint16_t len))
{
    uint8_t res;
    uint8_t source;
    uint8_t status;
    int8_t reg;
    
    /* link interface function */
    DRIVER_ADXL345_LINK_INIT(&gs_handle, adxl345_handle_t);
    DRIVER_ADXL345_LINK_IIC_INIT(&gs_handle, adxl345_interface_iic_init);
    DRIVER_ADXL345_LINK_IIC_DEINIT(&gs_handle, adxl345_interface_iic_deinit);
    DRIVER_ADXL345_LINK_IIC_READ(&gs_handle, adxl345_interface_iic_read);
    DRIVER_ADXL345_LINK_IIC_WRITE(&gs_handle, adxl345_interface_iic_write);
    DRIVER_ADXL345_LINK_SPI_INIT(&gs_handle, adxl345_interface_spi_init);
    DRIVER_ADXL345_LINK_SPI_DEINIT(&gs_handle, adxl345_interface_spi_deinit);
    DRIVER_ADXL345_LINK_SPI_READ(&gs_handle, adxl345_interface_spi_read);
    DRIVER_ADXL345_LINK_SPI_WRITE(&gs_handle, adxl345_interface_spi_write);
    DRIVER_ADXL345_LINK_DELAY_MS(&gs_handle, adxl345_interface_delay_ms);
    DRIVER_ADXL345_LINK_DEBUG_PRINT(&gs_handle, adxl345_interface_debug_print);
    DRIVER_ADXL345_LINK_RECEIVE_CALLBACK(&gs_handle, a_adxl345_fifo_receive_callback);
    
    /* set the interface */
    res = adxl345_set_interface(&gs_handle, interface);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interface failed.\n");
       
        return 1;
    }
    
    /* set addr pin */
    res = adxl345_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set addr pin failed.\n");
       
        return 1;
    }
    
    /* adxl345 initialization */
    res = adxl345_init(&gs_handle);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: init failed.\n");
       
        return 1;
    }
    
    /* set default rate */
    res = adxl345_set_rate(&gs_handle, ADXL345_FIFO_DEFAULT_RATE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default spi wire */
    res = adxl345_set_spi_wire(&gs_handle, ADXL345_FIFO_DEFAULT_SPI_WIRE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set spi wire failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt pin */
    res = adxl345_set_interrupt_active_level(&gs_handle, ADXL345_FIFO_DEFAULT_INTERRUPT_ACTIVE_LEVEL);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set adxl345 set interrupt active level failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set full resolution */
    res = adxl345_set_full_resolution(&gs_handle, ADXL345_FIFO_DEFAULT_FULL_RESOLUTION);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set full resolution failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set auto sleep */
    res = adxl345_set_auto_sleep(&gs_handle, ADXL345_FIFO_DEFAULT_AUTO_SLEEP);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set auto sleep failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set sleep */
    res = adxl345_set_sleep(&gs_handle, ADXL345_FIFO_DEFAULT_SLEEP);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set sleep failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set sleep frequency */
    res = adxl345_set_sleep_frequency(&gs_handle, ADXL345_FIFO_DEFAULT_SLEEP_FREQUENCY);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set sleep frequency failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set justify */
    res = adxl345_set_justify(&gs_handle, ADXL345_FIFO_DEFAULT_JUSTIFY);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set justify failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set range */
    res = adxl345_set_range(&gs_handle, ADXL345_FIFO_DEFAULT_RANGE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set range failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set mode */
    res = adxl345_set_mode(&gs_handle, ADXL345_MODE_FIFO);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set mode failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set trigger pin */
    res = adxl345_set_trigger_pin(&gs_handle, ADXL345_FIFO_DEFAULT_TRIGGER_PIN);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set trigger pin failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set watermark level */
    res = adxl345_set_watermark(&gs_handle, ADXL345_FIFO_DEFAULT_WATERMARK);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set watermark failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default offset */
    res = adxl345_offset_convert_to_register(&gs_handle, ADXL345_FIFO_DEFAULT_OFFSET, (int8_t *)&reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: offset convert to register failed.\n");
        (void)adxl345_deinit(&gs_handle); 
        
        return 1;
    }
    res = adxl345_set_offset(&gs_handle, reg, reg, reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set offset failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default tap threshold */
    res = adxl345_tap_threshold_convert_to_register(&gs_handle, ADXL345_FIFO_DEFAULT_TAP_THRESHOLD, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: tap threshold convert to register failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_set_tap_threshold(&gs_handle, reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set tap threshold failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default duration */
    res = adxl345_duration_convert_to_register(&gs_handle, ADXL345_FIFO_DEFAULT_DURATION, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: duration convert to register failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_set_duration(&gs_handle, reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set duration failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set latent time */
    res = adxl345_latent_convert_to_register(&gs_handle, ADXL345_FIFO_DEFAULT_LATENT, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: latent convert to register failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_set_latent(&gs_handle, reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set latent failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set window time */
    res = adxl345_window_convert_to_register(&gs_handle, ADXL345_FIFO_DEFAULT_WINDOW, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: window convert to register failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_set_window(&gs_handle, reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set window failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable tap */
    res = adxl345_set_tap_axis(&gs_handle, ADXL345_TAP_AXIS_X, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set tap axis failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_set_tap_axis(&gs_handle, ADXL345_TAP_AXIS_Y, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set tap axis failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_set_tap_axis(&gs_handle, ADXL345_TAP_AXIS_Z, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set tap axis failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default suppress */
    res = adxl345_set_tap_suppress(&gs_handle, ADXL345_FIFO_DEFAULT_TAP_SUPPRESS);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set tap suppress failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default single tap map */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_SINGLE_TAP, ADXL345_FIFO_DEFAULT_INTERRUPT_SINGLE_TAP_MAP);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable single tap */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_SINGLE_TAP, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default double tap map */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_DOUBLE_TAP, ADXL345_FIFO_DEFAULT_INTERRUPT_DOUBLE_TAP_MAP);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable double tap */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_DOUBLE_TAP, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default linking activity and inactivity */
    res = adxl345_set_link_activity_inactivity(&gs_handle, ADXL345_FIFO_DEFAULT_LINK_ACTIVITY_INACTIVITY);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set link activity inactivity failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default action threshold */
    res = adxl345_action_threshold_convert_to_register(&gs_handle, ADXL345_FIFO_DEFAULT_ACTION_THRESHOLD, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: action threshold convert to register failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_set_action_threshold(&gs_handle, reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action threshold failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default inaction threshold */
    res = adxl345_inaction_threshold_convert_to_register(&gs_handle, ADXL345_FIFO_DEFAULT_INACTION_THRESHOLD, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: inaction threshold convert to register failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_set_inaction_threshold(&gs_handle, reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set inaction threshold failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default inaction time */
    res = adxl345_inaction_time_convert_to_register(&gs_handle, ADXL345_FIFO_DEFAULT_INACTION_TIME, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: inaction time convert to register failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_set_inaction_time(&gs_handle, reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set inaction time failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable action */
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_ACTION_X, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_ACTION_Y, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_ACTION_Z, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_INACTION_X, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_INACTION_Y, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_INACTION_Z, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default action coupled */
    res = adxl345_set_action_coupled(&gs_handle, ADXL345_FIFO_DEFAULT_ACTION_COUPLED);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action coupled failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default inaction coupled */
    res = adxl345_set_inaction_coupled(&gs_handle, ADXL345_FIFO_DEFAULT_INACTION_COUPLED);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set inaction coupled failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default activity map */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_ACTIVITY, ADXL345_FIFO_DEFAULT_INTERRUPT_ACTIVITY_MAP);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable activity interrupt */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_ACTIVITY, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default inactivity map */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_INACTIVITY, ADXL345_FIFO_DEFAULT_INTERRUPT_INACTIVITY_MAP);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable inactivity interrupt */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_INACTIVITY, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set free fall threshold */
    res = adxl345_free_fall_threshold_convert_to_register(&gs_handle, ADXL345_FIFO_DEFAULT_FREE_FALL_THRESHOLD, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: free fall threshold convert to register failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_set_free_fall_threshold(&gs_handle, reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set free fall threshold failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set free fall time */
    res = adxl345_free_fall_time_convert_to_register(&gs_handle, ADXL345_FIFO_DEFAULT_FREE_FALL_TIME, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: free fall time convert to register failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_set_free_fall_time(&gs_handle, reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set free fall time failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default free fall map */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_FREE_FALL, ADXL345_FIFO_DEFAULT_INTERRUPT_FREE_FALL_MAP);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable free fall interrupt */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_FREE_FALL, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default data ready map */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_DATA_READY, ADXL345_FIFO_DEFAULT_INTERRUPT_DATA_READY_MAP);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_DATA_READY, ADXL345_FIFO_DEFAULT_INTERRUPT_DATA_READY);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default watermark map */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_WATERMARK, ADXL345_FIFO_DEFAULT_INTERRUPT_WATERMARK_MAP);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_WATERMARK, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default overrun map */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_OVERRUN, ADXL345_FIFO_DEFAULT_INTERRUPT_OVERRUN_MAP);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_OVERRUN, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* clear interrupt */
    res = adxl345_get_interrupt_source(&gs_handle, &source);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt source failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* get fifo status */
    res = adxl345_get_watermark_level(&gs_handle, &status);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get watermark level failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    a_callback = callback;
    
    return 0;
}

/**
 * @brief  fifo example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t adxl345_fifo_deinit(void)
{
    if (adxl345_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

