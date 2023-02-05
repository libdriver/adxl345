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
#include "mutex.h"
#include "getopt.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];                        /**< uart buffer */
volatile uint16_t g_len;                   /**< uart buffer length */
volatile uint8_t g_flag;                   /**< interrupt flag */
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
        /* run the callback in the mutex mode */
        mutex_irq(g_gpio_irq);
    }
}

/**
 * @brief     fifo callback
 * @param[in] **g points to a converted data buffer
 * @param[in] len is the data buffer
 * @note      none
 */
static void a_fifo_callback(float (*g)[3], uint16_t len)
{
    g_flag = 1;
}

/**
 * @brief     interrupt callback
 * @param[in] type is the irq type
 * @note      none
 */
static void a_interrupt_callback(uint8_t type)
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
}

/**
 * @brief     adxl345 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t adxl345(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"addr", required_argument, NULL, 1},
        {"interface", required_argument, NULL, 2},
        {"mask", required_argument, NULL, 3},
        {"times", required_argument, NULL, 4},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    uint32_t mask = 15;
    adxl345_address_t addr = ADXL345_ADDRESS_ALT_0;
    adxl345_interface_t interface = ADXL345_INTERFACE_IIC;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* addr */
            case 1 :
            {
                /* set the addr pin */
                if (strcmp("0", optarg) == 0)
                {
                    addr = ADXL345_ADDRESS_ALT_0;
                }
                else if (strcmp("1", optarg) == 0)
                {
                    addr = ADXL345_ADDRESS_ALT_1;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* interface */
            case 2 :
            {
                /* set the interface */
                if (strcmp("iic", optarg) == 0)
                {
                    interface = ADXL345_INTERFACE_IIC;
                }
                else if (strcmp("spi", optarg) == 0)
                {
                    interface = ADXL345_INTERFACE_SPI;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* mask */
            case 3 :
            {
                /* set the mask */
                mask = atol(optarg);
                
                break;
            }

            /* running times */
            case 4 :
            {
                /* set the times */
                times = atol(optarg);
                
                break;
            } 
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        uint8_t res;
        
        /* run register test */
        res = adxl345_register_test(interface, addr);
        if (res != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("t_read", type) == 0)
    {
        uint8_t res;
        
        /* read test */
        res = adxl345_read_test(interface, addr, times);
        if (res != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("t_fifo", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* set the gpio irq */
        g_gpio_irq = adxl345_fifo_test_irq_handler;
        
        /* run fifo test */
        res = adxl345_fifo_test(interface, addr);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("t_int", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* set the gpio irq */
        g_gpio_irq = adxl345_action_test_irq_handler;
        
        /* run interrupt test */
        res = adxl345_tap_action_fall_test(interface, addr);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("e_basic", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        float g[3];
        
        /* basic init */
        res = adxl345_basic_init(interface, addr);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* read data */
            res = adxl345_basic_read((float *)g);
            if (res != 0)
            {
                (void)adxl345_basic_deinit();
                
                return 1;
            }
            
            /* output */
            adxl345_interface_debug_print("adxl345: x is %0.3f.\n", g[0]);
            adxl345_interface_debug_print("adxl345: y is %0.3f.\n", g[1]);
            adxl345_interface_debug_print("adxl345: z is %0.3f.\n", g[2]);
            
            /* delay 1000ms */
            adxl345_interface_delay_ms(1000);
        }
        
        /* basic deinit */
        (void)adxl345_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_fifo", type) == 0)
    {
        uint8_t res;
        uint32_t timeout;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* set the gpio irq */
        g_gpio_irq = adxl345_fifo_irq_handler;
        
        /* set 0 */
        g_flag = 0;
        
        /* fifo init */
        res = adxl345_fifo_init(interface, addr, a_fifo_callback);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
         
        /* set timeout */
        timeout = 500;
        
        /* loop */
        while (times != 0)
        {
            /* check the flag */
            if (g_flag != 0)
            {
                adxl345_interface_debug_print("adxl345: fifo read %d.\n", times);
                g_flag = 0;
                timeout = 500;
                times--;
            }
            timeout--;
            /* check the timeout */
            if (timeout == 0)
            {
                (void)gpio_interrupt_deinit();
                (void)adxl345_fifo_deinit();
                g_gpio_irq = NULL;
                adxl345_interface_debug_print("adxl345: fifo read timeout.\n");
                
                return 1;
            }
            
            /* delay 10ms */
            adxl345_interface_delay_ms(10);
        }
        
        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        
        /* fifo deinit */
        (void)adxl345_fifo_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("e_int", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* set the interrupt irq */
        g_gpio_irq = adxl345_interrupt_irq_handler;
        
        /* interrupt init */
        res = adxl345_interrupt_init(interface, addr, a_interrupt_callback,
                                    (adxl345_bool_t)((mask >> 0) & 0x01), 
                                    (adxl345_bool_t)((mask >> 1) & 0x01),
                                    (adxl345_bool_t)((mask >> 2) & 0x01),
                                    (adxl345_bool_t)((mask >> 3) & 0x01)
                                        );
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* set the times */
        times = 500;
        while (times != 0)
        {
            /* mutex lock */
            mutex_lock();
            
            /* run the server */
            (void)adxl345_interrupt_server();
            times--;
            
            /* mutex unlock */
            mutex_unlock();
            
            /* delay 500ms */
            adxl345_interface_delay_ms(500);
        }
        adxl345_interface_debug_print("adxl345: finish interrupt.\n");
        
        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        
        /* interrupt deinit */
        (void)adxl345_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        adxl345_interface_debug_print("Usage:\n");
        adxl345_interface_debug_print("  adxl345 (-i | --information)\n");
        adxl345_interface_debug_print("  adxl345 (-h | --help)\n");
        adxl345_interface_debug_print("  adxl345 (-p | --port)\n");
        adxl345_interface_debug_print("  adxl345 (-t reg | --test=reg) [--addr=<0 | 1>] [--interface=<iic | spi>]\n");
        adxl345_interface_debug_print("  adxl345 (-t read | --test=read) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]\n");
        adxl345_interface_debug_print("  adxl345 (-t fifo | --test=fifo) [--addr=<0 | 1>] [--interface=<iic | spi>]\n");
        adxl345_interface_debug_print("  adxl345 (-t int | --test=int) [--addr=<0 | 1>] [--interface=<iic | spi>]\n");
        adxl345_interface_debug_print("  adxl345 (-e basic | --example=basic) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]\n");
        adxl345_interface_debug_print("  adxl345 (-e fifo | --example=fifo) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]\n");
        adxl345_interface_debug_print("  adxl345 (-e int | --example=int) [--addr=<0 | 1>] [--interface=<iic | spi>] [--mask=<msk>]\n");
        adxl345_interface_debug_print("\n");
        adxl345_interface_debug_print("Options:\n");
        adxl345_interface_debug_print("      --addr=<0 | 1>                 Set the chip address.([default: 0])\n");
        adxl345_interface_debug_print("  -e <basic | fifo | int>, --example=<basic | fifo | int>\n");
        adxl345_interface_debug_print("                                     Run the driver example.\n");
        adxl345_interface_debug_print("  -h, --help                         Show the help.\n");
        adxl345_interface_debug_print("  -i, --information                  Show the chip information.\n");
        adxl345_interface_debug_print("      --interface=<iic | spi>        Set the chip interface.([default: iic])\n");
        adxl345_interface_debug_print("      --mask=<msk>                   Set the interrupt mask, bit 0 is the tap enable mask,\n");
        adxl345_interface_debug_print("                                     bit 1 is the action enable mask, bit 2 is the inaction enable mask,\n");
        adxl345_interface_debug_print("                                     bit 3 is the free fall enable mask.([default: 15])\n");
        adxl345_interface_debug_print("  -p, --port                         Display the pin connections of the current board.\n");
        adxl345_interface_debug_print("  -t <reg | read | fifo | int>, --test=<reg | read | fifo | int>\n");
        adxl345_interface_debug_print("                                     Run the driver test.\n");
        adxl345_interface_debug_print("      --times=<num>                  Set the running times.([default: 3])\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        adxl345_info_t info;
        
        /* print adxl345 info */
        adxl345_info(&info);
        adxl345_interface_debug_print("adxl345: chip is %s.\n", info.chip_name);
        adxl345_interface_debug_print("adxl345: manufacturer is %s.\n", info.manufacturer_name);
        adxl345_interface_debug_print("adxl345: interface is %s.\n", info.interface);
        adxl345_interface_debug_print("adxl345: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        adxl345_interface_debug_print("adxl345: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        adxl345_interface_debug_print("adxl345: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        adxl345_interface_debug_print("adxl345: max current is %0.2fmA.\n", info.max_current_ma);
        adxl345_interface_debug_print("adxl345: max temperature is %0.1fC.\n", info.temperature_max);
        adxl345_interface_debug_print("adxl345: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
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
    uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart init */
    uart_init(115200);
    
    /* shell init && register adxl345 function */
    shell_init();
    shell_register("adxl345", adxl345);
    uart_print("adxl345: welcome to libdriver adxl345.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("adxl345: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("adxl345: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("adxl345: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("adxl345: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("adxl345: param is invalid.\n");
            }
            else
            {
                uart_print("adxl345: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
