/**
 * Copyright (C) LibDriver 2015-2021 All rights reserved
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
 * @file      driver_adxl345_read_test.c
 * @brief     driver adxl345 read test source file
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

#include "driver_adxl345_read_test.h"

static adxl345_handle_t gs_handle;        /**< adxl345 handle */
static int16_t gs_raw_test[20][3];        /**< raw test buffer */
static float gs_test[20][3];              /**< test buffer */

/**
 * @brief     read test
 * @param[in] interface is the chip interface
 * @param[in] addr_pin is the iic device address
 * @param[in] times is the test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t adxl345_read_test(adxl345_interface_t interface, adxl345_address_t addr_pin, uint32_t times)
{
    volatile uint8_t res, i;
    volatile int8_t reg;
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
    DRIVER_ADXL345_LINK_RECEIVE_CALLBACK(&gs_handle, adxl345_interface_receive_callback);
    
    /* get information */
    res = adxl345_info(&info);
    if (res)
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
        adxl345_interface_debug_print("adxl345: driver version is %d.%d.\n", info.driver_version/1000, (info.driver_version%1000)/100);
        adxl345_interface_debug_print("adxl345: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        adxl345_interface_debug_print("adxl345: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        adxl345_interface_debug_print("adxl345: max current is %0.2fmA.\n", info.max_current_ma);
        adxl345_interface_debug_print("adxl345: max temperature is %0.1fC.\n", info.temperature_max);
        adxl345_interface_debug_print("adxl345: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* set the interface */
    res = adxl345_set_interface(&gs_handle, interface);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set interface failed.\n");
       
        return 1;
    }
    
    /* set the address pin */
    res = adxl345_set_addr_pin(&gs_handle, addr_pin);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set addr pin failed.\n");
       
        return 1;
    }
    
    /* init adxl345 */
    res = adxl345_init(&gs_handle);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: init failed.\n");
       
        return 1;
    }
    
    /* set the spi 4 type */
    res = adxl345_set_spi_wire(&gs_handle, ADXL345_SPI_WIRE_4);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set spi wire failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt low */
    res = adxl345_set_interrupt_active_level(&gs_handle, ADXL345_INTERRUPT_ACTIVE_LEVEL_LOW);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set interrupt active level failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable auto sleep */
    res = adxl345_set_auto_sleep(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set auto sleep failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable sleep */
    res = adxl345_set_sleep(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set sleep failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set sleep 1Hz */
    res = adxl345_set_sleep_frequency(&gs_handle, ADXL345_SLEEP_FREQUENCY_1HZ);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set sleep frequency failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set bypass mode */
    res = adxl345_set_mode(&gs_handle, ADXL345_MODE_BYPASS);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set mode failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set trigger pin int 2 */
    res = adxl345_set_trigger_pin(&gs_handle, ADXL345_INTERRUPT_PIN2);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set trigger pin failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set watermark 16 leval */
    res = adxl345_set_watermark(&gs_handle, 16);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set watermark failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set offset */
    res = adxl345_offset_convert_to_register(&gs_handle, 0.0f, (int8_t *)&reg);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: offset convert to register failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set offset 0 */
    res = adxl345_set_offset(&gs_handle, reg, reg, reg);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set offset failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set threshold 3g */
    res = adxl345_tap_threshold_convert_to_register(&gs_handle, 3.0f, (uint8_t *)&reg);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: tap threshold convert to register failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set tap threshold */
    res = adxl345_set_tap_threshold(&gs_handle, reg);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set tap threshold failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set 10 ms */
    res = adxl345_duration_convert_to_register(&gs_handle, 10 * 1000, (uint8_t *)&reg);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: duration convert to register failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set duration */
    res = adxl345_set_duration(&gs_handle, reg);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set duration failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set 20 ms */
    res = adxl345_latent_convert_to_register(&gs_handle, 20.0f, (uint8_t *)&reg);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: latent convert to register failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set latent */
    res = adxl345_set_latent(&gs_handle, reg);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set latent failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set 80 ms */
    res = adxl345_window_convert_to_register(&gs_handle, 80.0f, (uint8_t *)&reg);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: window convert to register failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set windows time */
    res = adxl345_set_window(&gs_handle, reg);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set window failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set x axis */
    res = adxl345_set_tap_axis(&gs_handle, ADXL345_TAP_AXIS_X, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set tap axis failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set y axis */
    res = adxl345_set_tap_axis(&gs_handle, ADXL345_TAP_AXIS_Y, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set tap axis failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set z axis */
    res = adxl345_set_tap_axis(&gs_handle, ADXL345_TAP_AXIS_Z, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set tap axis failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable suppress */
    res = adxl345_set_tap_suppress(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set tap suppress failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* map int 1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_SINGLE_TAP, ADXL345_INTERRUPT_PIN1);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set single tap */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_SINGLE_TAP, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* map int 1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_DOUBLE_TAP, ADXL345_INTERRUPT_PIN1);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set double tap */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_DOUBLE_TAP, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* link activity and inactivity */
    res = adxl345_set_link_activity_inactivity(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set link activity inactivity failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_action_threshold_convert_to_register(&gs_handle, 2.0f, (uint8_t *)&reg);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: action threshold convert to register failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set action */
    res = adxl345_set_action_threshold(&gs_handle, reg);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set action threshold failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_inaction_threshold_convert_to_register(&gs_handle, 1.0f, (uint8_t *)&reg);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: inaction threshold convert to register failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set inaction threshold */
    res = adxl345_set_inaction_threshold(&gs_handle, reg);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set inaction threshold failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set 3 s */
    res = adxl345_inaction_time_convert_to_register(&gs_handle, 3, (uint8_t *)&reg);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: inaction time convert to register failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set inaction time */
    res = adxl345_set_inaction_time(&gs_handle, reg);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set inaction time failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set action x */
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_ACTION_X, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set action y */
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_ACTION_Y, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set action z */
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_ACTION_Z, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set inaction x */
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_INACTION_X, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set inaction y */
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_INACTION_Y, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set inaction z */
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_INACTION_Z, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set action ac coupled */
    res = adxl345_set_action_coupled(&gs_handle, ADXL345_COUPLED_AC);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set action coupled failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set inaction dc coupled */
    res = adxl345_set_inaction_coupled(&gs_handle, ADXL345_COUPLED_DC);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set inaction coupled failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* map int 1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_ACTIVITY, ADXL345_INTERRUPT_PIN1);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set activity */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_ACTIVITY, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* map int 1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_INACTIVITY, ADXL345_INTERRUPT_PIN1);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set inactivity */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_INACTIVITY, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set free fall */
    res = adxl345_free_fall_threshold_convert_to_register(&gs_handle, 0.8f, (uint8_t *)&reg);     
    if (res)
    {
        adxl345_interface_debug_print("adxl345: free fall threshold convert to register failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set free fall threshold */
    res = adxl345_set_free_fall_threshold(&gs_handle, reg);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set free fall threshold failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set 10 ms */
    res = adxl345_free_fall_time_convert_to_register(&gs_handle, 10, (uint8_t *)&reg);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: free fall time convert to register failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set free fall time */
    res = adxl345_set_free_fall_time(&gs_handle, reg);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set free fall time failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt 1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_FREE_FALL, ADXL345_INTERRUPT_PIN1);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set fall */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_FREE_FALL, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt 1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_DATA_READY, ADXL345_INTERRUPT_PIN1);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set data ready */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_DATA_READY, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt 1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_WATERMARK, ADXL345_INTERRUPT_PIN1);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set data ready */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_WATERMARK, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt 1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_OVERRUN, ADXL345_INTERRUPT_PIN1);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set data ready */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_OVERRUN, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start read test */
    adxl345_interface_debug_print("adxl345: start read test.\n");
    
    /* set justify right */
    adxl345_interface_debug_print("adxl345: set align right.\n");
    res = adxl345_set_justify(&gs_handle, ADXL345_JUSTIFY_RIGHT);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set justify failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set 100 rate */
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_100);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set full resolution */
    res = adxl345_set_full_resolution(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set full resolution failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* 2g */
    adxl345_interface_debug_print("adxl345: set 2g range.\n");
    res = adxl345_set_range(&gs_handle, ADXL345_RANGE_2G);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set range failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* 4g */
    adxl345_interface_debug_print("adxl345: set 4g range.\n");
    res = adxl345_set_range(&gs_handle, ADXL345_RANGE_4G);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set range failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);                                           
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* 8g */
    adxl345_interface_debug_print("adxl345: set 8g range.\n");
    res = adxl345_set_range(&gs_handle, ADXL345_RANGE_8G);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set range failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* 16g */
    adxl345_interface_debug_print("adxl345: set 16g range.\n");
    res = adxl345_set_range(&gs_handle, ADXL345_RANGE_16G);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set range failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* full resolution */
    adxl345_interface_debug_print("adxl345: set full resolution.\n");
    res = adxl345_set_full_resolution(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set full resolution failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set align left */
    adxl345_interface_debug_print("adxl345: set align left.\n");
    res = adxl345_set_justify(&gs_handle, ADXL345_JUSTIFY_LEFT);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set justify failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set full resolution */
    res = adxl345_set_full_resolution(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set full resolution failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* 2g */
    adxl345_interface_debug_print("adxl345: set 2g range.\n");
    res = adxl345_set_range(&gs_handle, ADXL345_RANGE_2G);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set range failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* 4g */
    adxl345_interface_debug_print("adxl345: set 4g range.\n");
    res = adxl345_set_range(&gs_handle, ADXL345_RANGE_4G);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set range failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* 8g */
    adxl345_interface_debug_print("adxl345: set 8g range.\n");
    res = adxl345_set_range(&gs_handle, ADXL345_RANGE_8G);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set range failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* 16g */
    adxl345_interface_debug_print("adxl345: set 16g range.\n");
    res = adxl345_set_range(&gs_handle, ADXL345_RANGE_16G);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set range failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* full resolution */
    adxl345_interface_debug_print("adxl345: set full resolution.\n");
    res = adxl345_set_full_resolution(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set full resolution failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set justify right */
    res = adxl345_set_justify(&gs_handle, ADXL345_JUSTIFY_RIGHT);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set justify failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set rate 0.1Hz */
    adxl345_interface_debug_print("adxl345: set rate 0.1Hz.\n"); 
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_0P1);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set rate 0.2Hz */
    adxl345_interface_debug_print("adxl345: set rate 0.2Hz.\n"); 
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_0P2);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set rate 0.39Hz */
    adxl345_interface_debug_print("adxl345: set rate 0.39Hz.\n");
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_0P39);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set rate 0.78Hz */
    adxl345_interface_debug_print("adxl345: set rate 0.78Hz.\n");
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_0P78);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set rate 1.56Hz */
    adxl345_interface_debug_print("adxl345: set rate 1.56Hz.\n");
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_1P56);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set rate 3.13Hz */
    adxl345_interface_debug_print("adxl345: set rate 3.13Hz.\n");
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_3P13);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set rate 6.25Hz */
    adxl345_interface_debug_print("adxl345: set rate 6.25Hz.\n");
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_6P25);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set rate 12.5Hz */
    adxl345_interface_debug_print("adxl345: set rate 12.5Hz.\n");
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_12P5);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000); 
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set rate 25Hz */
    adxl345_interface_debug_print("adxl345: set rate 25Hz.\n");
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_25);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set rate 50Hz */
    adxl345_interface_debug_print("adxl345: set rate 50Hz.\n"); 
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_50);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set rate 100Hz */
    adxl345_interface_debug_print("adxl345: set rate 100Hz.\n"); 
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_100);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set rate 200Hz */
    adxl345_interface_debug_print("adxl345: set rate 200Hz.\n");
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_200);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set rate 400Hz */
    adxl345_interface_debug_print("adxl345: set rate 400Hz.\n");
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_400);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set rate 800Hz */
    adxl345_interface_debug_print("adxl345: set rate 800Hz.\n");
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_800);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set rate 1600Hz */
    adxl345_interface_debug_print("adxl345: set rate 1600Hz.\n");
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_1600);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set rate 3200Hz */
    adxl345_interface_debug_print("adxl345: set rate 3200Hz.\n");
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_3200);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set rate low power 12.5Hz */
    adxl345_interface_debug_print("adxl345: set rate low power 12.5Hz.\n");
    res = adxl345_set_rate(&gs_handle, ADXL345_LOW_POWER_RATE_12P5);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set rate low power 25Hz */
    adxl345_interface_debug_print("adxl345: set rate low power 25Hz.\n");
    res = adxl345_set_rate(&gs_handle, ADXL345_LOW_POWER_RATE_25);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set rate low power 50Hz */
    adxl345_interface_debug_print("adxl345: set rate low power 50Hz.\n");
    res = adxl345_set_rate(&gs_handle, ADXL345_LOW_POWER_RATE_50);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set rate low power 100Hz */
    adxl345_interface_debug_print("adxl345: set rate low power 100Hz.\n");
    res = adxl345_set_rate(&gs_handle, ADXL345_LOW_POWER_RATE_100);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set rate low power 200Hz */
    adxl345_interface_debug_print("adxl345: set rate low power 200Hz.\n"); 
    res = adxl345_set_rate(&gs_handle, ADXL345_LOW_POWER_RATE_200);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* set rate low power 400Hz */
    adxl345_interface_debug_print("adxl345: set rate low power 400Hz.\n");
    res = adxl345_set_rate(&gs_handle, ADXL345_LOW_POWER_RATE_400);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        volatile uint16_t len;
        
        len = 1;
        
        /* read data */
        if (adxl345_read(&gs_handle, (int16_t (*)[3])gs_raw_test, (float (*)[3])gs_test, (uint16_t *)&len))
        {
            adxl345_interface_debug_print("adxl345: read failed.\n");
            adxl345_deinit(&gs_handle);
            
            return 1;
        }
        adxl345_interface_debug_print("x is %0.2f g.\n", gs_test[0][0]);
        adxl345_interface_debug_print("y is %0.2f g.\n", gs_test[0][1]);
        adxl345_interface_debug_print("z is %0.2f g.\n", gs_test[0][2]);
        adxl345_interface_delay_ms(1000);
    }
    
    /* stop measure */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    /* finish read test */
    adxl345_interface_debug_print("adxl345: finish read test.\n");
    adxl345_deinit(&gs_handle);
    
    return 0;
}