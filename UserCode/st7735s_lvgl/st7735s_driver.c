/**
 * @file st7735s_driver.c
 * @author X. Y.
 * @brief
 * @version 0.1
 * @date 2022-09-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "st7735s_driver.h"
#include "cmsis_os.h"

// LCD define

// LCD Screen 1
LCD_t lcd1 = {
    .hspi          = &hspi2,
    .height        = 160,
    .width         = 128,
    .csGPIOPort    = GPIOE,
    .csGPIOPin     = GPIO_PIN_8,
    .dcGPIOPort    = GPIOB,
    .dcGPIOPin     = GPIO_PIN_1,
    .resetGPIOPort = GPIOC,
    .resetGPIOPin  = GPIO_PIN_5};

static inline void _SetCS(LCD_t *lcd, bool isEnable)
{
    HAL_GPIO_WritePin(lcd->csGPIOPort, lcd->csGPIOPin, !isEnable);
}

static inline void _SwitchToData(LCD_t *lcd, bool isData)
{
    HAL_GPIO_WritePin(lcd->dcGPIOPort, lcd->dcGPIOPin, isData);
}

static inline void _SetResetPin(LCD_t *lcd, bool isResetEnable)
{
    HAL_GPIO_WritePin(lcd->resetGPIOPort, lcd->resetGPIOPin, !isResetEnable);
}

static inline void _Delay(uint32_t millisec)
{
    osDelay(millisec);
}

static inline void _WriteData(LCD_t *lcd, uint8_t data)
{
    _SwitchToData(lcd, true);
    while (HAL_SPI_Transmit(lcd->hspi, &data, 1, HAL_MAX_DELAY) == HAL_BUSY)
        ;
}

static inline void _WriteDataBuffer(LCD_t *lcd, uint8_t *buffer, uint16_t size)
{
    _SwitchToData(lcd, true);
    while (HAL_SPI_Transmit_DMA(lcd->hspi, buffer, size) == HAL_BUSY)
        ;
}

/**
 * @brief Write command to LCD
 *
 * @param command
 */
static inline void _WriteCMD(LCD_t *lcd, uint8_t cmd)
{
    _SwitchToData(lcd, false);
    while (HAL_SPI_Transmit(lcd->hspi, &cmd, 1, HAL_MAX_DELAY) == HAL_BUSY)
        ;
}

//各种参数的宏定义
#define ST7735_MADCTL_MY  0x80
#define ST7735_MADCTL_MX  0x40
#define ST7735_MADCTL_MV  0x20
#define ST7735_MADCTL_ML  0x10
#define ST7735_MADCTL_RGB 0x00
#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MH  0x04

//参数命令宏定义
#define ST7735_NOP     0x00 //没有操作
#define ST7735_SWRESET 0x01 //软件重置
#define ST7735_RDDID   0x04 //读取身份信息
#define ST7735_RDDST   0x09 //读取状态

#define ST7735_SLPIN   0x10 //睡眠模式
#define ST7735_SLPOUT  0x11 //唤醒模式
#define ST7735_PTLON   0x12 //部分模式
#define ST7735_NORON   0x13 //正常模式

#define ST7735_INVOFF  0x20 //从反演模式恢复（见datasheet p94）
#define ST7735_INVON   0x21 //进入反演模式（见datasheet p94）
#define ST7735_DISPOFF 0x28 //关闭显示
#define ST7735_DISPON  0x29 //从关闭显示模式恢复
#define ST7735_CASET   0x2A //设定列地址
#define ST7735_RASET   0x2B //设定行地址
#define ST7735_RAMWR   0x2C //写memory
#define ST7735_RAMRD   0x2E //读memory

#define ST7735_PTLAR   0x30 //定义部分显示部分
#define ST7735_COLMOD  0x3A //确定颜色数据格式
#define ST7735_MADCTL  0x36 //读写内存扫描方向

#define ST7735_FRMCTR1 0xB1 //设定设备帧率
#define ST7735_FRMCTR2 0xB2 //设定MCU帧率
#define ST7735_FRMCTR3 0xB3 // Set the frame frequency of the Partial mode/ full colors.
#define ST7735_INVCTR  0xB4 // Display Inversion mode control
#define ST7735_DISSET5 0xB6 //设备显示设置

#define ST7735_PWCTR1  0xC0 // Set the GVDD voltage
#define ST7735_PWCTR2  0xC1 // Set the VGH and VGL supply power level
#define ST7735_PWCTR3  0xC2 //设定运算放大器中的电流量
#define ST7735_PWCTR4  0xC3 //设定运算放大器中的电流量
#define ST7735_PWCTR5  0xC4 //设定运算放大器中的电流量
#define ST7735_VMCTR1  0xC5 // Set VCOMH Voltage

#define ST7735_RDID1   0xDA // returns 8-bit LCD module’s manufacturer ID
#define ST7735_RDID2   0xDB // returns 8-bit LCD module/driver version ID
#define ST7735_RDID3   0xDC // returns 8-bit LCD module/driver ID
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC //设定运算放大器中的电流量
#define ST7735_GMCTRP1 0xE0 //设定运算放大器中的电流量
#define ST7735_GMCTRN1 0xE1 //设定运算放大器中的电流量

