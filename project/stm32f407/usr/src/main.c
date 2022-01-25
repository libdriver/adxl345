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
 * @file      main.c
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-04-20
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/04/20  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_adxl345_tap_action_fall_test.h"
#include "driver_adxl345_fifo_test.h"
#include "driver_adxl345_read_test.h"
#include "driver_adxl345_register_test.h"
#include "driver_adxl345_interrupt.h"
#include "driver_adxl345_fifo.h"
#include "driver_adxl345_basic.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "gpio.h"
#include "uart.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];                        /**< uart buffer */
uint16_t g_len;                            /**< uart buffer length */
uint8_t g_flag;                            /**< interrupt flag */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq */

/**
 * @brief exti 0 irq
 * @note  none
 */
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
 * @brief     gpio exti callback
 * @param[in] pin is the gpio pin
 * @note      none
 */
void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    if (pin == GPIO_PIN_0)
    {
        if (g_gpio_irq)
        {
            g_gpio_irq();
        }
    }
}

/**
 * @brief     fifo callback
 * @param[in] **g points to a converted data buffer
 * @param[in] len is the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 * @note      none
 */
static uint8_t _fifo_callback(float (*g)[3], uint16_t len)
{
    g_flag = 1;
    
    return 0;
}

/**
 * @brief     interrupt callback
 * @param[in] type is the irq type
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 * @note      none
 */
