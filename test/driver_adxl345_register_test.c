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
 * @file      driver_adxl345_register_test.c
 * @brief     driver adxl345 register test source file
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

#include "driver_adxl345_register_test.h"
#include <stdlib.h>

static adxl345_handle_t gs_handle;        /**< adxl345 handle */

/**
 * @brief     register test
 * @param[in] interface is the chip interface
 * @param[in] addr_pin is the iic device address
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t adxl345_register_test(adxl345_interface_t interface, adxl345_address_t addr_pin)
{
    uint8_t res, reg, check;
    int8_t x, y, z;
    int8_t x_check, y_check, z_check;
    float f, f_check;
    int8_t i_reg;
    uint32_t us, us_check;
    uint16_t ms, ms_check;
    adxl345_info_t info;
    adxl345_interface_t interface_test;
    adxl345_address_t addr;
    adxl345_bool_t bool_test;
    adxl345_coupled_t coupled;
    adxl345_rate_t rate;
    adxl345_interrupt_active_level_t pin_type;
    adxl345_interrupt_pin_t pin;
    adxl345_justify_t justify;
    adxl345_range_t range;
    adxl345_mode_t mode;
    adxl345_sleep_frequency_t sleep_frequency;
    adxl345_trigger_status_t trigger;
    
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
    
    /* start register test */
    adxl345_interface_debug_print("adxl345: start register test.\n");
    
    /* adxl345_set_interface/adxl345_get_interface test */
    adxl345_interface_debug_print("adxl345: adxl345_set_interface/adxl345_get_interface test.\n");
    
    /* set IIC interface */
    res = adxl345_set_interface(&gs_handle, ADXL345_INTERFACE_IIC);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interface failed.\n");
       
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interface iic.\n");
    res = adxl345_get_interface(&gs_handle, &interface_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interface failed.\n");
       
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interface %s.\n", interface_test==ADXL345_INTERFACE_IIC?"ok":"error");
    
    /* set SPI interface */
    res = adxl345_set_interface(&gs_handle, ADXL345_INTERFACE_SPI);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interface failed.\n");
       
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interface spi.\n");
    res = adxl345_get_interface(&gs_handle, &interface_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interface failed.\n");
       
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interface %s.\n", interface_test==ADXL345_INTERFACE_SPI?"ok":"error");
    
    /* adxl345_set_addr_pin/adxl345_get_addr_pin test */
    adxl345_interface_debug_print("adxl345: adxl345_set_addr_pin/adxl345_get_addr_pin test.\n");
    
    /* ALT0 */
    res = adxl345_set_addr_pin(&gs_handle, ADXL345_ADDRESS_ALT_0);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set addr pin failed.\n");
       
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set addr pin alt0.\n");
    res = adxl345_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get addr pin failed.\n");
       
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check addr pin %s.\n", addr==ADXL345_ADDRESS_ALT_0?"ok":"error");
    
    /* ALT1 */
    res = adxl345_set_addr_pin(&gs_handle, ADXL345_ADDRESS_ALT_1);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set addr pin failed.\n");
       
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set addr pin alt1.\n");
    res = adxl345_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get addr pin failed.\n");
       
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check addr pin %s.\n", addr==ADXL345_ADDRESS_ALT_1?"ok":"error");
    
    /* set the interface */
    res = adxl345_set_interface(&gs_handle, interface);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interface failed.\n");
       
        return 1;
    }
    
    /* set the address pin */
    res = adxl345_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set addr pin failed.\n");
       
        return 1;
    }
    
    /* init the adxl345 */
    res = adxl345_init(&gs_handle);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: init failed.\n");
       
        return 1;
    }
    
    /* adxl345_set_tap_threshold/adxl345_get_tap_threshold test */
    adxl345_interface_debug_print("adxl345: adxl345_set_tap_threshold/adxl345_get_tap_threshold test.\n");
    reg = rand()%256;
    res = adxl345_set_tap_threshold(&gs_handle, reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set tap threshold failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set tap threshold 0x%02X.\n", reg);
    res = adxl345_get_tap_threshold(&gs_handle, (uint8_t *)&check);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get tap threshold failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check tap threshold %s.\n", check==reg?"ok":"error");
    
    /* adxl345_set_offset/adxl345_get_offset test */
    adxl345_interface_debug_print("adxl345: adxl345_set_offset/adxl345_get_offset test.\n");
    x = rand()%128;
    y = rand()%128;
    y = -y;
    z = rand()%128;
    res = adxl345_set_offset(&gs_handle, x, y, z);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set offset failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set x offset %d.\n", x);
    adxl345_interface_debug_print("adxl345: set y offset %d.\n", y);
    adxl345_interface_debug_print("adxl345: set z offset %d.\n", z);
    res = adxl345_get_offset(&gs_handle, (int8_t *)&x_check, (int8_t *)&y_check, (int8_t *)&z_check);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get offset failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check x offset %s.\n", x==x_check?"ok":"error");
    adxl345_interface_debug_print("adxl345: check y offset %s.\n", y==y_check?"ok":"error");
    adxl345_interface_debug_print("adxl345: check z offset %s.\n", z==z_check?"ok":"error");
    
    /* adxl345_set_duration/adxl345_get_duration test */
    adxl345_interface_debug_print("adxl345: adxl345_set_duration/adxl345_get_duration test.\n");
    reg = rand()%256;
    res = adxl345_set_duration(&gs_handle, reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set duration failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set duration 0x%02X.\n", reg);
    res = adxl345_get_duration(&gs_handle, (uint8_t *)&check);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get duration failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check duration %s.\n", check==reg?"ok":"error");
    
    /* adxl345_set_latent/adxl345_get_latent test */
    adxl345_interface_debug_print("adxl345: adxl345_set_latent/adxl345_get_latent test.\n");
    reg = rand()%256;
    res = adxl345_set_latent(&gs_handle, reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set latent failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set latent 0x%02X.\n", reg);
    res = adxl345_get_latent(&gs_handle, (uint8_t *)&check);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get latent failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check latent %s.\n", check==reg?"ok":"error");
    
    /* adxl345_set_window/adxl345_get_window test */
    adxl345_interface_debug_print("adxl345: adxl345_set_window/adxl345_get_window test.\n");
    reg = rand()%256;
    res = adxl345_set_window(&gs_handle, reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set window failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set window 0x%02X.\n", reg);
    res = adxl345_get_window(&gs_handle, (uint8_t *)&check);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get window failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check window %s.\n", check==reg?"ok":"error");
    
    /* adxl345_set_action_threshold/adxl345_get_action_threshold test */
    adxl345_interface_debug_print("adxl345: adxl345_set_action_threshold/adxl345_get_action_threshold test.\n");
    reg = rand()%256;
    res = adxl345_set_action_threshold(&gs_handle, reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action threshold failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set action threshold 0x%02X.\n", reg);
    res = adxl345_get_action_threshold(&gs_handle, (uint8_t *)&check);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get action threshold failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check action threshold %s.\n", check==reg?"ok":"error");
    
    /* adxl345_set_inaction_threshold/adxl345_get_inaction_threshold test */
    adxl345_interface_debug_print("adxl345: adxl345_set_inaction_threshold/adxl345_get_inaction_threshold test.\n");
    reg = rand()%256;
    res = adxl345_set_inaction_threshold(&gs_handle, reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set inaction threshold failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set inaction threshold 0x%02X.\n", reg);
    res = adxl345_get_inaction_threshold(&gs_handle, (uint8_t *)&check);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get inaction threshold failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check inaction threshold %s.\n", check==reg?"ok":"error");
    
    /* adxl345_set_inaction_time/adxl345_get_inaction_time test */
    adxl345_interface_debug_print("adxl345: adxl345_set_inaction_time/adxl345_get_inaction_time test.\n");
    reg = rand()%256;
    res = adxl345_set_inaction_time(&gs_handle, reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set inaction time failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set inaction time 0x%02X.\n", reg);
    res = adxl345_get_inaction_time(&gs_handle, (uint8_t *)&check);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get inaction time failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check inaction time %s.\n", check==reg?"ok":"error");
    
    /* adxl345_set_action_inaction/adxl345_get_action_inaction test */
    adxl345_interface_debug_print("adxl345: adxl345_set_action_inaction/adxl345_get_action_inaction test.\n");
    
    /* action_x_enable */ 
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_ACTION_X, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set action x enable.\n");
    res = adxl345_get_action_inaction(&gs_handle, ADXL345_ACTION_X, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check action inaction %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* action_x_disable */
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_ACTION_X, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set action x disable.\n");
    res = adxl345_get_action_inaction(&gs_handle, ADXL345_ACTION_X, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check action inaction %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* action_y_enable */
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_ACTION_Y, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set action y enable.\n");
    res = adxl345_get_action_inaction(&gs_handle, ADXL345_ACTION_Y, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check action inaction %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* action_y_disable */
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_ACTION_Y, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set action y disable.\n");
    res = adxl345_get_action_inaction(&gs_handle, ADXL345_ACTION_Y, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check action inaction %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* action_z_enable */
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_ACTION_Z, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set action z enable.\n");
    res = adxl345_get_action_inaction(&gs_handle, ADXL345_ACTION_Z, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check action inaction %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* action_z_disable */
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_ACTION_Z, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set action z disable.\n");
    res = adxl345_get_action_inaction(&gs_handle, ADXL345_ACTION_Z, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check action inaction %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* inaction_x_enable */ 
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_INACTION_X, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set inaction x enable.\n");
    res = adxl345_get_action_inaction(&gs_handle, ADXL345_INACTION_X, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check action inaction %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* inaction_x_disable */ 
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_INACTION_X, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set inaction x disable.\n");
    res = adxl345_get_action_inaction(&gs_handle, ADXL345_INACTION_X, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check action inaction %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* inaction_y_enable */
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_INACTION_Y, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set inaction y enable.\n");
    res = adxl345_get_action_inaction(&gs_handle, ADXL345_INACTION_Y, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check action inaction %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* inaction_y_disable */
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_INACTION_Y, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set inaction y disable.\n");
    res = adxl345_get_action_inaction(&gs_handle, ADXL345_INACTION_Y, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check action inaction %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* inaction_z_enable */
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_INACTION_Z, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set inaction z enable.\n");
    res = adxl345_get_action_inaction(&gs_handle, ADXL345_INACTION_Z, &bool_test);
    if (res != 0)
    { 
        adxl345_interface_debug_print("adxl345: get action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check action inaction %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* inaction_z_disable */
    res = adxl345_set_action_inaction(&gs_handle, ADXL345_INACTION_Z, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set inaction z disable.\n");
    res = adxl345_get_action_inaction(&gs_handle, ADXL345_INACTION_Z, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get action inaction failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check action inaction %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* adxl345_set_action_coupled/adxl345_get_action_coupled test */
    adxl345_interface_debug_print("adxl345: adxl345_set_action_coupled/adxl345_get_action_coupled test.\n");
    
    /* coupled dc */ 
    res = adxl345_set_action_coupled(&gs_handle, ADXL345_COUPLED_DC);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action coupled failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set action coupled dc.\n");
    res = adxl345_get_action_coupled(&gs_handle, &coupled);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get action coupled failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check action coupled %s.\n", coupled==ADXL345_COUPLED_DC?"ok":"error");
    
    /* coupled ac */
    res = adxl345_set_action_coupled(&gs_handle, ADXL345_COUPLED_AC);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set action coupled failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set action coupled ac.\n");
    res = adxl345_get_action_coupled(&gs_handle, &coupled);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get action coupled failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check action coupled %s.\n", coupled==ADXL345_COUPLED_AC?"ok":"error");
    
    /* adxl345_set_inaction_coupled/adxl345_get_inaction_coupled test */
    adxl345_interface_debug_print("adxl345: adxl345_set_inaction_coupled/adxl345_get_inaction_coupled test.\n");
    
    /* coupled dc */
    res = adxl345_set_inaction_coupled(&gs_handle, ADXL345_COUPLED_DC);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set inaction coupled failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set inaction coupled dc.\n");
    res = adxl345_get_inaction_coupled(&gs_handle, &coupled);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get inaction coupled failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check inaction coupled %s.\n", coupled==ADXL345_COUPLED_DC?"ok":"error");
    
    /* coupled ac */ 
    res = adxl345_set_inaction_coupled(&gs_handle, ADXL345_COUPLED_AC);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set inaction coupled failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set inaction coupled ac.\n");
    res = adxl345_get_inaction_coupled(&gs_handle, &coupled);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get inaction coupled failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check inaction coupled %s.\n", coupled==ADXL345_COUPLED_AC?"ok":"error");
    
    /* adxl345_set_free_fall_threshold/adxl345_get_free_fall_threshold test */
    adxl345_interface_debug_print("adxl345: adxl345_set_free_fall_threshold/adxl345_get_free_fall_threshold test.\n");
    reg = rand()%256;
    res = adxl345_set_free_fall_threshold(&gs_handle, reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set free fall threshold failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set free fall threshold 0x%02X.\n", reg);
    res = adxl345_get_free_fall_threshold(&gs_handle, (uint8_t *)&check);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get free fall threshold failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check free fall threshold %s.\n", check==reg?"ok":"error");
    
    /* adxl345_set_free_fall_time/adxl345_get_free_fall_time test */
    adxl345_interface_debug_print("adxl345: adxl345_set_free_fall_time/adxl345_get_free_fall_time test.\n");
    reg = rand()%256;
    res = adxl345_set_free_fall_time(&gs_handle, reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set free fall time failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set free fall time 0x%02X.\n", reg);
    res = adxl345_get_free_fall_time(&gs_handle, (uint8_t *)&check);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get free fall time failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check free fall time %s.\n", check==reg?"ok":"error");
    
    /* adxl345_set_tap_axis/adxl345_get_tap_axis test */
    adxl345_interface_debug_print("adxl345: adxl345_set_tap_axis/adxl345_get_tap_axis test.\n");
    
    /* tap_x_enable */ 
    res = adxl345_set_tap_axis(&gs_handle, ADXL345_TAP_AXIS_X, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set tap axis failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set tap axis x enable.\n");
    res = adxl345_get_tap_axis(&gs_handle, ADXL345_TAP_AXIS_X, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get tap axis failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check tap axis %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* tap_x_disable */
    res = adxl345_set_tap_axis(&gs_handle, ADXL345_TAP_AXIS_X, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set tap axis failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set tap axis x disable.\n");
    res = adxl345_get_tap_axis(&gs_handle, ADXL345_TAP_AXIS_X, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get tap axis failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check tap axis %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* tap_y_enable */
    res = adxl345_set_tap_axis(&gs_handle, ADXL345_TAP_AXIS_Y, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set tap axis failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set tap axis y enable.\n");
    res = adxl345_get_tap_axis(&gs_handle, ADXL345_TAP_AXIS_Y, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get tap axis failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check tap axis %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* tap_y_disable */
    res = adxl345_set_tap_axis(&gs_handle, ADXL345_TAP_AXIS_Y, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set tap axis failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set tap axis y disable.\n");
    res = adxl345_get_tap_axis(&gs_handle, ADXL345_TAP_AXIS_Y, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get tap axis failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check tap axis %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* tap_z_enable */
    res = adxl345_set_tap_axis(&gs_handle, ADXL345_TAP_AXIS_Z, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set tap axis failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set tap axis z enable.\n");
    res = adxl345_get_tap_axis(&gs_handle, ADXL345_TAP_AXIS_Z, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get tap axis failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check tap axis %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* tap_z_disable */
    res = adxl345_set_tap_axis(&gs_handle, ADXL345_TAP_AXIS_Z, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set tap axis failed.\n");
        (void)adxl345_deinit(&gs_handle);

        return 1;
    }
    adxl345_interface_debug_print("adxl345: set tap axis z disable.\n");
    res = adxl345_get_tap_axis(&gs_handle, ADXL345_TAP_AXIS_Z, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get tap axis failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check tap axis %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* adxl345_set_tap_suppress/adxl345_get_tap_suppress test */
    adxl345_interface_debug_print("adxl345: adxl345_set_tap_suppress/adxl345_get_tap_suppress test.\n");
    
    /* enable */
    res = adxl345_set_tap_suppress(&gs_handle, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set tap suppress failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set tap suppress enable.\n");
    res = adxl345_get_tap_suppress(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get tap suppress failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check tap suppress %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* disable */
    res = adxl345_set_tap_suppress(&gs_handle, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set tap suppress failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }    
    adxl345_interface_debug_print("adxl345: set tap suppress disable.\n");
    res = adxl345_get_tap_suppress(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get tap suppress failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check tap suppress %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* get tap status test */
    adxl345_interface_debug_print("adxl345: get tap status test.\n");
    res = adxl345_get_tap_status(&gs_handle, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get tap status failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: tap status is 0x%02X.\n", reg);
    
    /* adxl345_set_rate/adxl345_get_rate test */
    adxl345_interface_debug_print("adxl345: adxl345_set_rate/adxl345_get_rate test.\n");
    
    /* 0.1Hz */ 
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_0P1);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set rate 0.1Hz.\n");
    res = adxl345_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check rate %s.\n", rate==ADXL345_RATE_0P1?"ok":"error");
    
    /* 0.2Hz */ 
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_0P2);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set rate 0.2Hz.\n");
    res = adxl345_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check rate %s.\n", rate==ADXL345_RATE_0P2?"ok":"error");
    
    /* 0.39Hz */
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_0P39);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set rate 0.39Hz.\n");
    res = adxl345_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check rate %s.\n", rate==ADXL345_RATE_0P39?"ok":"error");
    
    /* 0.78Hz */
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_0P78);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set rate 0.78Hz.\n");
    res = adxl345_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check rate %s.\n", rate==ADXL345_RATE_0P78?"ok":"error");
    
    /* 1.56Hz */
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_1P56);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set rate 1.56Hz.\n");
    res = adxl345_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check rate %s.\n", rate==ADXL345_RATE_1P56?"ok":"error");
    
    /* 3.13Hz */
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_3P13);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set rate 3.13Hz.\n");
    res = adxl345_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check rate %s.\n", rate==ADXL345_RATE_3P13?"ok":"error");
    
    /* 6.25Hz */
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_6P25);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set rate 6.25Hz.\n");
    res = adxl345_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check rate %s.\n", rate==ADXL345_RATE_6P25?"ok":"error");
    
    /* 12.5Hz */ 
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_12P5);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set rate 12.5Hz.\n");
    res = adxl345_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check rate %s.\n", rate==ADXL345_RATE_12P5?"ok":"error");
    
    /* 25Hz */
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_25);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set rate 25Hz.\n");
    res = adxl345_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check rate %s.\n", rate==ADXL345_RATE_25?"ok":"error");
    
    /* 50Hz */ 
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_50);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set rate 50Hz.\n");
    res = adxl345_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check rate %s.\n", rate==ADXL345_RATE_50?"ok":"error");
    
    /* 100Hz */
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_100);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set rate 100Hz.\n");
    res = adxl345_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check rate %s.\n", rate==ADXL345_RATE_100?"ok":"error");
    
    /* 200Hz */
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_200);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set rate 200Hz.\n");
    res = adxl345_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check rate %s.\n", rate==ADXL345_RATE_200?"ok":"error");
    
    /* 400Hz */
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_400);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set rate 400Hz.\n");
    res = adxl345_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check rate %s.\n", rate==ADXL345_RATE_400?"ok":"error");
    
    /* 800Hz */
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_800);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set rate 800Hz.\n");
    res = adxl345_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check rate %s.\n", rate==ADXL345_RATE_800?"ok":"error");
    
    /* 1600Hz */
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_1600);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set rate 1600Hz.\n");
    res = adxl345_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check rate %s.\n", rate==ADXL345_RATE_1600?"ok":"error");
    
    /* 3200Hz */
    res = adxl345_set_rate(&gs_handle, ADXL345_RATE_3200);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set rate 3200Hz.\n");
    res = adxl345_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check rate %s.\n", rate==ADXL345_RATE_3200?"ok":"error");
    
    /* low 12.5Hz */
    res = adxl345_set_rate(&gs_handle, ADXL345_LOW_POWER_RATE_12P5);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set rate low 12.5Hz.\n");
    res = adxl345_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check rate %s.\n", rate==ADXL345_LOW_POWER_RATE_12P5?"ok":"error");
    
    /* low 25Hz */
    res = adxl345_set_rate(&gs_handle, ADXL345_LOW_POWER_RATE_25);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set rate low 25Hz.\n");
    res = adxl345_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check rate %s.\n", rate==ADXL345_LOW_POWER_RATE_25?"ok":"error");
    
    /* low 50Hz */
    res = adxl345_set_rate(&gs_handle, ADXL345_LOW_POWER_RATE_50);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set rate low 50Hz.\n");
    res = adxl345_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check rate %s.\n", rate==ADXL345_LOW_POWER_RATE_50?"ok":"error");
    
    /* low 100Hz */
    res = adxl345_set_rate(&gs_handle, ADXL345_LOW_POWER_RATE_100);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set rate low 100Hz.\n");
    res = adxl345_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check rate %s.\n", rate==ADXL345_LOW_POWER_RATE_100?"ok":"error");
    
    /* low 200Hz */
    res = adxl345_set_rate(&gs_handle, ADXL345_LOW_POWER_RATE_200);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set rate low 200Hz.\n");
    res = adxl345_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check rate %s.\n", rate==ADXL345_LOW_POWER_RATE_200?"ok":"error");
    
    /* low 400Hz */
    res = adxl345_set_rate(&gs_handle, ADXL345_LOW_POWER_RATE_400);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set rate low 400Hz.\n");
    res = adxl345_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get rate failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check rate %s.\n", rate==ADXL345_LOW_POWER_RATE_400?"ok":"error");
    
    /* adxl345_set_interrupt/adxl345_get_interrupt test */
    adxl345_interface_debug_print("adxl345: adxl345_set_interrupt/adxl345_get_interrupt test.\n");
    
    /* data enable */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_DATA_READY, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt data ready enable.\n");
    res = adxl345_get_interrupt(&gs_handle, ADXL345_INTERRUPT_DATA_READY, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* data disable */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_DATA_READY, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt data ready disable.\n");
    res = adxl345_get_interrupt(&gs_handle, ADXL345_INTERRUPT_DATA_READY, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* single enable */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_SINGLE_TAP, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt single enable.\n");
    res = adxl345_get_interrupt(&gs_handle, ADXL345_INTERRUPT_SINGLE_TAP, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* single disable */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_SINGLE_TAP, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt single disable.\n");
    res = adxl345_get_interrupt(&gs_handle, ADXL345_INTERRUPT_SINGLE_TAP, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
   
    /* double enable */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_DOUBLE_TAP, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt double enable.\n");
    res = adxl345_get_interrupt(&gs_handle, ADXL345_INTERRUPT_DOUBLE_TAP, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* double disable */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_DOUBLE_TAP, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt double disable.\n");
    res = adxl345_get_interrupt(&gs_handle, ADXL345_INTERRUPT_DOUBLE_TAP, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* activity enable */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_ACTIVITY, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt activity enable.\n");
    res = adxl345_get_interrupt(&gs_handle, ADXL345_INTERRUPT_ACTIVITY, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* activity disable */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_ACTIVITY, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt activity disable.\n");
    res = adxl345_get_interrupt(&gs_handle, ADXL345_INTERRUPT_ACTIVITY, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* inactivity enable */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_INACTIVITY, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt inactivity enable.\n");
    res = adxl345_get_interrupt(&gs_handle, ADXL345_INTERRUPT_INACTIVITY, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* inactivity disable */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_INACTIVITY, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt inactivity disable.\n");
    res = adxl345_get_interrupt(&gs_handle, ADXL345_INTERRUPT_INACTIVITY, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* free fall enable */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_FREE_FALL, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt free fall enable.\n");
    res = adxl345_get_interrupt(&gs_handle, ADXL345_INTERRUPT_FREE_FALL, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* free fall disable */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_FREE_FALL, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt free fall disable.\n");
    res = adxl345_get_interrupt(&gs_handle, ADXL345_INTERRUPT_FREE_FALL, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* watermark enable */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_WATERMARK, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt watermark enable.\n");
    res = adxl345_get_interrupt(&gs_handle, ADXL345_INTERRUPT_WATERMARK, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* watermark disable */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_WATERMARK, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt watermark disable.\n");
    res = adxl345_get_interrupt(&gs_handle, ADXL345_INTERRUPT_WATERMARK, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* overrun enable */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_OVERRUN, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt overrun enable.\n");
    res = adxl345_get_interrupt(&gs_handle, ADXL345_INTERRUPT_OVERRUN, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* overrun disable */
    res = adxl345_set_interrupt(&gs_handle, ADXL345_INTERRUPT_OVERRUN, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt overrun disable.\n");
    res = adxl345_get_interrupt(&gs_handle, ADXL345_INTERRUPT_OVERRUN, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* adxl345_set_interrupt_map/adxl345_get_interrupt_map test */
    adxl345_interface_debug_print("adxl345: adxl345_set_interrupt_map/adxl345_get_interrupt_map test.\n");
    
    /* data pin1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_DATA_READY, ADXL345_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt map data ready pin1.\n");
    res = adxl345_get_interrupt_map(&gs_handle, ADXL345_INTERRUPT_DATA_READY, &pin);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt map %s.\n", pin==ADXL345_INTERRUPT_PIN1?"ok":"error");
    
    /* data pin2 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_DATA_READY, ADXL345_INTERRUPT_PIN2);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt map data ready pin2.\n");
    res = adxl345_get_interrupt_map(&gs_handle, ADXL345_INTERRUPT_DATA_READY, &pin);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt map %s.\n", pin==ADXL345_INTERRUPT_PIN2?"ok":"error");
    
    /* single tap pin1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_SINGLE_TAP, ADXL345_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt map single tap pin1.\n");
    res = adxl345_get_interrupt_map(&gs_handle, ADXL345_INTERRUPT_SINGLE_TAP, &pin);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt map %s.\n", pin==ADXL345_INTERRUPT_PIN1?"ok":"error");
    
    /* single pin2 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_SINGLE_TAP, ADXL345_INTERRUPT_PIN2);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt map single pin2.\n");
    res = adxl345_get_interrupt_map(&gs_handle, ADXL345_INTERRUPT_SINGLE_TAP, &pin);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt map %s.\n", pin==ADXL345_INTERRUPT_PIN2?"ok":"error");
    
    /* double tap pin1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_DOUBLE_TAP, ADXL345_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt map double tap pin1.\n");
    res = adxl345_get_interrupt_map(&gs_handle, ADXL345_INTERRUPT_DOUBLE_TAP, &pin);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt map %s.\n", pin==ADXL345_INTERRUPT_PIN1?"ok":"error");
    
    /* double pin2 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_DOUBLE_TAP, ADXL345_INTERRUPT_PIN2);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt map double pin2.\n");
    res = adxl345_get_interrupt_map(&gs_handle, ADXL345_INTERRUPT_DOUBLE_TAP, &pin);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt map %s.\n", pin==ADXL345_INTERRUPT_PIN2?"ok":"error");
    
    /* activity pin1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_ACTIVITY, ADXL345_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt map activity pin1.\n");
    res = adxl345_get_interrupt_map(&gs_handle, ADXL345_INTERRUPT_ACTIVITY, &pin);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt map %s.\n", pin==ADXL345_INTERRUPT_PIN1?"ok":"error");
    
    /* activity pin2 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_ACTIVITY, ADXL345_INTERRUPT_PIN2);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt activity pin2.\n");
    res = adxl345_get_interrupt_map(&gs_handle, ADXL345_INTERRUPT_ACTIVITY, &pin);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt map %s.\n", pin==ADXL345_INTERRUPT_PIN2?"ok":"error");
    
    /* inactivity pin1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_INACTIVITY, ADXL345_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt map inactivity pin1.\n");
    res = adxl345_get_interrupt_map(&gs_handle, ADXL345_INTERRUPT_INACTIVITY, &pin);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt map %s.\n", pin==ADXL345_INTERRUPT_PIN1?"ok":"error");
    
    /* inactivity pin2 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_INACTIVITY, ADXL345_INTERRUPT_PIN2);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt inactivity pin2.\n");
    res = adxl345_get_interrupt_map(&gs_handle, ADXL345_INTERRUPT_INACTIVITY, &pin);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt map %s.\n", pin==ADXL345_INTERRUPT_PIN2?"ok":"error");
    
    /* free fall pin1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_FREE_FALL, ADXL345_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt map free fall pin1.\n");
    res = adxl345_get_interrupt_map(&gs_handle, ADXL345_INTERRUPT_FREE_FALL, &pin);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt map %s.\n", pin==ADXL345_INTERRUPT_PIN1?"ok":"error");
    
    /* free fall pin2 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_FREE_FALL, ADXL345_INTERRUPT_PIN2);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt free fall pin2.\n");
    res = adxl345_get_interrupt_map(&gs_handle, ADXL345_INTERRUPT_FREE_FALL, &pin);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt map %s.\n", pin==ADXL345_INTERRUPT_PIN2?"ok":"error");
    
    /* watermark pin1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_WATERMARK, ADXL345_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt map watermark pin1.\n");
    res = adxl345_get_interrupt_map(&gs_handle, ADXL345_INTERRUPT_WATERMARK, &pin);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt map %s.\n", pin==ADXL345_INTERRUPT_PIN1?"ok":"error");
    
    /* watermark pin2 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_WATERMARK, ADXL345_INTERRUPT_PIN2);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt watermark pin2.\n");
    res = adxl345_get_interrupt_map(&gs_handle, ADXL345_INTERRUPT_WATERMARK, &pin);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt map %s.\n", pin==ADXL345_INTERRUPT_PIN2?"ok":"error");
    
    /* overrun pin1 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_OVERRUN, ADXL345_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt map overrun pin1.\n");
    res = adxl345_get_interrupt_map(&gs_handle, ADXL345_INTERRUPT_OVERRUN, &pin);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt map %s.\n", pin==ADXL345_INTERRUPT_PIN1?"ok":"error");
    
    /* overrun pin2 */
    res = adxl345_set_interrupt_map(&gs_handle, ADXL345_INTERRUPT_OVERRUN, ADXL345_INTERRUPT_PIN2);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt overrun pin2.\n");
    res = adxl345_get_interrupt_map(&gs_handle, ADXL345_INTERRUPT_OVERRUN, &pin);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt map failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt map %s.\n", pin==ADXL345_INTERRUPT_PIN2?"ok":"error");
    
    /* adxl345_get_interrupt_source test */
    adxl345_interface_debug_print("adxl345: adxl345_get_interrupt_source test.\n");
    res = adxl345_get_interrupt_source(&gs_handle, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt source failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt source 0x%02X.\n", res);
    
    /* adxl345_set_self_test/adxl345_get_self_test test */
    adxl345_interface_debug_print("adxl345: adxl345_set_self_test/adxl345_get_self_test test.\n");
    
    /* enable */
    res = adxl345_set_self_test(&gs_handle, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set self test failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set self test enable.\n");
    res = adxl345_get_self_test(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get self test failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check self test %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* disable */
    res = adxl345_set_self_test(&gs_handle, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set self test failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set self test disable.\n");
    res = adxl345_get_self_test(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get self test failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check self test %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* adxl345_set_spi_wire/adxl345_get_spi_wire test */
    adxl345_interface_debug_print("adxl345: adxl345_set_spi_wire/adxl345_get_spi_wire test.\n");
    
    /* enable */
    res = adxl345_set_self_test(&gs_handle, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set self test failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set self test enable.\n");
    res = adxl345_get_self_test(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get self test failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check self test %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* disable */
    res = adxl345_set_self_test(&gs_handle, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set self test failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set self test disable.\n");
    res = adxl345_get_self_test(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get self test failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check self test %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* adxl345_set_interrupt_active_level/adxl345_get_interrupt_active_level test */
    adxl345_interface_debug_print("adxl345: adxl345_set_interrupt_active_level/adxl345_get_interrupt_active_level test.\n");
    
    /* high */
    res = adxl345_set_interrupt_active_level(&gs_handle, ADXL345_INTERRUPT_ACTIVE_LEVEL_HIGH);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt active level failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt active level high.\n");
    res = adxl345_get_interrupt_active_level(&gs_handle, &pin_type);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt active level failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt active level %s.\n", pin_type==ADXL345_INTERRUPT_ACTIVE_LEVEL_HIGH?"ok":"error");
    
    /* low */
    res = adxl345_set_interrupt_active_level(&gs_handle, ADXL345_INTERRUPT_ACTIVE_LEVEL_LOW);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set interrupt active level failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set interrupt active level low.\n");
    res = adxl345_get_interrupt_active_level(&gs_handle, &pin_type);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get interrupt active level failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check interrupt active level %s.\n", pin_type==ADXL345_INTERRUPT_ACTIVE_LEVEL_LOW?"ok":"error");
    
    /* adxl345_set_full_resolution/adxl345_get_full_resolution test */
    adxl345_interface_debug_print("adxl345: adxl345_set_full_resolution/adxl345_get_full_resolution test.\n");
    
    /* enable */
    res = adxl345_set_full_resolution(&gs_handle, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set full resolution failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: enable full resolution.\n");
    res = adxl345_get_full_resolution(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get full resolution failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check full resolution %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* disable */
    res = adxl345_set_full_resolution(&gs_handle, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set full resolution failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: disable full resolution.\n");
    res = adxl345_get_full_resolution(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get full resolution failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check full resolution %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* adxl345_set_justify/adxl345_get_justify test */
    adxl345_interface_debug_print("adxl345: adxl345_set_justify/adxl345_get_justify test.\n");
    
    /* right */
    res = adxl345_set_justify(&gs_handle, ADXL345_JUSTIFY_RIGHT);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set justify failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set justify right.\n");
    res = adxl345_get_justify(&gs_handle, &justify);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get justify failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check justify %s.\n", justify==ADXL345_JUSTIFY_RIGHT?"ok":"error");
    
    /* left */
    res = adxl345_set_justify(&gs_handle, ADXL345_JUSTIFY_LEFT);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set justify failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set justify left.\n");
    res = adxl345_get_justify(&gs_handle, &justify);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get justify failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check justify %s.\n", justify==ADXL345_JUSTIFY_LEFT?"ok":"error");
    
    /* adxl345_set_range/adxl345_get_range test */
    adxl345_interface_debug_print("adxl345: adxl345_set_range/adxl345_get_range test.\n");
    
    /* 2g */
    res = adxl345_set_range(&gs_handle, ADXL345_RANGE_2G);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set range failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set range 2g.\n");
    res = adxl345_get_range(&gs_handle, &range);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get range failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check range %s.\n", range==ADXL345_RANGE_2G?"ok":"error");
    
    /* 4g */
    res = adxl345_set_range(&gs_handle, ADXL345_RANGE_4G);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set range failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set range 4g.\n");
    res = adxl345_get_range(&gs_handle, &range);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get range failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check range %s.\n", range==ADXL345_RANGE_4G?"ok":"error");
    
    /* 8g */
    res = adxl345_set_range(&gs_handle, ADXL345_RANGE_8G);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set range failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set range 8g.\n");
    res = adxl345_get_range(&gs_handle, &range);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get range failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check range %s.\n", range==ADXL345_RANGE_8G?"ok":"error");
    
    /* 16g */
    res = adxl345_set_range(&gs_handle, ADXL345_RANGE_16G);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set range failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set range 16g.\n");
    res = adxl345_get_range(&gs_handle, &range);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get range failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check range %s.\n", range==ADXL345_RANGE_16G?"ok":"error");
    
    /* adxl345_set_mode/adxl345_get_mode */
    adxl345_interface_debug_print("adxl345: adxl345_set_mode/adxl345_get_mode test.\n");
    
    /* bypass */
    res = adxl345_set_mode(&gs_handle, ADXL345_MODE_BYPASS);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set mode failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set mode bypass.\n");
    res = adxl345_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get mode failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check mode %s.\n", mode==ADXL345_MODE_BYPASS?"ok":"error");
   
    /* fifo */
    res = adxl345_set_mode(&gs_handle, ADXL345_MODE_FIFO);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set mode failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set mode fifo.\n");
    res = adxl345_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get mode failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check mode %s.\n", mode==ADXL345_MODE_FIFO?"ok":"error");
    
    /* stream */
    res = adxl345_set_mode(&gs_handle, ADXL345_MODE_STREAM);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set mode failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set mode stream.\n");
    res = adxl345_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get mode failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check mode %s.\n", mode==ADXL345_MODE_STREAM?"ok":"error");
    
    /* trigger */
    res = adxl345_set_mode(&gs_handle, ADXL345_MODE_TRIGGER);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set mode failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set mode trigger.\n");
    res = adxl345_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get mode failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check mode %s.\n", mode==ADXL345_MODE_TRIGGER?"ok":"error");
    
    /* adxl345_set_trigger_pin/adxl345_get_trigger_pin test */
    adxl345_interface_debug_print("adxl345: adxl345_set_trigger_pin/adxl345_get_trigger_pin test.\n");
    
    /* pin1 */
    res = adxl345_set_trigger_pin(&gs_handle, ADXL345_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set trigger pin failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set trigger pin 1.\n");
    res = adxl345_get_trigger_pin(&gs_handle, &pin);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get trigger pin failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check trigger pin %s.\n", pin==ADXL345_INTERRUPT_PIN1?"ok":"error");
    
    /* pin2 */
    res = adxl345_set_trigger_pin(&gs_handle, ADXL345_INTERRUPT_PIN2);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set trigger pin failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set trigger pin 2.\n");
    res = adxl345_get_trigger_pin(&gs_handle, &pin);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get trigger pin failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check trigger pin %s.\n", pin==ADXL345_INTERRUPT_PIN2?"ok":"error");
    
    /* adxl345_set_watermark/adxl345_get_watermark test */
    adxl345_interface_debug_print("adxl345: adxl345_set_watermark/adxl345_get_watermark test.\n");
    reg = rand()%32;
    res = adxl345_set_watermark(&gs_handle, reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set watermark failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set watermark 0x%02X.\n", reg);
    res = adxl345_get_watermark(&gs_handle, (uint8_t *)&check);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get watermark failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check watermark %s.\n", check==reg?"ok":"error");
    
    /* adxl345_get_watermark_level test */
    adxl345_interface_debug_print("adxl345: adxl345_get_watermark_level test.\n");
    res = adxl345_get_watermark_level(&gs_handle, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get watermark level failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: watermark level is 0x%02X.\n", reg);
    
    /* adxl345_get_trigger_status test */
    adxl345_interface_debug_print("adxl345: adxl345_get_trigger_status test.\n");
    res = adxl345_get_trigger_status(&gs_handle, (adxl345_trigger_status_t *)&trigger);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get trigger status failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: trigger status is 0x%02X.\n", trigger);
    
    /* adxl345_set_link_activity_inactivity/adxl345_get_link_activity_inactivity test */
    adxl345_interface_debug_print("adxl345: adxl345_set_link_activity_inactivity/adxl345_get_link_activity_inactivity test.\n");
    
    /* enable */
    res = adxl345_set_link_activity_inactivity(&gs_handle, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set link activity inactivity failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: enable link activity inactivity.\n");
    res = adxl345_get_link_activity_inactivity(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get link activity inactivity failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check link activity inactivity %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* disable */
    res = adxl345_set_link_activity_inactivity(&gs_handle, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set link activity inactivity failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: disable link activity inactivity.\n");
    res = adxl345_get_link_activity_inactivity(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get link activity inactivity    failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check link activity inactivity %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* adxl345_set_auto_sleep/adxl345_get_auto_sleep test */
    adxl345_interface_debug_print("adxl345: adxl345_set_auto_sleep/adxl345_get_auto_sleep test.\n");
    
    /* enable */ 
    res = adxl345_set_auto_sleep(&gs_handle, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set auto sleep failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: enable auto sleep.\n");
    res = adxl345_get_auto_sleep(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get auto sleep failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check auto sleep %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* disable */
    res = adxl345_set_auto_sleep(&gs_handle, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set auto sleep failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: disable auto sleep.\n");
    res = adxl345_get_auto_sleep(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get auto sleep failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check auto sleep %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* adxl345_set_measure/adxl345_get_measure test */
    adxl345_interface_debug_print("adxl345: adxl345_set_measure/adxl345_get_measure test.\n");
    
    /* enable */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: enable measure.\n");
    res = adxl345_get_measure(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get measure failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check measure %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* disable */
    res = adxl345_set_measure(&gs_handle, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set measure failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: disable measure.\n");
    res = adxl345_get_measure(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get measure failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check measure %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* adxl345_set_sleep/adxl345_get_sleep test */
    adxl345_interface_debug_print("adxl345: adxl345_set_sleep/adxl345_get_sleep test.\n");
    
    /* enable */ 
    res = adxl345_set_sleep(&gs_handle, ADXL345_BOOL_TRUE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set sleep failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: enable sleep.\n");
    res = adxl345_get_sleep(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get sleep failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check sleep %s.\n", bool_test==ADXL345_BOOL_TRUE?"ok":"error");
    
    /* disable */
    res = adxl345_set_sleep(&gs_handle, ADXL345_BOOL_FALSE);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set sleep failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: disable sleep.\n");
    res = adxl345_get_sleep(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get sleep failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check sleep %s.\n", bool_test==ADXL345_BOOL_FALSE?"ok":"error");
    
    /* adxl345_set_sleep_frequency/adxl345_get_sleep_frequency test */
    adxl345_interface_debug_print("adxl345: adxl345_set_sleep_frequency/adxl345_get_sleep_frequency test.\n");
    
    /* 8Hz */
    res = adxl345_set_sleep_frequency(&gs_handle, ADXL345_SLEEP_FREQUENCY_8HZ);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set sleep frequency failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set sleep frequency 8 Hz.\n");
    res = adxl345_get_sleep_frequency(&gs_handle, &sleep_frequency);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get sleep frequency failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check sleep frequency %s.\n", sleep_frequency==ADXL345_SLEEP_FREQUENCY_8HZ?"ok":"error");
    
    /* 4Hz */
    res = adxl345_set_sleep_frequency(&gs_handle, ADXL345_SLEEP_FREQUENCY_4HZ);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set sleep frequency failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set sleep frequency 4 Hz.\n");
    res = adxl345_get_sleep_frequency(&gs_handle, &sleep_frequency);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get sleep frequency failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check sleep frequency %s.\n", sleep_frequency==ADXL345_SLEEP_FREQUENCY_4HZ?"ok":"error");
    
    /* 2Hz */
    res = adxl345_set_sleep_frequency(&gs_handle, ADXL345_SLEEP_FREQUENCY_2HZ);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set sleep frequency failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set sleep frequency 2 Hz.\n");
    res = adxl345_get_sleep_frequency(&gs_handle, &sleep_frequency);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get sleep frequency failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check sleep frequency %s.\n", sleep_frequency==ADXL345_SLEEP_FREQUENCY_2HZ?"ok":"error");
    
    /* 1Hz */
    res = adxl345_set_sleep_frequency(&gs_handle, ADXL345_SLEEP_FREQUENCY_1HZ);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: set sleep frequency failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: set sleep frequency 1 Hz.\n");
    res = adxl345_get_sleep_frequency(&gs_handle, &sleep_frequency);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: get sleep frequency failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: check sleep frequency %s.\n", sleep_frequency==ADXL345_SLEEP_FREQUENCY_1HZ?"ok":"error");
    
    /* adxl345_tap_threshold_convert_to_register/adxl345_tap_threshold_convert_to_data test */
    adxl345_interface_debug_print("adxl345: adxl345_tap_threshold_convert_to_register/adxl345_tap_threshold_convert_to_data test.\n");
    f = (rand()%800)/100.0f;
    res = adxl345_tap_threshold_convert_to_register(&gs_handle, f, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: tap threshold convert to register failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_tap_threshold_convert_to_data(&gs_handle, reg, (float *)&f_check);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: tap threshold convert to data failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: tap threshold before is %.4f.\n", f);
    adxl345_interface_debug_print("adxl345: tap threshold register is 0x%02X.\n", reg);
    adxl345_interface_debug_print("adxl345: tap threshold convert is %.4f.\n", f_check);
    
    /* adxl345_offset_convert_to_register/adxl345_offset_convert_to_data test */
    adxl345_interface_debug_print("adxl345: adxl345_offset_convert_to_register/adxl345_offset_convert_to_data test.\n");
    f = (rand()%100)/100.0f;
    res = adxl345_offset_convert_to_register(&gs_handle, f, (int8_t *)&i_reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: offset convert to register failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_offset_convert_to_data(&gs_handle, i_reg, (float *)&f_check);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: offset convert to data failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: offset before is %.4f.\n", f);
    adxl345_interface_debug_print("adxl345: offset register is 0x%02X.\n", i_reg);
    adxl345_interface_debug_print("adxl345: offset convert is %.4f.\n", f_check);
    
    /* adxl345_duration_convert_to_register/adxl345_duration_convert_to_data test */
    adxl345_interface_debug_print("adxl345: adxl345_duration_convert_to_register/adxl345_duration_convert_to_data test.\n");
    us = (rand()%10)*625;
    res = adxl345_duration_convert_to_register(&gs_handle, us, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: duration convert to register failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }    
    res = adxl345_duration_convert_to_data(&gs_handle, reg, (uint32_t *)&us_check);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: duration convert to data failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: duration before is %d.\n", us);
    adxl345_interface_debug_print("adxl345: duration register is 0x%02X.\n", reg);
    adxl345_interface_debug_print("adxl345: duration convert is %d.\n", us_check);
    
    /* adxl345_latent_convert_to_register/adxl345_latent_convert_to_data test */
    adxl345_interface_debug_print("adxl345: adxl345_latent_convert_to_register/adxl345_latent_convert_to_data test.\n");
    f = (rand()%800)/10.0f;
    res = adxl345_latent_convert_to_register(&gs_handle, f, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: latent convert to register failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }    
    res = adxl345_latent_convert_to_data(&gs_handle, reg, (float *)&f_check);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: latent convert to data failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: latent before is %.4f.\n", f);
    adxl345_interface_debug_print("adxl345: latent register is 0x%02X.\n", reg);
    adxl345_interface_debug_print("adxl345: latent convert is %.4f.\n", f_check);
    
    /* adxl345_window_convert_to_register/adxl345_window_convert_to_data test */
    adxl345_interface_debug_print("adxl345: adxl345_window_convert_to_register/adxl345_window_convert_to_data test.\n");
    f = (rand()%800)/100.0f;
    res = adxl345_window_convert_to_register(&gs_handle, f, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: window convert to register failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }    
    res = adxl345_window_convert_to_data(&gs_handle, reg, (float *)&f_check);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: window convert to data failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: window before is %.4f.\n", f);
    adxl345_interface_debug_print("adxl345: window register is 0x%02X.\n", reg);
    adxl345_interface_debug_print("adxl345: window convert is %.4f.\n", f_check);
    
    /* adxl345_action_threshold_convert_to_register/adxl345_action_threshold_convert_to_data test */
    adxl345_interface_debug_print("adxl345: adxl345_action_threshold_convert_to_register/adxl345_action_threshold_convert_to_data test.\n");
    f = (rand()%800)/100.0f;
    res = adxl345_action_threshold_convert_to_register(&gs_handle, f, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: action threshold convert to register failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_action_threshold_convert_to_data(&gs_handle, reg, (float *)&f_check);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: action threshold convert to data failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: action threshold before is %.4f.\n", f);
    adxl345_interface_debug_print("adxl345: action threshold register is 0x%02X.\n", reg);
    adxl345_interface_debug_print("adxl345: action threshold convert is %.4f.\n", f_check);
    
    /* adxl345_inaction_threshold_convert_to_register/adxl345_inaction_threshold_convert_to_data test */
    adxl345_interface_debug_print("adxl345: adxl345_inaction_threshold_convert_to_register/adxl345_inaction_threshold_convert_to_data test.\n");
    f = (rand()%800)/100.0f;
    res = adxl345_inaction_threshold_convert_to_register(&gs_handle, f, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: inaction threshold convert to register failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_inaction_threshold_convert_to_data(&gs_handle, reg, (float *)&f_check);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: inaction threshold convert to data failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: inaction threshold before is %.4f.\n", f);
    adxl345_interface_debug_print("adxl345: inaction threshold register is 0x%02X.\n", reg);
    adxl345_interface_debug_print("adxl345: inaction threshold convert is %.4f.\n", f_check);
    
    /* adxl345_inaction_time_convert_to_register/adxl345_inaction_time_convert_to_data test */
    adxl345_interface_debug_print("adxl345: adxl345_inaction_time_convert_to_register/adxl345_inaction_time_convert_to_data test.\n");
    check = rand()%256;
    res = adxl345_inaction_time_convert_to_register(&gs_handle, check, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: inaction time convert to register failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_inaction_time_convert_to_data(&gs_handle, reg, (uint8_t *)&check);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: inaction time convert to data failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: inaction time before is 0x%02X.\n", check);
    adxl345_interface_debug_print("adxl345: inaction time register is 0x%02X.\n", reg);
    adxl345_interface_debug_print("adxl345: inaction time convert is 0x%02X.\n", check);
    
    /* adxl345_free_fall_threshold_convert_to_register/adxl345_free_fall_threshold_convert_to_data test */
    adxl345_interface_debug_print("adxl345: adxl345_free_fall_threshold_convert_to_register/adxl345_free_fall_threshold_convert_to_data test.\n");
    f = (rand()%800)/100.0f;
    res = adxl345_free_fall_threshold_convert_to_register(&gs_handle, f, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: free fall threshold convert to register failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_free_fall_threshold_convert_to_data(&gs_handle, reg, (float *)&f_check);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: free fall threshold convert to data failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: free fall threshold before is %.4f.\n", f);
    adxl345_interface_debug_print("adxl345: free fall threshold register is 0x%02X.\n", reg);
    adxl345_interface_debug_print("adxl345: free fall threshold convert is %.4f.\n", f_check);
    
    /* adxl345_free_fall_time_convert_to_register/adxl345_free_fall_time_convert_to_data test */
    adxl345_interface_debug_print("adxl345: adxl345_free_fall_time_convert_to_register/adxl345_free_fall_time_convert_to_data test.\n");
    ms = rand()%800;
    res = adxl345_free_fall_time_convert_to_register(&gs_handle, ms, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: free fall time convert to register failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl345_free_fall_time_convert_to_data(&gs_handle, reg, (uint16_t *)&ms_check);
    if (res != 0)
    {
        adxl345_interface_debug_print("adxl345: free fall time convert to data failed.\n");
        (void)adxl345_deinit(&gs_handle);
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: free time threshold before is %d.\n", ms);
    adxl345_interface_debug_print("adxl345: free time threshold register is 0x%02X.\n", reg);
    adxl345_interface_debug_print("adxl345: free time threshold convert is %d.\n", ms_check);
    
    /* finish register test */
    adxl345_interface_debug_print("adxl345: finish register test.\n");
    (void)adxl345_deinit(&gs_handle);
    
    return 0;
}
