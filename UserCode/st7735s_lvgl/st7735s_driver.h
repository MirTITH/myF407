#ifndef __ST7735_DRIVER_H__
#define __ST7735_DRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "spi.h"
#include "gpio.h"

#define DISPLAY_H 128
#define DISPLAY_W 160

#define ST7735_RES_GPIO_Port GPIOC
#define ST7735_RES_Pin GPIO_PIN_5

#define ST7735_DC_GPIO_Port GPIOB
#define ST7735_DC_Pin GPIO_PIN_1

#define ST7735_CS_GPIO_Port GPIOE
#define ST7735_CS_Pin GPIO_PIN_8

//宏定义基本引脚功能
#define LCD_RESET_ENABLE() HAL_GPIO_WritePin(ST7735_RES_GPIO_Port, ST7735_RES_Pin, GPIO_PIN_RESET )
#define LCD_RESET_DISABLE() HAL_GPIO_WritePin(ST7735_RES_GPIO_Port, ST7735_RES_Pin, GPIO_PIN_SET )

#define LCD_DC_DATA() HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET )
#define LCD_DC_COMMMAND() HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_RESET )

#define LCD_CS_ENABLE()  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET )
#define LCD_CS_DISABLE() HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET )

//各种参数的宏定义
#define ST7735_MADCTL_MY  0x80
#define ST7735_MADCTL_MX  0x40
#define ST7735_MADCTL_MV  0x20
#define ST7735_MADCTL_ML  0x10
#define ST7735_MADCTL_RGB 0x00
#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MH  0x04

//参数命令宏定义
#define ST7735_NOP     0x00//没有操作
#define ST7735_SWRESET 0x01//软件重置
#define ST7735_RDDID   0x04//读取身份信息
#define ST7735_RDDST   0x09//读取状态

#define ST7735_SLPIN   0x10//睡眠模式
#define ST7735_SLPOUT  0x11//唤醒模式
#define ST7735_PTLON   0x12//部分模式
#define ST7735_NORON   0x13//正常模式

#define ST7735_INVOFF  0x20//从反演模式恢复（见datasheet p94）
#define ST7735_INVON   0x21//进入反演模式（见datasheet p94）
#define ST7735_DISPOFF 0x28//关闭显示
#define ST7735_DISPON  0x29//从关闭显示模式恢复
#define ST7735_CASET   0x2A//设定列地址
#define ST7735_RASET   0x2B//设定行地址
#define ST7735_RAMWR   0x2C//写memory
#define ST7735_RAMRD   0x2E//读memory

#define ST7735_PTLAR   0x30//定义部分显示部分
#define ST7735_COLMOD  0x3A//确定颜色数据格式
#define ST7735_MADCTL  0x36//读写内存扫描方向

#define ST7735_FRMCTR1 0xB1//设定设备帧率
#define ST7735_FRMCTR2 0xB2//设定MCU帧率
#define ST7735_FRMCTR3 0xB3//Set the frame frequency of the Partial mode/ full colors.
#define ST7735_INVCTR  0xB4//Display Inversion mode control
#define ST7735_DISSET5 0xB6//设备显示设置

#define ST7735_PWCTR1  0xC0//Set the GVDD voltage
#define ST7735_PWCTR2  0xC1//Set the VGH and VGL supply power level
#define ST7735_PWCTR3  0xC2//设定运算放大器中的电流量
#define ST7735_PWCTR4  0xC3//设定运算放大器中的电流量
#define ST7735_PWCTR5  0xC4//设定运算放大器中的电流量
#define ST7735_VMCTR1  0xC5//Set VCOMH Voltage

#define ST7735_RDID1   0xDA//returns 8-bit LCD module’s manufacturer ID
#define ST7735_RDID2   0xDB//returns 8-bit LCD module/driver version ID
#define ST7735_RDID3   0xDC//returns 8-bit LCD module/driver ID
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC//设定运算放大器中的电流量
#define ST7735_GMCTRP1 0xE0//设定运算放大器中的电流量
#define ST7735_GMCTRN1 0xE1//设定运算放大器中的电流量

// Color definitions
#define	ST7735_BLACK   0x0000
#define	ST7735_BLUE    0x001F
#define	ST7735_RED     0xF800
#define	ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0
#define ST7735_WHITE   0xFFFF
#define ST7735_COLOR565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

//ST7735_driver库函数
void LCD_Init();
void LCD_fillScreen(uint16_t color);
uint16_t RGB565( uint8_t r, uint8_t g, uint8_t b );
void LCD_DrawRegion( int xS, int yS, int xE, int yE, uint16_t color);
void LCD_showimage(const unsigned char *p);
void LCD_showletter(int xS,int yS,const unsigned char *p);

void LCD_Write_COMMAND(uint8_t address);
void LCD_Write_Data8(uint8_t data);
void LCD_Write_Data16(uint16_t data);
void LCD_Zone_Set(uint8_t xStart, uint8_t yStart, uint8_t xEnd, uint8_t yEnd);

void drawNumber_1(int num, int xStart, int yStart);
void drawNumber_2(int num, int xStart, int yStart);
void drawNumber_3(int num, int xStart, int yStart);
void drawNumber_4(int num, int xStart, int yStart);
void drawNumber_5(int num, int xStart, int yStart);
void drawNumber_6(int num, int xStart, int yStart);
void drawNumber(int num, int xStart, int yStart);
void lcd_init();

#ifdef __cplusplus
}
#endif

#endif //__ST7735_DRIVER_H__
