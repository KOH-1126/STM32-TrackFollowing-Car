/* OLED 驱动头文件*/

#ifndef __OLED_Driver_H
#define __OLED_Driver_H

/* Includes ------------------------------------------------------------------*/
//#include "stm8s.h"
//#include "stm32f0xx.h"
//#include "bsp.h"
//#include "stm32lib.h"
//#include "STM8L15x.h"
#include "stm32f10x.h"
/* Exported constants --------------------------------------------------------*/

/* LCD Chip Select I/O definition */
//#define LCD_CS_PORT (GPIOE)
//#define LCD_CS_PIN  (GPIO_Pin_9)

/* LCD Reset I/O definition */
//#define LCD_RST_PORT (GPIOE)
//#define LCD_RST_PIN  (GPIO_Pin_8)

/* LCD Data/Command I/O definition */
//#define LCD_DC_PORT (GPIOE)
//#define LCD_DC_PIN  (GPIO_Pin_7)

/* LCD managed as 2 Lines of 15 characters (2Lines * 15Char) */
//#define LCD_LINE_MAX_CHAR ((u8)21)

/* LCD Commands */
#define COMMAND_TYPE       ((u8)0xF8)
#define DATA_TYPE          ((u8)0xFA)

//#define SH1106G							//

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void LCD_WDATA(unsigned char data);
void Write_Command(unsigned char commend);
//void LCD_ReadStatus(void);
void LCD_ChipSelect(FunctionalState NewState);
void LCD_SendByte(u8 DataType, u8 DataToSend);
//void LCD_SendBuffer(u8 *pTxBuffer, u8 *pRxBuffer, u8 NumByte);
void SPI1_Init(void);
void OLED_Init(void);
void Init_SH1106(void);
//void Internal_setting(void);
//void LCD_SSD1306_Init(void);
void LCD_ON(void);
void LCD_OFF(void);
void LCD_Clear(void);
void LCD_All(void);
void LCD_ClearLine(u8 Line);
//void LCD_SetCursorPos(u8 Line, u8 Offset);
void OLED_GOTO(unsigned char x,unsigned char y);
//void LCD_PrintChar(u8 Ascii);
//void LCD_PrintString(u8 Line, FunctionalState AutoComplete, FunctionalState Append, u8 *ptr);
//void LCD_PrintMsg(u8 *ptr);
//void LCD_Print(u8 *ptr);
//void LCD_PrintDec1(u8 Number);
//void LCD_PrintDec2(u8 Number);
//void LCD_PrintDec3(u16 Number);
//void LCD_PrintDec4(u16 Number);
//void LCD_PrintHex1(u8 Number);
//void LCD_PrintHex2(u8 Number);
//void LCD_PrintHex3(u16 Number);
//void LCD_PrintBin2(u8 Number);
//void LCD_PrintBin4(u8 Number);
//void LCD_RollString(u8 Line, u8 *ptr, u16 speed);
//void display_Of_num(unsigned char x,unsigned char y,unsigned char num1,unsigned char num2);
void display_num(unsigned char x,unsigned char y,unsigned int num);
void display_longnum(unsigned char x,unsigned char y,unsigned long num);
void display_HEX(unsigned char x,unsigned char y,unsigned char num);
void display_list_char(unsigned char x,unsigned char y,char *data_s);
void display_n_char(unsigned char n,unsigned char x,unsigned char y,char *data_s);
//void display_5_char(unsigned char x,unsigned char y,unsigned char *data_s);
//void display_6_char(unsigned char x,unsigned char y,unsigned char *data_s);
//void display_7_char(unsigned char x,unsigned char y,unsigned char *data_s);
//void display_10_char(unsigned char x,unsigned char y,unsigned char *data_s);
//void display_one_char(unsigned char x,unsigned char y,unsigned char char_data);
//void Display_Bar(unsigned char x,unsigned char y,unsigned char num);
//void Display_TimeFont(unsigned char x,unsigned char y,unsigned char num);
//void Display_vol(unsigned char x,unsigned char y,unsigned char vol);
//void Display_Freq(unsigned char x,unsigned char y,unsigned int Freq);

//void LCD_DrawPicture(u8 StartX,u8 StartY,u8 EndX,u8 EndY,int8u *pic);
//void LCD_PutChar(unsigned char x,unsigned char y,unsigned char str8);
//void LCD_PutHZ(u8 x,u8 y,u16 hz);
//void LCD_PutDHZ(u8 x,u8 y,u16 hz);
//void LCD_PutDSZ(u8 x,u8 y,u16 hz);
//void LCD_Print_Str(u8 x,u8 y,char* str);
//void LCD_ClearArea(u8 Startx, u8 Endx, u8 Starty, u8 Endy);

#endif /* __OLED_Driver_H */
//-----------End Of File-------------------
