#include "st7735s_driver.h"
#include "main.h"
#include "gpio.h"
#include "spi.h"
#include "LetterGraph.h"

//我感觉下面这个用不到，所以注释掉了
/*typedef struct 
{
    SPI_HandleTypeDef *hspi;
}LCD_t;

LCD_t LCDParam;*/

//向SPI接口写东西
void LCD_Write_Bus(uint8_t data){
    
    if(HAL_SPI_Transmit(&hspi2,&data,1,0xffff) != HAL_OK){
        Error_Handler();
    }
}

//向SPI接口写命令
void LCD_Write_COMMAND(uint8_t address){
    LCD_DC_COMMMAND();
    
    LCD_CS_ENABLE();
    LCD_Write_Bus(address);
    LCD_CS_DISABLE();
}

//向SPI接口写8位数据
void LCD_Write_Data8(uint8_t data){
    LCD_DC_DATA();
    
    LCD_CS_ENABLE();
    LCD_Write_Bus(data);
    LCD_CS_DISABLE();
}

//向SPI接口写16位数据
void LCD_Write_Data16(uint16_t data){

    uint8_t temBuf[2];
    temBuf[0] = data >> 8;
    temBuf[1] = data & 0xff;

    LCD_DC_DATA();
    
    LCD_CS_ENABLE();
    if(HAL_SPI_Transmit(&hspi2,(uint8_t*)temBuf,2,0xffff) != HAL_OK ){
        Error_Handler();
    }

    LCD_CS_DISABLE();
}

//设定数据书写的范围
void LCD_Zone_Set(uint8_t xStart, uint8_t yStart, uint8_t xEnd, uint8_t yEnd){
    LCD_Write_COMMAND(ST7735_CASET);        // Column Address set
    LCD_Write_Data8(xStart>>8);
    LCD_Write_Data8(xStart);
    LCD_Write_Data8(xEnd>>8);
    LCD_Write_Data8(xEnd);

    LCD_Write_COMMAND(ST7735_RASET);        // Row Address Set 
    LCD_Write_Data8(yStart>>8);
    LCD_Write_Data8(yStart);
    LCD_Write_Data8(yEnd>>8);
    LCD_Write_Data8(yEnd);

    LCD_Write_COMMAND(ST7735_RAMWR);        // Write Data command
}

//初始化LCD
void LCD_Init(){
    
    LCD_RESET_DISABLE();
    HAL_Delay(5);
    LCD_RESET_ENABLE();
    HAL_Delay(5);
    LCD_RESET_DISABLE();
    HAL_Delay(20);
    
    LCD_Write_COMMAND(ST7735_SWRESET); //Software Reset
    HAL_Delay(20);

    LCD_Write_COMMAND(ST7735_SLPOUT); //Sleep out
    HAL_Delay(50);

    //--------------------------------------ST7735S Frame Rate----------------------------------------------//
    LCD_Write_COMMAND(ST7735_FRMCTR1); //Frame rate 80Hz Frame rate=333k/((RTNA + 20) x (LINE + FPA + BPA))
    LCD_Write_Data8(0x02);  //RTNA
    LCD_Write_Data8(0x35);  //FPA
    LCD_Write_Data8(0x36);  //BPA
    LCD_Write_COMMAND(ST7735_FRMCTR2); //Frame rate 80Hz
    LCD_Write_Data8(0x02);
    LCD_Write_Data8(0x35);
    LCD_Write_Data8(0x36);
    LCD_Write_COMMAND(ST7735_FRMCTR3); //Frame rate 80Hz
    LCD_Write_Data8(0x02);
    LCD_Write_Data8(0x35);
    LCD_Write_Data8(0x36);
    LCD_Write_Data8(0x02);
    LCD_Write_Data8(0x35);
    LCD_Write_Data8(0x36);

    //------------------------------------Display Inversion Control-----------------------------------------//
    LCD_Write_COMMAND(ST7735_INVCTR);  
    LCD_Write_Data8(0x03);

    //---------------------------------End ST7735S Power Sequence---------------------------------------//
    LCD_Write_COMMAND(ST7735_VMCTR1); //VCOM
    LCD_Write_Data8(0x0a);
    LCD_Write_COMMAND(ST7735_MADCTL); //MX, MY, RGB mode
    LCD_Write_Data8(0xC0);

    //------------------------------------ST7735S Gamma Sequence-----------------------------------------//
    //电源参数：定值
    LCD_Write_COMMAND(ST7735_GMCTRP1);
    LCD_Write_Data8(0x12);
    LCD_Write_Data8(0x1C);
    LCD_Write_Data8(0x10);
    LCD_Write_Data8(0x18);
    LCD_Write_Data8(0x33);
    LCD_Write_Data8(0x2C);
    LCD_Write_Data8(0x25);
    LCD_Write_Data8(0x28);
    LCD_Write_Data8(0x28);
    LCD_Write_Data8(0x27);
    LCD_Write_Data8(0x2F);
    LCD_Write_Data8(0x3C);
    LCD_Write_Data8(0x00);
    LCD_Write_Data8(0x03);
    LCD_Write_Data8(0x03);
    LCD_Write_Data8(0x10);
    LCD_Write_COMMAND(ST7735_GMCTRN1);
    LCD_Write_Data8(0x12);
    LCD_Write_Data8(0x1C);
    LCD_Write_Data8(0x10);
    LCD_Write_Data8(0x18);
    LCD_Write_Data8(0x2D);
    LCD_Write_Data8(0x28);
    LCD_Write_Data8(0x23);
    LCD_Write_Data8(0x28);
    LCD_Write_Data8(0x28);
    LCD_Write_Data8(0x26);
    LCD_Write_Data8(0x2F);
    LCD_Write_Data8(0x3B);
    LCD_Write_Data8(0x00);
    LCD_Write_Data8(0x03);
    LCD_Write_Data8(0x03);
    LCD_Write_Data8(0x10);
    //------------------------------------End ST7735S Gamma Sequence-----------------------------------------//
    LCD_Write_COMMAND(ST7735_COLMOD); //16-bit pixel
    LCD_Write_Data8(0x05);
    LCD_Write_COMMAND(ST7735_DISPON); //Display on 
    HAL_Delay(50); 

}

