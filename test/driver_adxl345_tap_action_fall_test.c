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
 * @file      driver_adxl345_tap_action_fall_test.c
 * @brief     driver adxl345 tap action fall test source file
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

#include "driver_adxl345_tap_action_fall_test.h"

static adxl345_handle_t gs_handle;             /**< adxl345 handle */
static uint8_t gs_tap_action_fall_flag;        /**< tap action fall flag */

/**
 * @brief  action test irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t adxl345_action_test_irq_handler(void)
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
 * @brief     interface test receive callback
 * @param[in] type is the irq type
 * @note      none
 */
static void a_adxl345_interface_test_receive_callback(uint8_t type)
{
    switch (type)
    {
        case ADXL345_INTERRUPT_DATA_READY :
        {
            break;
        }
        case ADXL345_INTERRUPT_SINGLE_TAP :
        {
            gs_tap_action_fall_flag |= 1 << 0;
            adxl345_interface_debug_print("adxl345: irq single tap.\n");
            
            break;
        }
        case ADXL345_INTERRUPT_DOUBLE_TAP :
        {
            gs_tap_action_fall_flag |= 1 << 1;
            adxl345_interface_debug_print("adxl345: irq double tap.\n");
            
            break;
        }
        case ADXL345_INTERRUPT_ACTIVITY :
        {
            gs_tap_action_fall_flag |= 1 << 2;
            adxl345_interface_debug_print("adxl345: irq activity.\n");
            
            break;
        }
        case ADXL345_INTERRUPT_INACTIVITY :
        {
            gs_tap_action_fall_flag |= 1 << 3;
            adxl345_interface_debug_print("adxl345: irq inactivity.\n");
            
            break;
        }
        case ADXL345_INTERRUPT_FREE_FALL :
        {
            gs_tap_action_fall_flag |= 1 << 4;
            adxl345_interface_debug_print("adxl345: irq free fall.\n");
            
            break;
        }
        case ADXL345_INTERRUPT_OVERRUN :
        {
            break;
        }
        default :
        {
            break;
        }
    }
}

/**
 * @brief  test server
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
static uint8_t a_adxl345_test_server(void)
{
    int16_t raw[3];
    float g_out[3];
    uint16_t len;
    
    len = 1;
    
    /* read data */
    if (adxl345_read(&gs_handle, (int16_t (*)[3])&raw, (float (*)[3])&g_out, (uint16_t *)&len) != 0)
    {
        adxl345_interface_debug_print("adxl345: read failed.\n");
       
        return 1;
    }

    return 0;
}