static uint8_t _interrupt_callback(uint8_t type)
{
    switch (type)
    {
        case ADXL345_INTERRUPT_DATA_READY :
        {
            break;
        }
        case ADXL345_INTERRUPT_SINGLE_TAP :
        {
            adxl345_interface_debug_print("adxl345: irq single tap.\n");
            
            break;
        }
        case ADXL345_INTERRUPT_DOUBLE_TAP :
        {
            adxl345_interface_debug_print("adxl345: irq double tap.\n");
            
            break;
        }
        case ADXL345_INTERRUPT_ACTIVITY :
        {
            adxl345_interface_debug_print("adxl345: irq activity.\n");
            
            break;
        }
        case ADXL345_INTERRUPT_INACTIVITY :
        {
            adxl345_interface_debug_print("adxl345: irq inactivity.\n");
            
            break;
        }
        case ADXL345_INTERRUPT_FREE_FALL :
        {
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

    return 0;
}

/**
 * @brief     adxl345 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t adxl345(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
        goto help;
    }
    else if (argc == 2)
    {
        if (strcmp("-i", argv[1]) == 0)
        {
            adxl345_info_t info;
            
            /* print adxl345 info */
            
            adxl345_info(&info);
            adxl345_interface_debug_print("adxl345: chip is %s.\n", info.chip_name);
            adxl345_interface_debug_print("adxl345: manufacturer is %s.\n", info.manufacturer_name);
            adxl345_interface_debug_print("adxl345: interface is %s.\n", info.interface);
            adxl345_interface_debug_print("adxl345: driver version is %d.%d.\n", info.driver_version/1000, (info.driver_version%1000)/100);
            adxl345_interface_debug_print("adxl345: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
            adxl345_interface_debug_print("adxl345: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
            adxl345_interface_debug_print("adxl345: max current is %0.2fmA.\n", info.max_current_ma);
            adxl345_interface_debug_print("adxl345: max temperature is %0.1fC.\n", info.temperature_max);
            adxl345_interface_debug_print("adxl345: min temperature is %0.1fC.\n", info.temperature_min);
            
            return 0;
        }
        else if (strcmp("-p", argv[1]) == 0)
        {
            /* print pin connection */
            
            adxl345_interface_debug_print("adxl345: SPI interface SCK connected to GPIOA PIN5.\n");
            adxl345_interface_debug_print("adxl345: SPI interface MISO connected to GPIOA PIN6.\n");
            adxl345_interface_debug_print("adxl345: SPI interface MOSI connected to GPIOA PIN7.\n");
            adxl345_interface_debug_print("adxl345: SPI interface CS connected to GPIOA PIN4.\n");
            adxl345_interface_debug_print("adxl345: IIC interface SCL connected to GPIOB PIN8.\n");
            adxl345_interface_debug_print("adxl345: IIC interface SDA connected to GPIOB PIN9.\n");
            adxl345_interface_debug_print("adxl345: INT connected to GPIOB PIN0.\n");
            
            return 0;
        }
        else if (strcmp("-h", argv[1]) == 0)
        {
            /* show adxl345 help */
            
            help:
            
            adxl345_interface_debug_print("adxl345 -i\n\tshow adxl345 chip and driver information.\n");
            adxl345_interface_debug_print("adxl345 -h\n\tshow adxl345 help.\n");
            adxl345_interface_debug_print("adxl345 -p\n\tshow adxl345 pin connections of the current board.\n");
            adxl345_interface_debug_print("adxl345 -t reg (-iic (0 | 1) | -spi)\n\trun adxl345 register test.\n");
            adxl345_interface_debug_print("adxl345 -t read <times> (-iic (0 | 1) | -spi)\n\trun adxl345 read test.times means the test times.\n");
            adxl345_interface_debug_print("adxl345 -t fifo (-iic (0 | 1) | -spi)\n\trun adxl345 fifo test.\n");
            adxl345_interface_debug_print("adxl345 -t int (-iic (0 | 1) | -spi)\n\trun adxl345 interrupt test.\n");
            adxl345_interface_debug_print("adxl345 -c basic <times> (-iic (0 | 1) | -spi)\n\trun adxl345 basic function.times is the read times.\n");
            adxl345_interface_debug_print("adxl345 -c fifo <times> (-iic (0 | 1) | -spi)\n\trun adxl345 fifo function.times is the read times.\n");
            adxl345_interface_debug_print("adxl345 -c int (-iic (0 | 1) | -spi) <mask>\n\trun adxl345 interrupt function.mask is the interrupt mask."
                                          "bit 0 is the tap enable mask.bit 1 is the action enable mask.");
            adxl345_interface_debug_print("bit 2 is the inaction enable mask.bit 3 is the free fall enable mask.\n");
            
            return 0;
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 4)
    {
        if (strcmp("-t", argv[1]) == 0)
        {
            if (strcmp("reg", argv[2]) == 0)
            {
                if (strcmp("-spi", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    
                    /* run register test */
                    res = adxl345_register_test(ADXL345_INTERFACE_SPI, ADXL345_ADDRESS_ALT_0);
                    if (res)
                    {
                        return 1;
                    }
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("fifo", argv[2]) == 0)
            {
                if (strcmp("-spi", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    
                    res = gpio_interrupt_init();
                    if (res)
                    {
                        return 1;
                    }
                    g_gpio_irq = adxl345_fifo_test_irq_handler;
                    
                    /* run fifo test */
                    res = adxl345_fifo_test(ADXL345_INTERFACE_SPI, ADXL345_ADDRESS_ALT_0);
                    if (res)
                    {
                        gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("int", argv[2]) == 0)
            {
                if (strcmp("-spi", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    
                    res = gpio_interrupt_init();
                    if (res)
                    {
                        return 1;
                    }
                    g_gpio_irq = adxl345_action_test_irq_handler;
                    
                    /* run interrupt test */
                    res = adxl345_tap_action_fall_test(ADXL345_INTERFACE_SPI, ADXL345_ADDRESS_ALT_0);
                    if (res)
                    {
                        gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 5)
    {
        if (strcmp("-t", argv[1]) == 0)
        {
            if (strcmp("reg", argv[2]) == 0)
            {
                if (strcmp("-iic", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    adxl345_address_t addr_pin;
                    
                    if (strcmp("0", argv[4]) == 0)
                    {
                        addr_pin = ADXL345_ADDRESS_ALT_0;
                    }
                    else if (strcmp("1", argv[4]) == 0)
                    {
                        addr_pin = ADXL345_ADDRESS_ALT_1;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    /* run register test */
                    res = adxl345_register_test(ADXL345_INTERFACE_IIC, addr_pin);
                    if (res)
                    {
                        return 1;
                    }
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("fifo", argv[2]) == 0)
            {
                if (strcmp("-iic", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    adxl345_address_t addr_pin;
                    
                    if (strcmp("0", argv[4]) == 0)
                    {
                        addr_pin = ADXL345_ADDRESS_ALT_0;
                    }
                    else if (strcmp("1", argv[4]) == 0)
                    {
                        addr_pin = ADXL345_ADDRESS_ALT_1;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    res = gpio_interrupt_init();
                    if (res)
                    {
                        return 1;
                    }
                    g_gpio_irq = adxl345_fifo_test_irq_handler;
                    
                    /* fifo test */
                    res = adxl345_fifo_test(ADXL345_INTERFACE_IIC, addr_pin);
                    if (res)
                    {
                        gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("int", argv[2]) == 0)
            {
                if (strcmp("-iic", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    adxl345_address_t addr_pin;
                    
                    if (strcmp("0", argv[4]) == 0)
                    {
                        addr_pin = ADXL345_ADDRESS_ALT_0;
                    }
                    else if (strcmp("1", argv[4]) == 0)
                    {
                        addr_pin = ADXL345_ADDRESS_ALT_1;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    res = gpio_interrupt_init();
                    if (res)
                    {
                        return 1;
                    }
                    g_gpio_irq = adxl345_action_test_irq_handler;
                    
                    /* interrupt test */
                    res = adxl345_tap_action_fall_test(ADXL345_INTERFACE_IIC, addr_pin);
                    if (res)
                    {
                        gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("read", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint32_t times;
                
                times = atoi(argv[3]);
                
                if (strcmp("-spi", argv[4]) == 0)
                {
                    
                    /* read test */
                    res = adxl345_read_test(ADXL345_INTERFACE_SPI, ADXL345_ADDRESS_ALT_0, times);
                    if (res)
                    {
                        return 1;
                    }
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("basic", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint32_t i, times;
                
                times = atoi(argv[3]);
                
                if (strcmp("-spi", argv[4]) == 0)
                {
                    volatile float g[3];
                    
                    /* basic read */
                    res = adxl345_basic_init(ADXL345_INTERFACE_SPI, ADXL345_ADDRESS_ALT_0);
                    if (res)
                    {
                        return 1;
                    }
                    for (i = 0; i < times; i++)
                    {
                        res = adxl345_basic_read((float *)g);
                        if (res)
                        {
                            adxl345_basic_deinit();
                            return 1;
                        }
                        adxl345_interface_debug_print("adxl345: x is %0.3f.\n", g[0]);
                        adxl345_interface_debug_print("adxl345: y is %0.3f.\n", g[1]);
                        adxl345_interface_debug_print("adxl345: z is %0.3f.\n", g[2]);
                        
                        adxl345_interface_delay_ms(1000);
                    }
                    
                    adxl345_basic_deinit();
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("fifo", argv[2]) == 0)
            {
                volatile uint32_t i, times, timeout;
                
                times = atoi(argv[3]);
                
                if (strcmp("-spi", argv[4]) == 0)
                {
                    volatile uint8_t res;
                    volatile float g[3];
                    
                    res = gpio_interrupt_init();
                    if (res)
                    {
                        return 1;
                    }
                    g_gpio_irq = adxl345_fifo_irq_handler;
                    g_flag = 0;
                    res = adxl345_fifo_init(ADXL345_INTERFACE_SPI, ADXL345_ADDRESS_ALT_0, _fifo_callback);
                    if (res)
                    {
                        gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    timeout = 500;
                    while (times)
                    {
                        if (g_flag)
                        {
                            adxl345_interface_debug_print("adxl345: fifo read %d.\n", times);
                            g_flag = 0;
                            timeout = 500;
                            times--;
                        }
                        timeout--;
                        if (timeout == 0)
                        {
                            gpio_interrupt_deinit();
                            adxl345_fifo_deinit();
                            g_gpio_irq = NULL;
                            adxl345_interface_debug_print("adxl345: fifo read timeout.\n");
                            
                            return 1;
                        }
                        adxl345_interface_delay_ms(10);
                    }
                    
                    gpio_interrupt_deinit();
                    adxl345_fifo_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("int", argv[2]) == 0)
            {
                volatile uint32_t flag;
                
                flag = atoi(argv[4]);
                
                if (strcmp("-spi", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    volatile uint32_t times;
                    volatile float g[3];
                    
                    res = gpio_interrupt_init();
                    if (res)
                    {
                        return 1;
                    }
                    g_gpio_irq = adxl345_interrupt_irq_handler;
                    res = adxl345_interrupt_init(ADXL345_INTERFACE_SPI, ADXL345_ADDRESS_ALT_0, _interrupt_callback,
                                                (adxl345_bool_t)((flag >> 0) & 0x01), 
                                                (adxl345_bool_t)((flag >> 1) & 0x01),
                                                (adxl345_bool_t)((flag >> 2) & 0x01),
                                                (adxl345_bool_t)((flag >> 3) & 0x01)
                                                 );
                    if (res)
                    {
                        gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                     
                    times = 500;
                    while (times)
                    {
                        adxl345_interrupt_server();
                        times--;
                        adxl345_interface_delay_ms(10);
                    }
                    adxl345_interface_debug_print("adxl345: finish interrupt.\n");
                    gpio_interrupt_deinit();
                    adxl345_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 6)
    {
        if (strcmp("-t", argv[1]) == 0)
        {
            if (strcmp("read", argv[2]) == 0)
            {
                volatile uint32_t times;
                
                times = atoi(argv[3]);
                
                if (strcmp("-iic", argv[4]) == 0)
                {
                    volatile uint8_t res;
                    adxl345_address_t addr_pin;
                    
                    if (strcmp("0", argv[5]) == 0)
                    {
                        addr_pin = ADXL345_ADDRESS_ALT_0;
                    }
                    else if (strcmp("1", argv[5]) == 0)
                    {
                        addr_pin = ADXL345_ADDRESS_ALT_1;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    /* read test */
                    res = adxl345_read_test(ADXL345_INTERFACE_IIC, addr_pin, times);
                    if (res)
                    {
                        return 1;
                    }
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("basic", argv[2]) == 0)
            {
                volatile uint32_t i, times;
                
                times = atoi(argv[3]);
                
                if (strcmp("-iic", argv[4]) == 0)
                {
                    volatile uint8_t res;
                    volatile float g[3];
                    adxl345_address_t addr_pin;
                    
                    if (strcmp("0", argv[5]) == 0)
                    {
                        addr_pin = ADXL345_ADDRESS_ALT_0;
                    }
                    else if (strcmp("1", argv[5]) == 0)
                    {
                        addr_pin = ADXL345_ADDRESS_ALT_1;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    res = adxl345_basic_init(ADXL345_INTERFACE_IIC, addr_pin);
                    if (res)
                    {
                        return 1;
                    }
                    for (i = 0; i < times; i++)
                    {
                        res = adxl345_basic_read((float *)g);
                        if (res)
                        {
                            adxl345_basic_deinit();
                            return 1;
                        }
                        adxl345_interface_debug_print("adxl345: x is %0.3f.\n", g[0]);
                        adxl345_interface_debug_print("adxl345: y is %0.3f.\n", g[1]);
                        adxl345_interface_debug_print("adxl345: z is %0.3f.\n", g[2]);
                        
                        adxl345_interface_delay_ms(1000);
                    }
                    
                    adxl345_basic_deinit();
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("fifo", argv[2]) == 0)
            {
                volatile uint32_t i, times, timeout;
                
                times = atoi(argv[3]);
                
                if (strcmp("-iic", argv[4]) == 0)
                {
                    volatile uint8_t res;
                    volatile float g[3];
                    adxl345_address_t addr_pin;
                    
                    if (strcmp("0", argv[5]) == 0)
                    {
                        addr_pin = ADXL345_ADDRESS_ALT_0;
                    }
                    else if (strcmp("1", argv[5]) == 0)
                    {
                        addr_pin = ADXL345_ADDRESS_ALT_1;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    res = gpio_interrupt_init();
                    if (res)
                    {
                        return 1;
                    }
                    g_gpio_irq = adxl345_fifo_irq_handler;
                    g_flag = 0;
                    res = adxl345_fifo_init(ADXL345_INTERFACE_IIC, addr_pin, _fifo_callback);
                    if (res)
                    {
                        gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    timeout = 500;
                    while (times)
                    {
                        if (g_flag)
                        {
                            adxl345_interface_debug_print("adxl345: fifo read %d.\n", times);
                            g_flag = 0;
                            timeout = 500;
                            times--;
                        }
                        timeout--;
                        if (timeout == 0)
                        {
                            gpio_interrupt_deinit();
                            adxl345_fifo_deinit();
                            g_gpio_irq = NULL;
                            adxl345_interface_debug_print("adxl345: fifo read timeout.\n");
                            
                            return 1;
                        }
                        adxl345_interface_delay_ms(10);
                    }
                    
                    gpio_interrupt_deinit();
                    adxl345_fifo_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("int", argv[2]) == 0)
            {
                volatile uint32_t flag;
                volatile uint32_t times;
                
                flag = atoi(argv[5]);
                
                if (strcmp("-iic", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    volatile float g[3];
                    adxl345_address_t addr_pin;
                    
                    if (strcmp("0", argv[4]) == 0)
                    {
                        addr_pin = ADXL345_ADDRESS_ALT_0;
                    }
                    else if (strcmp("1", argv[4]) == 0)
                    {
                        addr_pin = ADXL345_ADDRESS_ALT_1;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    res = gpio_interrupt_init();
                    if (res)
                    {
                        return 1;
                    }
                    g_gpio_irq = adxl345_interrupt_irq_handler;
                    res = adxl345_interrupt_init(ADXL345_INTERFACE_IIC, addr_pin, _interrupt_callback,
                                                (adxl345_bool_t)((flag >> 0) & 0x01), 
                                                (adxl345_bool_t)((flag >> 1) & 0x01),
                                                (adxl345_bool_t)((flag >> 2) & 0x01),
                                                (adxl345_bool_t)((flag >> 3) & 0x01)
                                                 );
                    if (res)
                    {
                        gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    
                    times = 500;
                    while (times)
                    {
                        adxl345_interrupt_server();
                        times--;
                        adxl345_interface_delay_ms(10);
                    }
                    adxl345_interface_debug_print("adxl345: finish interrupt.\n");
                    gpio_interrupt_deinit();
                    adxl345_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    /* param is invalid */
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    volatile uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart1 init */
    uart1_init(115200);
    
    /* shell init && register adxl345 fuction */
    shell_init();
    shell_register("adxl345", adxl345);
    uart1_print("adxl345: welcome to libdriver adxl345.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart1_read(g_buf, 256);
        if (g_len)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart1_print("adxl345: run failed.\n");
            }
            else if (res == 2)
            {
                uart1_print("adxl345: unknow command.\n");
            }
            else if (res == 3)
            {
                uart1_print("adxl345: length is too long.\n");
            }
            else if (res == 4)
            {
                uart1_print("adxl345: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart1_print("adxl345: param is invalid.\n");
            }
            else
            {
                uart1_print("adxl345: unknow status code.\n");
            }
            uart1_flush();
        }
        delay_ms(100);
    }
}