//填充整个屏幕为某个颜色
void LCD_fillScreen(uint16_t color)
{
    // LCD_Zone_Set(0, 0, LCD_W - 1, LCD_H - 1);
    // // uint8_t temBuf[2];
    // // temBuf[0] = color >> 8;
    // // temBuf[1] = color & 0xff;

    // LCD_DC_DATA();
    // LCD_CS_ENABLE();
    
    // for (uint8_t i = 0; i < LCD_W; i++) {
    //     for (uint8_t j = 0; j < LCD_H; j++) {
    //         screenBuf[i][j][0] = color >> 8;
    //         screenBuf[i][j][1] = color & 0xff;
    //     }
    // }
    // HAL_SPI_Transmit_DMA(&hspi2, (uint8_t *)screenBuf, sizeof(screenBuf));
    // while(hspi2.State != HAL_SPI_STATE_READY);

    // LCD_CS_DISABLE();
}

//获取为屏幕填充任意颜色的16位（5-6-5）二进制数值
uint16_t RGB565( uint8_t r, uint8_t g, uint8_t b ){
    uint16_t sum;
    uint8_t _r, _g, _b;
    _r = (uint8_t)(int)((float)r/255.0f * 31.0f);
    _r &= 0x1f;
    _g = (uint8_t)(int)((float)g/255.0f * 63.0f);
    _g &= 0x3f;
    _b = (uint8_t)(int)((float)b/255.0f * 31.0f);
    _b &= 0x1f;

    sum = ( _r << 11 ) | ( _g << 5 ) | _b;

    return sum;
    
}

//在某个区域内填充颜色
void LCD_DrawRegion( int xS, int yS, int xE, int yE, uint16_t color){
    LCD_Zone_Set(xS, yS, xE, yE);
    for (int i = xS; i <= xE; i++)
    {
        for (int j = yS; j <= yE; j++)
        {
            LCD_Write_Data16(color);
        }
        
    }
    
}

//显示图像
void LCD_showimage(const unsigned char *p) {
 {
  	int i; 
	unsigned char picH,picL;
	LCD_fillScreen(ST7735_WHITE); //清屏  
	
	
			LCD_Zone_Set(0,0,127,159);		
		    for(i=0;i<128*160;i++)
			 {	
			 	picL=*(p+i*2);	//数据低位在前
				picH=*(p+i*2+1);				
				LCD_Write_Data16(picH<<8|picL);  						
			 }	
		 }
	}		