/**
 * @brief     tap action fall test
 * @param[in] interface is the chip interface
 * @param[in] addr_pin is the iic device address
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t adxl345_tap_action_fall_test(adxl345_interface_t interface, adxl345_address_t addr_pin)
{
    uint8_t res;
    int8_t reg;
    adxl345_info_t info;
    
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
    DRIVER_ADXL345_LINK_RECEIVE_CALLBACK(&gs_handle, a_adxl345_interface_test_receive_callback);
    
    /* get information */
    res = adxl345_info(&info);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        adxl345_interface_debug_print("adxl345: chip is %s.\n", info.chip_name);
        adxl345_interface_debug_print("adxl345: manufacturer is %s.\n", info.manufacturer_name);
        adxl345_interface_debug_print("adxl345: interface is %s.\n", info.interface);
        adxl345_interface_debug_print("adxl345: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        adxl345_interface_debug_print("adxl345: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        adxl345_interface_debug_print("adxl345: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        adxl345_interface_debug_print("adxl345: max current is %0.2fmA.\n", info.max_current_ma);
        adxl345_interface_debug_print("adxl345: max temperature is %0.1fC.\n", info.temperature_max);
        adxl345_interface_debug_print("adxl345: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* set interface */
    res = adxl345_set_interface(&gs_handle, interface);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interface failed.\n");
       
        return 1;
    }
    
    /* set address pin */
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
    
    /* set 12.5 rate */
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_12P5);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set 4 wire */
    res = adxl345_set_spi_wire(&gs_handle, ADXL345_SPI_WIRE_4);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set spi wire failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt low */
    res = adxl345_set_interrupt_active_level(&gs_handle, ADXL345_INTERRUPT_ACTIVE_LEVEL_LOW);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt active level failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set full resolution */
    res = adxl345_set_full_resolution(&gs_handle, ADXL345_BOOL_TRUE);
    if (res != 0) 
    {
        adxl345_interface_debug_print("adxl345: set full resolution failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable auto sleep */
    res = adxl345_set_auto_sleep(&gs_handle, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set auto sleep failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable sleep */
    res = adxl345_set_sleep(&gs_handle, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set sleep failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set sleep 1Hz */
    res = adxl345_set_sleep_frequency(&gs_handle, ADXL345_SLEEP_FREQUENCY_1HZ);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set sleep frequency failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set justify right */
    res = adxl345_set_justify(&gs_handle, ADXL345_JUSTIFY_RIGHT);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set justify failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set range 16g */
    res = adxl345_set_range(&gs_handle, ADXL345_RANGE_16G);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set range failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set bypass mode */
    res = adxl345_set_mode(&gs_handle, ADXL345_MODE_BYPASS);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set mode failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set trigger pin int 2 */
    res = adxl345_set_trigger_pin(&gs_handle, ADXL345_INTERRUPT_PIN2);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set trigger pin failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    } 
    
    /* set watermark 16 level */
    res = adxl345_set_watermark(&gs_handle, 16);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set watermark failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set offset */
    res = adxl345_offset_convert_to_register(&gs_handle, 0.0f, (int8_t *)&reg);
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
    
    /* set tap */
    res = adxl345_tap_threshold_convert_to_register(&gs_handle, 3.0f, (uint8_t *)&reg);
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
    
    /* set duration time */
    res = adxl345_duration_convert_to_register(&gs_handle, 10*1000, (uint8_t *)&reg);
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
    res = adxl345_latent_convert_to_register(&gs_handle, 20.0f, (uint8_t *)&reg);
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
    
    /* set window */
    res = adxl345_window_convert_to_register(&gs_handle, 80.0f, (uint8_t *)&reg);
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
    
    /* set tap x */
    res = adxl345_set_tap_axis(&gs_handle, ADXL345_TAP_AXIS_X, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set tap axis failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_set_tap_axis(&gs_handle, ADXL345_TAP_AXIS_Y, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set tap axis failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_set_tap_axis(&gs_handle, ADXL345_TAP_AXIS_Z, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set tap axis failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable suppress */
    res = adxl345_set_tap_suppress(&gs_handle, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set tap suppress failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* map int 1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_SINGLE_TAP, ADXL345_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set single tap */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_SINGLE_TAP, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* map int 1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_DOUBLE_TAP, ADXL345_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set double tap */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_DOUBLE_TAP, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* link activity and inactivity */
    res = adxl345_set_link_activity_inactivity(&gs_handle, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set link activity inactivity failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set action threshold */
    res = adxl345_action_threshold_convert_to_register(&gs_handle, 2.0f, (uint8_t *)&reg);
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
    
    /* set inaction threshold */
    res = adxl345_inaction_threshold_convert_to_register(&gs_handle, 1.0f, (uint8_t *)&reg);
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
    
    /* set inaction time */
    res = adxl345_inaction_time_convert_to_register(&gs_handle, 3, (uint8_t *)&reg);
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
    
    /* set action x */
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_ACTION_X, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set action y */
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_ACTION_Y, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set action z */
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_ACTION_Z, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set inaction x */
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_INACTION_X, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set inaction y */
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_INACTION_Y, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set inaction z */
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_INACTION_Z, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set action ac coupled */
    res = adxl345_set_action_coupled(&gs_handle, ADXL345_COUPLED_AC);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action coupled failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set inaction dc coupled */
    res = adxl345_set_inaction_coupled(&gs_handle, ADXL345_COUPLED_DC);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set inaction coupled failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* map int 1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_ACTIVITY, ADXL345_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set activity */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_ACTIVITY, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* map int 1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_INACTIVITY, ADXL345_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set inactivity */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_INACTIVITY, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set free fall */
    res = adxl345_free_fall_threshold_convert_to_register(&gs_handle, 0.8f, (uint8_t *)&reg);
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
    res = adxl345_free_fall_time_convert_to_register(&gs_handle, 10, (uint8_t *)&reg);
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
    
    /* set interrupt 1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_FREE_FALL, ADXL345_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set free fall */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_FREE_FALL, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt 1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_DATA_READY, ADXL345_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set data ready */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_DATA_READY, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt 1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_WATERMARK, ADXL345_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set data ready */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_WATERMARK, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt 1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_OVERRUN, ADXL345_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set overrun */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_OVERRUN, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
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
    adxl345_interface_delay_ms(10);
    
    /* start tap action fall test */
    adxl345_interface_debug_print("adxl345: start tap action fall test.\n"); 
    
    /* single tap test */
    adxl345_interface_debug_print("adxl345: please single tap.\n");
    gs_tap_action_fall_flag = 0;
    while (1)
    {
        if ((gs_tap_action_fall_flag & (1 << 0)) == (1 << 0))
        {
            adxl345_interface_debug_print("adxl345: detect single tap.\n");
               
            break;
        }
        if (a_adxl345_test_server() != 0)
        {
            return 1;
        }
        adxl345_interface_delay_ms(500);
    }
    
    /* double tap test */
    adxl345_interface_debug_print("adxl345: please double tap.\n");
    gs_tap_action_fall_flag = 0;
    while (1)
    {
        if ((gs_tap_action_fall_flag & (1 << 1)) == (1 << 1))
        {
            adxl345_interface_debug_print("adxl345: detect double tap.\n");
           
            break;
        }
        if (a_adxl345_test_server() != 0)
        {
            return 1;
        }
        adxl345_interface_delay_ms(500);
    }
    
    /* action */
    adxl345_interface_debug_print("adxl345: please action.\n");
    gs_tap_action_fall_flag = 0;
    while (1)
    {
        if ((gs_tap_action_fall_flag & (1 << 2)) == (1 << 2))
        {
            adxl345_interface_debug_print("adxl345: detect action.\n");
           
            break;
        }
        if (a_adxl345_test_server() != 0)
        {
            return 1;
        }
        adxl345_interface_delay_ms(500);
    }
    
    /* inaction */
    adxl345_interface_debug_print("adxl345: please stop action.\n");
    gs_tap_action_fall_flag = 0;
    while (1)
    {
        if ((gs_tap_action_fall_flag & (1 << 3)) == (1 << 3))
        {
            adxl345_interface_debug_print("adxl345: detect stop.\n");
           
            break;
        }
        if (a_adxl345_test_server() != 0)
        {
            return 1;
        }
        adxl345_interface_delay_ms(500);
    }
    
    /* free fall */
    adxl345_interface_debug_print("adxl345: please free fall.\n");
    gs_tap_action_fall_flag = 0;
    while (1)
    {
        if ((gs_tap_action_fall_flag & (1 << 4)) == (1 << 4))
        {
            adxl345_interface_debug_print("adxl345: detect free fall.\n");
           
            break;
        }
        if (a_adxl345_test_server() != 0)
        {
            return 1;
        }
        adxl345_interface_delay_ms(500);
    }
    adxl345_interface_debug_print("adxl345: finish tap action fall test.\n");
    (void)adxl345_deinit(&gs_handle);
    
    return 0;
}
