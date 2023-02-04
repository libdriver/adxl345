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
 * @file      driver_adxl345_fifo.h
 * @brief     driver adxl345 fifo header file
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

#ifndef DRIVER_ADXL345_FIFO_H
#define DRIVER_ADXL345_FIFO_H

#include "driver_adxl345_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup adxl345_example_driver
 * @{
 */

/**
 * @brief adxl345 fifo example default definition
 */
#define ADXL345_FIFO_DEFAULT_RATE                        ADXL345_RATE_12P5                     /**< rate 12.5 Hz */
#define ADXL345_FIFO_DEFAULT_SPI_WIRE                    ADXL345_SPI_WIRE_4                    /**< spi wire 4 */
#define ADXL345_FIFO_DEFAULT_INTERRUPT_ACTIVE_LEVEL      ADXL345_INTERRUPT_ACTIVE_LEVEL_LOW    /**< interrupt pin low */
#define ADXL345_FIFO_DEFAULT_FULL_RESOLUTION             ADXL345_BOOL_TRUE                     /**< enable full resolution */
#define ADXL345_FIFO_DEFAULT_AUTO_SLEEP                  ADXL345_BOOL_FALSE                    /**< disable auto sleep */
#define ADXL345_FIFO_DEFAULT_SLEEP                       ADXL345_BOOL_FALSE                    /**< disable sleep */
#define ADXL345_FIFO_DEFAULT_SLEEP_FREQUENCY             ADXL345_SLEEP_FREQUENCY_1HZ           /**< sleep frequency 1Hz */
#define ADXL345_FIFO_DEFAULT_JUSTIFY                     ADXL345_JUSTIFY_RIGHT                 /**< justify right */
#define ADXL345_FIFO_DEFAULT_RANGE                       ADXL345_RANGE_16G                     /**< range 16g */
#define ADXL345_FIFO_DEFAULT_TRIGGER_PIN                 ADXL345_INTERRUPT_PIN2                /**< trigger pin map interrupt pin 2 */
#define ADXL345_FIFO_DEFAULT_TAP_SUPPRESS                ADXL345_BOOL_FALSE                    /**< disable tap suppress */
#define ADXL345_FIFO_DEFAULT_INTERRUPT_SINGLE_TAP_MAP    ADXL345_INTERRUPT_PIN1                /**< single tap map interrupt pin 1 */
#define ADXL345_FIFO_DEFAULT_INTERRUPT_DOUBLE_TAP_MAP    ADXL345_INTERRUPT_PIN1                /**< double tap map interrupt pin 1 */
#define ADXL345_FIFO_DEFAULT_INTERRUPT_ACTIVITY_MAP      ADXL345_INTERRUPT_PIN1                /**< activity map interrupt pin 1 */
#define ADXL345_FIFO_DEFAULT_INTERRUPT_INACTIVITY_MAP    ADXL345_INTERRUPT_PIN1                /**< inactivity map interrupt pin 1 */
#define ADXL345_FIFO_DEFAULT_INTERRUPT_FREE_FALL_MAP     ADXL345_INTERRUPT_PIN1                /**< free fall map interrupt pin 1 */
#define ADXL345_FIFO_DEFAULT_INTERRUPT_DATA_READY_MAP    ADXL345_INTERRUPT_PIN1                /**< data ready map interrupt pin 1 */
#define ADXL345_FIFO_DEFAULT_INTERRUPT_WATERMARK_MAP     ADXL345_INTERRUPT_PIN1                /**< watermark map interrupt pin 1 */
#define ADXL345_FIFO_DEFAULT_INTERRUPT_OVERRUN_MAP       ADXL345_INTERRUPT_PIN1                /**< overrun map interrupt pin 1 */
#define ADXL345_FIFO_DEFAULT_LINK_ACTIVITY_INACTIVITY    ADXL345_BOOL_TRUE                     /**< enable activity inactivity */
#define ADXL345_FIFO_DEFAULT_INTERRUPT_DATA_READY        ADXL345_BOOL_FALSE                    /**< disable data ready */
#define ADXL345_FIFO_DEFAULT_ACTION_COUPLED              ADXL345_COUPLED_AC                    /**< action ac coupled */
#define ADXL345_FIFO_DEFAULT_INACTION_COUPLED            ADXL345_COUPLED_DC                    /**< inaction dc coupled */
#define ADXL345_FIFO_DEFAULT_WATERMARK                   16                                    /**< watermark 16 level */
#define ADXL345_FIFO_DEFAULT_OFFSET                      0.0f                                  /**< 0 offset */
#define ADXL345_FIFO_DEFAULT_TAP_THRESHOLD               3.0f                                  /**< tap threshold 3.0g */
#define ADXL345_FIFO_DEFAULT_DURATION                    10 * 1000                             /**< duration 10 ms */
#define ADXL345_FIFO_DEFAULT_LATENT                      20.0f                                 /**< latent 20 ms */
#define ADXL345_FIFO_DEFAULT_WINDOW                      80.0f                                 /**< window 80 ms */
#define ADXL345_FIFO_DEFAULT_ACTION_THRESHOLD            2.0f                                  /**< action threshold 2g */
#define ADXL345_FIFO_DEFAULT_INACTION_THRESHOLD          1.0f                                  /**< inaction threshold 1g */
#define ADXL345_FIFO_DEFAULT_INACTION_TIME               3                                     /**< inaction 3s */
#define ADXL345_FIFO_DEFAULT_FREE_FALL_THRESHOLD         0.8f                                  /**< free fall threshold 0.8g */
#define ADXL345_FIFO_DEFAULT_FREE_FALL_TIME              10                                    /**< free fall time 10 ms */

/**
 * @brief  fifo irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t adxl345_fifo_irq_handler(void);

/**
 * @brief     fifo example init
 * @param[in] interface is the chip interface
 * @param[in] addr_pin is the iic device address
 * @param[in] *callback points to a irq callback address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t adxl345_fifo_init(adxl345_interface_t interface, adxl345_address_t addr_pin,
                          void (*callback)(float (*g)[3], uint16_t len));

/**
 * @brief  fifo example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t adxl345_fifo_deinit(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