//显示文字
void LCD_showletter(int xS,int yS,const unsigned char *p){
    static uint16_t fontColor = ST7735_BLUE;
    static uint16_t backColor = ST7735_WHITE;
    LCD_Zone_Set(xS, yS, xS+7, yS+15);
    for (int j = 0; j < 16; j++)
    {
        for (int k = 0; k < 8; k++)
            {
                if(*(p+j)&(1<<k)){
                    LCD_Write_Data16(fontColor);
                }
                else{
                    LCD_Write_Data16(backColor);
                }
            }
            
        }
 }

 void drawNumber_1(int num, int xStart, int yStart)
{
	switch(num)
	{
		case 0: 
			LCD_showletter(xStart, yStart, Number_0); 
			break;

		case 1:  
			LCD_showletter(xStart, yStart, Number_1); 
			break;
		
		case 2:  
			LCD_showletter(xStart, yStart, Number_2); 
			break;

		case 3:  
			LCD_showletter(xStart, yStart, Number_3); 
			break;

		case 4:  
			LCD_showletter(xStart, yStart, Number_4); 
			break;

		case 5:  
			LCD_showletter(xStart, yStart, Number_5); 
			break;

		case 6:  
			LCD_showletter(xStart, yStart, Number_6); 
			break;

		case 7:  
			LCD_showletter(xStart, yStart, Number_7); 
			break;

		case 8:  
			LCD_showletter(xStart, yStart, Number_8); 
			break;

		case 9:  
			LCD_showletter(xStart, yStart, Number_9); 
			break;

		default:
			break;
	}
}

//显示一个两位数
void drawNumber_2(int num, int xStart, int yStart)
{
    drawNumber_1((num/10),xStart,yStart);
    drawNumber_1((num-(num/10)*10),(xStart+8),yStart);
}

//显示一个三位数
void drawNumber_3(int num, int xStart, int yStart)
{
    drawNumber_1((num/100),xStart,yStart);
    drawNumber_2((num-(num/100)*100),xStart+8,yStart);
}

//显示一个四位数
void drawNumber_4(int num, int xStart, int yStart)
{
    drawNumber_1((num/1000),xStart,yStart);
    drawNumber_3((num-(num/1000)*1000),xStart+8,yStart);
}

//显示一个五位数
void drawNumber_5(int num, int xStart, int yStart)
{
    drawNumber_1((num/10000),xStart,yStart);
    drawNumber_4((num-(num/10000)*10000),xStart+8,yStart);
}

//显示一个六位数
void drawNumber_6(int num, int xStart, int yStart)
{
    drawNumber_1((num/100000),xStart,yStart);
    drawNumber_5((num-(num/100000)*100000),xStart+8,yStart);
}

//显示一个小于6位的任意位数数字
void drawNumber(int num, int xStart, int yStart)
{
    if((num/100000) != 0) drawNumber_6(num,xStart,yStart);
    else if((num/10000) != 0) drawNumber_5(num,xStart,yStart);
    else if((num/1000) != 0) drawNumber_4(num,xStart,yStart);
    else if((num/100) != 0) drawNumber_3(num,xStart,yStart);
    else if((num/10) != 0) drawNumber_2(num,xStart,yStart);
    else drawNumber_1(num,xStart,yStart);
}

//测试代码
void lcd_init()
{
	
	
	LCD_fillScreen(ST7735_CASET);
  
	LCD_showletter(0,0,Letter_A);
	LCD_showletter(8,0,Letter_X);
	LCD_showletter(0,16,Letter_A);
	LCD_showletter(8,16,Letter_Y);
	LCD_showletter(0,32,Letter_A);
	LCD_showletter(8,32,Letter_Z);
	
	LCD_showletter(0,48,Letter_W);
	LCD_showletter(8,48,Letter_X);
	LCD_showletter(0,64,Letter_W);
	LCD_showletter(8,64,Letter_Y);
	LCD_showletter(0,80,Letter_W);
	LCD_showletter(8,80,Letter_Z);
	
	LCD_showletter(0,96,Letter_R);
	LCD_showletter(8,96,Letter_X);
	LCD_showletter(0,112,Letter_R);
	LCD_showletter(8,112,Letter_Y);
	LCD_showletter(0,128,Letter_R);
	LCD_showletter(8,128,Letter_Z);
	
	LCD_showletter(0,144,Letter_T);
}