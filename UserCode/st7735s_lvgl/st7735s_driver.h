/**
 * @file st7735s_driver.h
 * @author X. Y.
 * @brief
 * @version 0.1
 * @date 2022-09-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "spi.h"
#include <stdbool.h>

typedef struct
{
    SPI_HandleTypeDef *hspi;     // SPI handle of the LCD
    uint16_t height;             // height of the LCD
    uint16_t width;              // width of the LCD
    GPIO_TypeDef *resetGPIOPort; // reset GPIO port
    uint16_t resetGPIOPin;       // reset GPIO pin
    GPIO_TypeDef *dcGPIOPort;    // data or command GPIO port
    uint16_t dcGPIOPin;          // data or command GPIO pin
    GPIO_TypeDef *csGPIOPort;    // chip select GPIO port
    uint16_t csGPIOPin;          // chip select GPIO pin
} LCD_t;

extern LCD_t lcd1;

void LCD_Init(LCD_t *lcd);

void LCD_Reset(LCD_t *lcd);

void LCD_SPI_TxCpltCallback(LCD_t *lcd);

void LCD_Flush(LCD_t *lcd, uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY, uint8_t *data);

#ifdef __cplusplus
}
#endif