static inline void _SetZone(LCD_t *lcd, uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY)
{
    _WriteCMD(lcd, ST7735_CASET);

    _WriteData(lcd, startX >> 8);
    _WriteData(lcd, startX);
    _WriteData(lcd, endX >> 8);
    _WriteData(lcd, endX);

    _WriteCMD(lcd, ST7735_RASET);

    _WriteData(lcd, startY >> 8);
    _WriteData(lcd, startY);
    _WriteData(lcd, endY >> 8);
    _WriteData(lcd, endY);
}

void LCD_Reset(LCD_t *lcd)
{
    _SetCS(lcd, true);

    _SetResetPin(lcd, false);
    _Delay(5);
    _SetResetPin(lcd, true);
    _Delay(5);
    _SetResetPin(lcd, false);
    _Delay(20);
    _WriteCMD(lcd, ST7735_SWRESET); // Software Reset
    _Delay(20);
    _WriteCMD(lcd, ST7735_SLPOUT); // Sleep out

    _SetCS(lcd, false);
    _Delay(50);
}

void LCD_Init(LCD_t *lcd)
{
    LCD_Reset(lcd);
    //--------------------------------------ST7735S Frame Rate----------------------------------------------//
    _SetCS(lcd, true);

    _WriteCMD(lcd, ST7735_FRMCTR1); // Frame rate 80Hz Frame rate=333k/((RTNA + 20) x (LINE + FPA + BPA))
    _WriteData(lcd, 0x02);          // RTNA
    _WriteData(lcd, 0x35);          // FPA
    _WriteData(lcd, 0x36);          // BPA
    _WriteCMD(lcd, ST7735_FRMCTR2); // Frame rate 80Hz
    _WriteData(lcd, 0x02);
    _WriteData(lcd, 0x35);
    _WriteData(lcd, 0x36);
    _WriteCMD(lcd, ST7735_FRMCTR3); // Frame rate 80Hz
    _WriteData(lcd, 0x02);
    _WriteData(lcd, 0x35);
    _WriteData(lcd, 0x36);
    _WriteData(lcd, 0x02);
    _WriteData(lcd, 0x35);
    _WriteData(lcd, 0x36);

    //------------------------------------Display Inversion Control-----------------------------------------//
    _WriteCMD(lcd, ST7735_INVCTR);
    _WriteData(lcd, 0x03);

    //---------------------------------End ST7735S Power Sequence---------------------------------------//
    _WriteCMD(lcd, ST7735_VMCTR1); // VCOM
    _WriteData(lcd, 0x0a);
    _WriteCMD(lcd, ST7735_MADCTL); // MX, MY, RGB mode
    _WriteData(lcd, 0xC0);

    //------------------------------------ST7735S Gamma Sequence-----------------------------------------//
    //电源参数：定值
    _WriteCMD(lcd, ST7735_GMCTRP1);
    _WriteData(lcd, 0x12);
    _WriteData(lcd, 0x1C);
    _WriteData(lcd, 0x10);
    _WriteData(lcd, 0x18);
    _WriteData(lcd, 0x33);
    _WriteData(lcd, 0x2C);
    _WriteData(lcd, 0x25);
    _WriteData(lcd, 0x28);
    _WriteData(lcd, 0x28);
    _WriteData(lcd, 0x27);
    _WriteData(lcd, 0x2F);
    _WriteData(lcd, 0x3C);
    _WriteData(lcd, 0x00);
    _WriteData(lcd, 0x03);
    _WriteData(lcd, 0x03);
    _WriteData(lcd, 0x10);
    _WriteCMD(lcd, ST7735_GMCTRN1);
    _WriteData(lcd, 0x12);
    _WriteData(lcd, 0x1C);
    _WriteData(lcd, 0x10);
    _WriteData(lcd, 0x18);
    _WriteData(lcd, 0x2D);
    _WriteData(lcd, 0x28);
    _WriteData(lcd, 0x23);
    _WriteData(lcd, 0x28);
    _WriteData(lcd, 0x28);
    _WriteData(lcd, 0x26);
    _WriteData(lcd, 0x2F);
    _WriteData(lcd, 0x3B);
    _WriteData(lcd, 0x00);
    _WriteData(lcd, 0x03);
    _WriteData(lcd, 0x03);
    _WriteData(lcd, 0x10);
    //------------------------------------End ST7735S Gamma Sequence-----------------------------------------//
    _WriteCMD(lcd, ST7735_COLMOD); // 16-bit pixel
    _WriteData(lcd, 0x05);
    _WriteCMD(lcd, ST7735_DISPON); // Display on

    _SetCS(lcd, false);
    _Delay(50);
}

void LCD_Flush(LCD_t *lcd, uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY, uint8_t *data)
{
    _SetCS(lcd, true);

    _SetZone(lcd, startX, startY, endX, endY);
    _WriteCMD(lcd, ST7735_RAMWR);
    _WriteDataBuffer(lcd, data, 2 * (endX - startX + 1) * (endY - startY + 1));
}

void LCD_SPI_TxCpltCallback(LCD_t *lcd)
{
    _SetCS(lcd, false);
}
