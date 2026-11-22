/* OLED 驱动源文件*/
/* Includes ------------------------------------------------------------------*/
#include "OLED_Driver.h"
#include "interface.h"
#include "font8_16.h"
#include "stm32f10x.h"
#include "stm32f10x_spi.h"
//#include "font16_16.h"
//#include "font15_32.h"
//#include "font30_32.h"
//#include "STM8L15x.h"
//#include "bsp_spi.h"
//#include "HZ16x16.h"
//#include "ASCII6x8.h"
/* Private define ------------------------------------------------------------*/
//#define STATUS_TYPE 0xFC

/** @addtogroup LCD_Private_Functions */

void LCD_Delay(u16 nCount)
{
  /* Decrement nCount value */
  while (nCount != 0) {nCount--;}
}

/**
  * @brief read status from LCD
  * @par Parameters:
  * None
  * @retval void None
  * @par Required preconditions:
  * None
  * @par Functions called:
  * - LCD_SendByte()
	* - LCD_Delay()
  */
//void LCD_ReadStatus(void)
//{
//  return;
//}

/**
  * @brief Send a byte to LCD through the SPI peripheral
  * @param[in] DataToSend Data to be sent
  * @retval u8 Data read
  * @par Required preconditions: * None
  * @par Functions called: * None
  */
//static void LCD_SPISendByte(u8 DataToSend)						//移植到M0系统改用M0的函数
//{
  /* Send byte through the SPI peripheral */
  //SPI->DR = DataToSend;
  //while ((SPI->SR & SPI_SR_TXE) == 0)
  //{
    /* Wait while the byte is transmitted */
  //}
//	Send_Byte(DataToSend);
//}

/** @addtogroup LCD_Public_Functions */

/**
  * @brief Enable or Disable the LCD through CS pin
  * @param[in] NewState CS pin state
  * @retval void None
  * @par Required preconditions: * None
  * @par Functions called: * - GPIO_WriteHigh() * - GPIO_WriteLow()
  * @par Example: * @code
  * LCD_ChipSelect(ENABLE); * @endcode
  */
void LCD_ChipSelect(FunctionalState NewState)
{
  if (NewState != DISABLE) LCD_CS_RESET; //{GPIO_ResetBits(LCD_CS_PORT, LCD_CS_PIN);} 	/* CS pin low: LCD enabled */
  else LCD_CS_SET; //{GPIO_SetBits(LCD_CS_PORT, LCD_CS_PIN);} 												/* CS pin high: LCD disabled */
}

/**
  * @brief Send a byte to LCD
  * @param[in] DataType Type of Data to be sent
  * @param[in] DataToSend Data to be sent
  * @retval void None
  * @par Required preconditions: * None
  * @par Functions called: * - LCD_ChipSelect * - LCD_SPISendByte
  * @par Example: LCD_SendByte(COMMAND_TYPE, DISPLAY_ON); * @endcode
  */
void LCD_SendByte(unsigned char DataType, unsigned char DataToSend)
{
	//static u8 t;
	
	if(DataType == COMMAND_TYPE) LCD_DC_RESET; //GPIO_ResetBits(LCD_DC_PORT, LCD_DC_PIN); 	/* DC pin Low: command */
	else if(DataType == DATA_TYPE) LCD_DC_SET; //GPIO_SetBits(LCD_DC_PORT, LCD_DC_PIN); 		/* DC pin high: Data */
	else return;

  LCD_ChipSelect(ENABLE); 	
  LCD_Delay(300);	
	/* Enable access to LCD */
  //LCD_SPISendByte(DataToSend); /* Send  byte */
  //Send_Byte(DataToSend);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1,DataToSend);
	//for(t=0;t<100;t++);
  LCD_ChipSelect(DISABLE); 	
  LCD_Delay(300);	
	/* Disable access to LCD */
}
//------------------------------------------------------------
void LCD_WDATA(unsigned char data) 
{ 
	LCD_SendByte(DATA_TYPE,data);
}
//------------------------------------------------------------
void LCD_WCOMM(unsigned char commend)
{
	LCD_SendByte(COMMAND_TYPE,commend);
}

void Write_Command(unsigned char commend)
{
	LCD_SendByte(COMMAND_TYPE,commend);
}

void Init_SH1106(void)					//-----------初始化代码
{

	 Write_Command(0xAE);     //Set Display Off 

   Write_Command(0xD5);     //display divide ratio/osc. freq. mode	
   Write_Command(0x80);     //
 
   Write_Command(0xA8);     //multiplex ration mode:63 
   Write_Command(0x3F);

   Write_Command(0xD3);     //Set Display Offset   
   Write_Command(0x00);
   
   Write_Command(0x40);     //Set Display Start Line 
   
   Write_Command(0xAD);     //DC-DC Control Mode Set 
   Write_Command(0x8B);     //DC-DC ON/OFF Mode Set  8A:off

   Write_Command(0x32);     //Set Pump voltage value 

   Write_Command(0xA1);     //Segment Remap	 

   Write_Command(0xC8);     //Set COM Output Scan Direction	

   Write_Command(0xDA);     //common pads hardware: alternative	
   Write_Command(0x12);

   Write_Command(0x81);     //contrast control 
   Write_Command(0x40);

   Write_Command(0xD9);	    //set pre-charge period	  
   Write_Command(0x1F);

   Write_Command(0xDB);     //VCOM deselect level mode 
   Write_Command(0x40);	    //

   Write_Command(0xA4);     //Set Entire Display On/Off	

   Write_Command(0xA6);     //Set Normal Display 

   Write_Command(0xAF);     //Set Display On  
}
	 
void LCD_ON(void)
{
	Write_Command(0xAF);     //Set Display On 
}

void LCD_OFF(void)
{
	Write_Command(0xAE);     //Set Display OFF 
}

////------------------设定X，Y坐标---------------------------
////#ifdef SH1106G

void OLED_GOTO(unsigned char x,unsigned char y)						////SH1106G
{											
  LCD_SendByte(COMMAND_TYPE, 0xB0+y);
	LCD_SendByte(COMMAND_TYPE, ((x+0x02)&0x0F));
	LCD_SendByte(COMMAND_TYPE, 0x10+(((x+0x02)>>4)&0x0F));
}

//#else
/*
void OLED_GOTO(unsigned char x,unsigned char y)
{
  //Set Page Address 
	//x += 0x10;											//SH1106G
  LCD_SendByte(COMMAND_TYPE, 0x22);
  LCD_SendByte(COMMAND_TYPE, y);    // start 
  LCD_SendByte(COMMAND_TYPE, 7);    // end
  //Set Column Address
	//y += 0x02;												//SH1106G
	//x += 0x02;
	  //LCD_SendByte(COMMAND_TYPE, 0x00+((x+0x02)&0x0F));
	//LCD_SendByte(COMMAND_TYPE, 0x10+(((x+0x02)>>4)&0x0F));
  LCD_SendByte(COMMAND_TYPE, 0x21);
  LCD_SendByte(COMMAND_TYPE, x);    // start 
  LCD_SendByte(COMMAND_TYPE, 127);  // end
}
*/
//#endif


/*	
void Internal_setting(void)			//(内部升压)
	{
		//RES=1;  
		//delay(1000);
		//RES=0;   
		//delay(1000);      
		//RES=1;
		//delay(1000);
		Write_Command(0xAE);    //display off

		Write_Command(0xD5);    //set osc division
		Write_Command(0x80);
		
		Write_Command(0xA8);    //multiplex ratio
		Write_Command(0x3F);    //duty = 1/64
		
		Write_Command(0xD3);    //set display offset
		Write_Command(0x00);   	//   0x20  
		
		Write_Command(0x40);    //set display start line
		
		Write_Command(0xAD);    //set charge pump enable
		Write_Command(0x8B);    //    0x8B    内供VCC   
		Write_Command(0x32);    //0X30---0X33  set VPP   9V 
		
		Write_Command(0xA1);    //set segment remap
		Write_Command(0xC8);    //Com scan direction
		
		Write_Command(0xDA);    //set COM pins
		Write_Command(0x12);
		
		Write_Command(0x81);    //contract control
		Write_Command(0x40);    //!128
		
		Write_Command(0xD9);    //set pre-charge period
		Write_Command(0x1F);    //0x22
		
		Write_Command(0xDB);    //set vcomh
		Write_Command(0x40);
		
		Write_Command(0xA4);    //Entire display on/off
		Write_Command(0xA6);    //normal / reverse

       
		//Write_Command(0x02);    //set lower  column address
		//Write_Command(0x10);    //set higher column address
		//Write_Command(0xB0);    //set page address

		Write_Command(0xAF);    //display ON
		
		LCD_Clear(); // Clear the LCD 
	} 
	
*/
/*
void LCD_SSD1306_Init(void)
	{
	//
	//Charge Pump Setting
		//LCD_SendByte(COMMAND_TYPE, 0xad); 
		//LCD_SendByte(COMMAND_TYPE, 0x8b);
		
		LCD_SendByte(COMMAND_TYPE, 0x8d);
		LCD_SendByte(COMMAND_TYPE, 0x14);
		
		LCD_SendByte(COMMAND_TYPE, 0xaf); 
	//Set Display Clock 
		LCD_SendByte(COMMAND_TYPE, 0xd5);
		LCD_SendByte(COMMAND_TYPE, 0xf0); 
	//Set Pre-charge Period
		LCD_SendByte(COMMAND_TYPE, 0xd9);  
		LCD_SendByte(COMMAND_TYPE, 0x11); 
	//Set VCOMH Deselect Level 
		LCD_SendByte(COMMAND_TYPE, 0xdb);
		LCD_SendByte(COMMAND_TYPE, 0x0); 
	//Set Norma Display 
		LCD_SendByte(COMMAND_TYPE, 0xa6);
	//Set Contrast Control
		LCD_SendByte(COMMAND_TYPE, 0x81);  
		LCD_SendByte(COMMAND_TYPE, 0xff); //1-256
	//Set Segment Re-map
		LCD_SendByte(COMMAND_TYPE, 0xa1);
	//Set COM Output Scan Direction 
		LCD_SendByte(COMMAND_TYPE, 0xc8); 
	//Set Memory Addressing Mode
		LCD_SendByte(COMMAND_TYPE, 0x20);
		LCD_SendByte(COMMAND_TYPE, 0x00); 
	//设定显示亮度
		LCD_SendByte(COMMAND_TYPE, 0x81);
		LCD_SendByte(COMMAND_TYPE, 0xFF); 
		LCD_Clear(); // Clear the LCD 
	}
	
*/
/**
  * @brief Initialize the LCD
  * @par Parameters: * None
  * @retval void None
  * @par Required preconditions: * None
  * @par Functions called:
  * - GPIO_Init - LCD_SendByte - LCD_Clear - LCD_Backlight
  * @par Example: LCD_Init();   @endcode
  */

void SPI1_Init(void)
{
	SPI_InitTypeDef   SPI_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	//SPI_I2S_DeInit(SPI1);
	SPI_Cmd(SPI1,DISABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	//SPI_SSOutputCmd(SPI1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LCD_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_SCK_GPIO , &GPIO_InitStructure);	
	//LCD_SCK_RESET;
	
	GPIO_InitStructure.GPIO_Pin = LCD_MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_MOSI_GPIO , &GPIO_InitStructure);	
	//LCD_MOSI_RESET;

	GPIO_InitStructure.GPIO_Pin = LCD_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_CS_PORT , &GPIO_InitStructure);	
	//LCD_CS_SET;
	
	GPIO_InitStructure.GPIO_Pin = LCD_RST_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_RST_PORT , &GPIO_InitStructure);	
	//LCD_RST_RESET;
	
	GPIO_InitStructure.GPIO_Pin = LCD_DC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_DC_PORT , &GPIO_InitStructure);	
	//LCD_DC_RESET;
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1,&SPI_InitStructure);
	
	SPI_Cmd(SPI1,ENABLE);
}

void OLED_Init(void)
{
	//GPIO_InitTypeDef    GPIO_InitStructure;
	
	/*
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);       //开启GPIOC端口
	
	//PC4配置为OLED模块的CMD,PC5配置为OLED模块的Reset
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_3;  	//选择第4、3端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;            	//配置为输出口
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         		//配置为推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	     		//50M时钟速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             	//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  								//选择第4端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;            	//配置为输出口
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         		//配置为推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	     		//50M时钟速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             	//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	*/
	
	
  /* Set LCD ChipSelect pin in Output push-pull low level (chip select disabled) */
  //GPIO_Init(LCD_CS_PORT, LCD_CS_PIN, GPIO_Mode_Out_PP_High_Fast);
	//GPIO_Init(LCD_RST_PORT, LCD_RST_PIN, GPIO_Mode_Out_PP_Low_Fast);
	//GPIO_Init(LCD_DC_PORT, LCD_DC_PIN, GPIO_Mode_Out_PP_Low_Fast);

	//GPIO_ResetBits(LCD_CS_PORT, LCD_CS_PIN);
	LCD_CS_RESET;
	//LCD_Delay(30000);//200
	//GPIO_SetBits(LCD_CS_PORT, LCD_CS_PIN);
  
	//LCD_CS_SET;
	
	//GPIO_ResetBits(LCD_RST_PORT, LCD_RST_PIN);
	LCD_RST_RESET;
	LCD_Delay(500);//200//30000
	//GPIO_SetBits(LCD_RST_PORT, LCD_RST_PIN);
	LCD_RST_SET;
	
	LCD_CS_SET;
	
	//#ifdef SH1106G
		//Internal_setting();
	Init_SH1106();
	//#else
		//LCD_SSD1306_Init();
	//#endif
	LCD_Clear();
	
}


/**
  * @brief Clear the LCD
  * @par Parameters: None
  * @retval void None
  */
void LCD_Clear(void)
{
  u16 i;
	u8  j;
  
	for(j=0;j<8;j++)
		{
			OLED_GOTO(0,j);
			for(i=0;i<128;i++)LCD_SendByte(DATA_TYPE,0x0);
		}
}

void LCD_All(void)
{
  u16 i;
	u8  j;
  
	for(j=0;j<8;j++)
		{
			OLED_GOTO(0,j);
			for(i=0;i<128;i++)LCD_SendByte(DATA_TYPE,0xFF);
		}
}


//***************************************************
void display_one_char(unsigned char x,unsigned char y,unsigned char char_data)
{
	unsigned char i;
	OLED_GOTO(x*8,y*2);
	char_data-=32;
	for (i=0;i<8;i++)
	{
		LCD_WDATA(ASCII_8_16[char_data][i]);
	}
	OLED_GOTO(x*8,y*2+1);
	for (i=8;i<16;i++)
	{
		LCD_WDATA(ASCII_8_16[char_data][i]);
	}
	LCD_WDATA(0);
}


//***************************************************
void display_list_char(unsigned char x,unsigned char y,char *data_s)
{
	while(*data_s)
	{
		display_one_char(x, y, *data_s); //显示单个字符     x,y是以字符为坐标
		x++;
		data_s++;
	}
}

void display_n_char(unsigned char n,unsigned char x,unsigned char y,char *data_s)
{
	int i;
	for(i=0;i<n;i++)
	{
		display_one_char(x, y, *data_s); //显示单个字符     x,y是以字符为坐标
		x++;
		data_s++;
	}
}
/*
void display_5_char(unsigned char x,unsigned char y,unsigned char *data_s)
{
	int i;
	for(i=0;i<5;i++)
	{
		display_one_char(x, y, *data_s); //显示单个字符     x,y是以字符为坐标
		x++;
		data_s++;
	}
}

void display_6_char(unsigned char x,unsigned char y,unsigned char *data_s)
{
	int i;
	for(i=0;i<6;i++)
	{
		display_one_char(x, y, *data_s); //显示单个字符     x,y是以字符为坐标
		x++;
		data_s++;
	}
}

void display_7_char(unsigned char x,unsigned char y,unsigned char *data_s)
{
	int i;
	for(i=0;i<7;i++)
	{
		display_one_char(x, y, *data_s); //显示单个字符     x,y是以字符为坐标
		x++;
		data_s++;
	}
}

void display_10_char(unsigned char x,unsigned char y,unsigned char *data_s)
{
	int i;
	for(i=0;i<10;i++)
	{
		display_one_char(x, y, *data_s); //显示单个字符     x,y是以字符为坐标
		x++;
		data_s++;
	}
}
*/

//----------------------------------------------------------------------------
void display_HEX(unsigned char x,unsigned char y,unsigned char num)
{
  unsigned char temp;
  
  temp=(num>>4)&0x0f;
  if(temp<10)display_one_char(x,y,temp+0x30);
  else display_one_char(x,y,temp+'A'-10);
  temp=num&0x0F;
  if(temp<10)display_one_char(x+1,y,temp+0x30);
  else display_one_char(x+1,y,temp+'A'-10);
}


//----------------------------------------------------------------------------
void display_num(unsigned char x,unsigned char y,unsigned int num)
{
  //unsigned char i;
  //display_one_char(x++,y,'[');
  if(num<10){
	display_one_char(x++,y,num+0x30);
	//display_one_char(x++,y,']');
	//display_one_char(x++,y,' ');
	//display_one_char(x++,y,' ');
	//display_one_char(x++,y,' ');
	}
	else if(num<100){
	display_one_char(x++,y,num/10+0x30);
	display_one_char(x++,y,num%10+0x30);
	//display_one_char(x++,y,']');
	//display_one_char(x++,y,' ');
	//display_one_char(x++,y,' ');
	}else if(num<1000){
	display_one_char(x++,y,num/100+0x30);
	display_one_char(x++,y,num%100/10+0x30);
	display_one_char(x++,y,num%10+0x30);
	//display_one_char(x++,y,']');
	//display_one_char(x++,y,' ');
	}else{
	display_one_char(x++,y,num/1000+0x30);
	display_one_char(x++,y,num%1000/100+0x30);
	display_one_char(x++,y,num%100/10+0x30);
	display_one_char(x++,y,num%10+0x30);
	//display_one_char(x++,y,']');
	}
	
}

void display_longnum(unsigned char x,unsigned char y,unsigned long num)
{
  //unsigned char i;
  //display_one_char(x++,y,'[');
  if(num<10){
	display_one_char(x++,y,num+0x30);
	//display_one_char(x++,y,']');
	//display_one_char(x++,y,' ');
	//display_one_char(x++,y,' ');
	//display_one_char(x++,y,' ');
	}
	else if(num<100){
	display_one_char(x++,y,num/10+0x30);
	display_one_char(x++,y,num%10+0x30);
	//display_one_char(x++,y,']');
	//display_one_char(x++,y,' ');
	//display_one_char(x++,y,' ');
	}else if(num<1000){
	display_one_char(x++,y,num/100+0x30);
	display_one_char(x++,y,num%100/10+0x30);
	display_one_char(x++,y,num%10+0x30);
	//display_one_char(x++,y,']');
	//display_one_char(x++,y,' ');
	}else if(num<10000){
	display_one_char(x++,y,num/1000+0x30);
	display_one_char(x++,y,num%1000/100+0x30);
	display_one_char(x++,y,num%100/10+0x30);
	display_one_char(x++,y,num%10+0x30);
	//display_one_char(x++,y,']');
	}else if(num<100000){
	display_one_char(x++,y,num/10000+0x30);
	display_one_char(x++,y,num%10000/1000+0x30);
	display_one_char(x++,y,num%1000/100+0x30);
	display_one_char(x++,y,num%100/10+0x30);
	display_one_char(x++,y,num%10+0x30);
	}else if(num<1000000){
	display_one_char(x++,y,num/100000+0x30);
	display_one_char(x++,y,num%100000/10000+0x30);
	display_one_char(x++,y,num%10000/1000+0x30);
	display_one_char(x++,y,num%1000/100+0x30);
	display_one_char(x++,y,num%100/10+0x30);
	display_one_char(x++,y,num%10+0x30);	
	}else if(num<10000000){
	display_one_char(x++,y,num/1000000+0x30);	
	display_one_char(x++,y,num%1000000/100000+0x30);
	display_one_char(x++,y,num%100000/10000+0x30);
	display_one_char(x++,y,num%10000/1000+0x30);
	display_one_char(x++,y,num%1000/100+0x30);
	display_one_char(x++,y,num%100/10+0x30);
	display_one_char(x++,y,num%10+0x30);	
	}else if(num<100000000){
	display_one_char(x++,y,num/10000000+0x30);	
	display_one_char(x++,y,num%10000000/1000000+0x30);	
	display_one_char(x++,y,num%1000000/100000+0x30);
	display_one_char(x++,y,num%100000/10000+0x30);
	display_one_char(x++,y,num%10000/1000+0x30);
	display_one_char(x++,y,num%1000/100+0x30);
	display_one_char(x++,y,num%100/10+0x30);
	display_one_char(x++,y,num%10+0x30);
	}else if(num<1000000000){
	display_one_char(x++,y,num/100000000+0x30);
	display_one_char(x++,y,num%100000000/10000000+0x30);	
	display_one_char(x++,y,num%10000000/1000000+0x30);	
	display_one_char(x++,y,num%1000000/100000+0x30);
	display_one_char(x++,y,num%100000/10000+0x30);
	display_one_char(x++,y,num%10000/1000+0x30);
	display_one_char(x++,y,num%1000/100+0x30);
	display_one_char(x++,y,num%100/10+0x30);
	display_one_char(x++,y,num%10+0x30);
	}else{// if(num<10000000000){
	display_one_char(x++,y,num/1000000000+0x30);
	display_one_char(x++,y,num%1000000000/100000000+0x30);
	display_one_char(x++,y,num%100000000/10000000+0x30);	
	display_one_char(x++,y,num%10000000/1000000+0x30);	
	display_one_char(x++,y,num%1000000/100000+0x30);
	display_one_char(x++,y,num%100000/10000+0x30);
	display_one_char(x++,y,num%10000/1000+0x30);
	display_one_char(x++,y,num%1000/100+0x30);
	display_one_char(x++,y,num%100/10+0x30);
	display_one_char(x++,y,num%10+0x30);
	}
	
}

/*
//----------------------------------------------------------------------------
void display_Of_num(unsigned char x,unsigned char y,unsigned char num1,unsigned char num2)
{
  display_one_char(x++,y,'[');
  if(num2<10)display_one_char(x++,y,num2+0x30);
	else 
  {
	display_one_char(x++,y,num2/10+0x30);
	display_one_char(x++,y,num2%10+0x30);
	}
  display_one_char(x++,y,'-');
  if(num1<10)display_one_char(x++,y,num1+0x30);
	else 
  {
	display_one_char(x++,y,num1/10+0x30);
	display_one_char(x++,y,num1%10+0x30);
	}
  display_one_char(x++,y,']');

}
*/
/*
//--------------------------------------------------------------------
void Display_Bar(unsigned char x,unsigned char y,unsigned char num)
{
  unsigned char i,j;
  i=num/13;  //0-19
  OLED_GOTO(x,y);
  LCD_WDATA(0x7e);
  LCD_WDATA(0x81);
  for(j=0;j<i;j++)
  {
  LCD_WDATA(0xbd);
  LCD_WDATA(0xbd);
  LCD_WDATA(0xbd);
  LCD_WDATA(0x81);
  }
  for(j=0;j<19-i;j++)
  {
  LCD_WDATA(0x81);
  LCD_WDATA(0x81);
  LCD_WDATA(0x81);
  LCD_WDATA(0x81);
  }
  LCD_WDATA(0x7e);
}
*/
/**
  * @brief Clear one LCD
  * @param[in] Line Line to be cleared (LCD_LINE1 or LCD_LINE2)
  * @retval void None
  * @par Required preconditions:
  * - LCD must be enabled
  * @par Functions called:
  * - LCD_SendByte
  * @par Example:
  * @code
  * LCD_ClearLine(LCD_LINE1);
  * @endcode
  */
/*
void LCD_ClearLine(u8 Line)
{

  u8 CharPos;

  // Select the line to be cleared 
  OLED_GOTO(0,Line*2);

  // Clear the selected line 
  for (CharPos = 0; CharPos < 128; CharPos++)
  {
    LCD_SendByte(DATA_TYPE, 0x00);
  }
	
	// Select the line to be cleared 
  OLED_GOTO(0,Line*2+1);

  // Clear the selected line 
  for (CharPos = 0; CharPos < 128; CharPos++)
  {
    LCD_SendByte(DATA_TYPE, 0x00);
  }

}
*/
/**
  * @brief Set the LCD cursor to the specified location
  * @param[in] Line Line where the cursor will be set (LCD_LINE1 or LCD_LINE2)
  * @param[in] Offset is the position offset (only even position are supported)
  * @retval void None
  * @par Required preconditions:
  * - LCD must be enabled
  * @par Functions called:
  * - LCD_SendByte
  * @par Example:
  * @code
  * LCD_SetCursorPos(LCD_LINE1, 2);
  * @endcode
  */
/*
void LCD_SetCursorPos(u8 Line, u8 Offset)
{
//  LCD_SendByte(COMMAND_TYPE, (u8)(Line + Offset));
OLED_GOTO(Offset,Line);
}
*/
/**
  * @brief Display a character at the current cursor position
  * @param[in] Ascii is the character ascii code to be displayed
  * @retval void None
  * @par Required preconditions:
  * - LCD must be enabled
  * - The cursor position must be set before calling this function
  * @par Functions called:
  * - LCD_SendByte
  * @par Example:
  * @code
  * LCD_PrintChar('A');
  * @endcode
  */
/*
void LCD_PrintChar(u8 Ascii)
{
unsigned char i;
 // LCD_SendByte(DATA_TYPE, Ascii);
Ascii-=32;
for (i=0;i<6;i++)
{
  LCD_SendByte(DATA_TYPE,Font6x8[Ascii][i]); 
}
LCD_SendByte(DATA_TYPE,0);

}
*/
/**
  * @brief Display a string on the selected line of the LCD
  * @param[in] Line Line where to display the string (LCD_LINE1 or LCD_LINE2)
  * @param[in] AutoComplete Defines if the line must be completed with spaces or not
  * @param[in] Append Defines if the string is appended or not
  * @param[in] ptr Pointer to the string to display
  * @retval void None
  * @par Required preconditions:
  * - LCD must be enabled
  * @par Functions called:
  * - LCD_SendByte
  * @par Example:
  * @code
  * LCD_PrintString(LCD_LINE1, DISABLE, DISABLE, "Hello");
  * @endcode
  */
	
/*
void LCD_PrintString(u8 Line, FunctionalState AutoComplete, FunctionalState Append, u8 *ptr)
{

  u8 CharPos = 0;

	OLED_GOTO(0,Line);

  // Set cursor position at beginning of Line if Append option is enabled 

  if (Append == DISABLE)
  {
   
  }

  // Display each character of the string 
  while ((*ptr != 0) && (CharPos < LCD_LINE_MAX_CHAR))
  {
    LCD_PrintChar(*ptr);
    CharPos++;
    ptr++;
  }

  // Complete the line with spaces if AutoFill option is enabled 
  if (AutoComplete == ENABLE)
  {
    while (CharPos < LCD_LINE_MAX_CHAR)
    {
      LCD_PrintChar(' ');
      CharPos++;
    }
  }

}
*/

/**
  * @brief Display a string on the LCD with automatic carriage return
  * @param[in] ptr Pointer to the string to display
  * @retval void None
  * @par Required preconditions:
  * - LCD must be enabled
  * - The LCD is cleared prior to display the string
  * - The character % is not supported
  * @par Functions called:
  * - LCD_SendByte
  * @par Example:
  * @code
  * LCD_PrintMsg("This is a very long message");
  * @endcode
  */
	
	
/*
void LCD_PrintMsg(u8 *ptr)
{

  u8 Char = 0;
  u8 CharPos = 0;
	u8 linenum = 0;
  LCD_Clear(); // Clear the LCD display 

  // Set cursor to home position on line 1 
  OLED_GOTO(0, linenum);

  // Send String 
  while ((*ptr != 0) && (CharPos < (LCD_LINE_MAX_CHAR * 2)))
  {

    // Check if string length is bigger than LINE1 
    if (CharPos == LCD_LINE_MAX_CHAR)
    {
      OLED_GOTO(0, ++linenum); // Select second line 
    }

    Char = *ptr;

    switch (Char)
    {
      case ('\r'):
        // Carriage return 
        CharPos++;
        ptr++;
      break;
      case ('\n'):
        CharPos = 0;
        ptr++;
        // Set cursor to line 2 
        OLED_GOTO(0, ++linenum);
      break;
      default:
        // Display characters different from (\r, \n) 
        LCD_PrintChar(Char);
        CharPos++;
        ptr++;
      break;
    }
  }

}
*/

/**
  * @brief Display the Number in decimal format at the current cursor position
  * @param[in] Number Number to be displayed
  * @note Convert only numbers between 0 and 9 (max 1 digit)
  * @retval void None
  * @par Required preconditions:
  * - LCD must be enabled
  * - The cursor position must be set before calling this function
  * @par Functions called:
  * - LCD_PrintChar
  * @par Example:
  * @code
  * LCD_PrintDec2(6);
  * @endcode
  */
	
/*
void LCD_PrintDec1(u8 Number)
{

  u8 NbreTmp;

  if (Number < (u8)10)
  {
   
    // Display second digit of the number : 10 
    NbreTmp = (u8)(Number / (u8)10);
   
    // Display last digit of the number : Units 
    NbreTmp = (u8)(Number - (u8)((u8)10 * NbreTmp));
    LCD_PrintChar((u8)(NbreTmp + (u8)0x30));
   
  }

}
*/

/**
  * @brief Display the Number in decimal format at the current cursor position
  * @param[in] Number Number to be displayed
  * @note Convert only numbers between 0 and 99 (max 2 digits)
  * @retval void None
  * @par Required preconditions:
  * - LCD must be enabled
  * - The cursor position must be set before calling this function
  * @par Functions called:
  * - LCD_PrintChar
  * @par Example:
  * @code
  * LCD_PrintDec2(60);
  * @endcode
  */
	
/*
void LCD_PrintDec2(u8 Number)
{

  u8 NbreTmp;

  if (Number < (u8)100)
  {
   
    // Display second digit of the number : 10 
    NbreTmp = (u8)(Number / (u8)10);
    LCD_PrintChar((u8)(NbreTmp + (u8)0x30));
   
    // Display last digit of the number : Units 
    NbreTmp = (u8)(Number - (u8)((u8)10 * NbreTmp));
    LCD_PrintChar((u8)(NbreTmp + (u8)0x30));
   
  }

}
*/

/**
  * @brief Display the Number in decimal format at the current cursor position
  * @param[in] Number Number to be displayed
  * @note Convert only numbers between 0 and 999 (max 3 digits)
  * @retval void None
  * @par Required preconditions:
  * - LCD must be enabled
  * - The cursor position must be set before calling this function
  * @par Functions called:
  * - LCD_PrintChar
  * @par Example:
  * @code
  * LCD_PrintDec3(879);
  * @endcode
  */
	
/*
void LCD_PrintDec3(u16 Number)
{

  u8 Nbre1Tmp;
  u8 Nbre2Tmp;

  if (Number < (u16)1000)
  {
   
    // Display first digit of the number : 100 
    Nbre1Tmp = (u8)(Number / (u8)100);
    LCD_PrintChar((u8)(Nbre1Tmp + (u8)0x30));
   
    // Display second digit of the number : 10 
    Nbre1Tmp = (u8)(Number - ((u8)100 * Nbre1Tmp));
    Nbre2Tmp = (u8)(Nbre1Tmp / (u8)10);
    LCD_PrintChar((u8)(Nbre2Tmp + (u8)0x30));
   
    // Display last digit of the number : Units 
    Nbre1Tmp = ((u8)(Nbre1Tmp - (u8)((u8)10 * Nbre2Tmp)));
    LCD_PrintChar((u8)(Nbre1Tmp + (u8)0x30));
  }
  
}
*/

/**
  * @brief Display the Number in decimal format at the current cursor position
  * @param[in] Number Number to be displayed
  * @note Convert only numbers between 0 and 9999 (max 4 digits)
  * @retval void None
  * @par Required preconditions:
  * - LCD must be enabled
  * - The cursor position must be set before calling this function
  * @par Functions called:
  * - LCD_PrintChar
  * @par Example:
  * @code
  * LCD_PrintDec4(1600);
  * @endcode
  */
	
/*
void LCD_PrintDec4(u16 Number)
{

  u16 Nbre1Tmp;
  u16 Nbre2Tmp;

  if (Number < (u16)10000)
  {
   
    // Display first digit of the number : 1000 
    Nbre1Tmp = (u16)(Number / (u16)1000);
    LCD_PrintChar((u8)(Nbre1Tmp + (u8)0x30));
   
    // Display second digit of the number : 100 
    Nbre1Tmp = (u16)(Number - ((u16)1000 * Nbre1Tmp));
    Nbre2Tmp = (u16)(Nbre1Tmp / (u8)100);
    LCD_PrintChar((u8)(Nbre2Tmp + (u8)0x30));
   
    // Display second digit of the number : 10 
    Nbre1Tmp = (u16)(Nbre1Tmp - ((u16)100 * Nbre2Tmp));
    Nbre2Tmp = (u16)(Nbre1Tmp / (u16)10);
    LCD_PrintChar((u8)(Nbre2Tmp + (u8)0x30));
   
    // Display last digit of the number : Units 
    Nbre1Tmp = ((u16)(Nbre1Tmp - (u16)((u16)10 * Nbre2Tmp)));
    LCD_PrintChar((u8)(Nbre1Tmp + (u8)0x30));
  }

}
*/

/**
  * @brief Display the Number in Hexadecimal format at the current cursor position
  * @param[in] Number Number to be displayed
  * @note Convert only numbers between 0 and 15 (only 1 digit)
  * @retval void None
  * @par Required preconditions:
  * - LCD must be enabled
  * - The cursor position must be set before calling this function
  * @par Functions called:
  * - LCD_PrintChar
  * @par Example:
  * @code
  * LCD_PrintHex1(10);
  * @endcode
  */
	
/*
void LCD_PrintHex1(u8 Number)
{
  if (Number < (u8)0x0A)
  {
    LCD_PrintChar((u8)(Number + (u8)0x30));
  }
  else if (Number < (u8)0x10)
	{
		LCD_PrintChar((u8)(Number + (u8)0x37));
	}
	else
	{
		LCD_PrintChar('-');
	}
}
*/

/**
  * @brief Display the Number in Hexadecimal format at the current cursor position
  * @param[in] Number Number to be displayed
  * @note Convert only numbers between 0 and 255 (max 2 digits)
  * @retval void None
  * @par Required preconditions:
  * - LCD must be enabled
  * - The cursor position must be set before calling this function
  * @par Functions called:
  * - LCD_PrintChar
  * @par Example:
  * @code
  * LCD_PrintHex2(200);
  * @endcode
  */
	
/*
void LCD_PrintHex2(u8 Number)
{
  LCD_PrintHex1((u8)(Number >> (u8)4));
  LCD_PrintHex1((u8)(Number & (u8)0x0F));
}
*/
/**
  * @brief Display the Number in Hexadecimal format at the current cursor position
  * @param[in] Number Number to be displayed
  * @note Convert only numbers between 0 and 4095 (max 3 digits)
  * @retval void None
  * @par Required preconditions:
  * - LCD must be enabled
  * - The cursor position must be set before calling this function
  * @par Functions called:
  * - LCD_PrintChar
  * @par Example:
  * @code
  * LCD_PrintHex3(1C4);
  * @endcode
  */
/*
void LCD_PrintHex3(u16 Number)
{
  LCD_PrintHex1((u8)(Number >> (u8)8)); 
  LCD_PrintHex1((u8)((u8)(Number) >> (u8)4));
  LCD_PrintHex1((u8)((u8)(Number) & (u8)0x0F));
}
*/
/**
  * @brief Display the Number in binary format at the current cursor position
  * @param[in] Number Number to be displayed
  * @note Convert only numbers between 0 and 3 (max 2 digits)
  * @retval void None
  * @par Required preconditions:
  * - LCD must be enabled
  * - The cursor position must be set before calling this function
  * @par Functions called:
  * - LCD_PrintChar
  * @par Example:
  * @code
  * LCD_PrintBin2(2);
  * @endcode
  */
/*
void LCD_PrintBin2(u8 Number)
{
  LCD_PrintHex1((u8)((u8)(Number & (u8)0x02) >> (u8)1));
  LCD_PrintHex1((u8)(Number & (u8)0x01));
}
*/
/**
  * @brief Display the Number in binary format at the current cursor position
  * @param[in] Number Number to be displayed
  * @note Convert only numbers between 0 and 15 (max 4 digits)
  * @retval void None
  * @par Required preconditions:
  * - LCD must be enabled
  * - The cursor position must be set before calling this function
  * @par Functions called:
  * - LCD_PrintChar
  * @par Example:
  * @code
  * LCD_PrintBin4(15);
  * @endcode
  */
/*
void LCD_PrintBin4(u8 Number)
{
  LCD_PrintHex1((u8)((u8)(Number & (u8)0x08) >> (u8)3));
  LCD_PrintHex1((u8)((u8)(Number & (u8)0x04) >> (u8)2));
  LCD_PrintHex1((u8)((u8)(Number & (u8)0x02) >> (u8)1));
  LCD_PrintHex1((u8)(Number & (u8)0x01));
}
*/
/**
  * @brief Display a string in rolling mode
  * @param[in] Line Line used for displaying the text (LCD_LINE1 or LCD_LINE2)
  * @param[in] ptr Pointer to the text to display
  * @param[in] speed Rolling speed
  * @retval void None
  * @par Required preconditions:
  * - LCD must be enabled
  * @par Functions called:
  * - LCD_SendByte
  * - LCD_ClearLine
  * - LCD_Delay
  * @par Example:
  * @code
  * u8 *pText;
  * pText = "Welcome into the fabulous world of STM8...";
  * LCD_RollString(LCD_LINE2, pText, 0xC000);
  * @endcode
  */
	
/*
void LCD_RollString(u8 Line, u8 *ptr, u16 speed)
{

  u8 CharPos = 0;
  u8 *ptr2;
  
  // Set cursor position at beginning of line 
  LCD_SetCursorPos(Line,0);
  
  ptr2 = ptr;
  
  // Display each character of the string 
  while (*ptr2 != 0)
  {
    if (*ptr != 0)
    {
      LCD_PrintChar(*ptr);
      ptr++;
    }
    else
    {
      LCD_PrintChar(' ');
    }
    
    CharPos++;
   
    if (CharPos == LCD_LINE_MAX_CHAR)
    {
      LCD_Delay(speed);
      LCD_ClearLine(Line);
      LCD_SetCursorPos(Line,0);
      CharPos = 0;
      ptr2++;
      ptr = ptr2;
    }  
  }
}
*/
/**
  * @brief Display a string from current position of the LCD cursor
  * @param[in] ptr Pointer to the string to display
  * @retval void None
  * @par Required preconditions:
  * - LCD must be enabled
  * @par Functions called:
  * - LCD_SendByte
  * @par Example:
  * @code
  * LCD_Print("Hello");
  * @endcode
  */
/*
void LCD_Print(u8 *ptr) 
{
  while (*ptr) 					// Display the string 
    LCD_PrintChar(*ptr++);
}
*/
//--------------------------------------------------------------------
/*
void Display_TimeFont(unsigned char x,unsigned char y,unsigned char num)
{
	unsigned char i;
	if(y>=7)return;
	if(num<10)
	{
		OLED_GOTO(x,y++);
		for(i=0;i<12;i++)LCD_SendByte(DATA_TYPE,Font_Time[num][i]);
		OLED_GOTO(x,y);
		for(i=12;i<24;i++)LCD_SendByte(DATA_TYPE,Font_Time[num][i]);
	}
	else
	{
		OLED_GOTO(x,y++);
			for(i=0;i<6;i++)LCD_SendByte(DATA_TYPE,Font_Time[num][i]);
		OLED_GOTO(x,y);
			for(i=6;i<12;i++)LCD_SendByte(DATA_TYPE,Font_Time[num][i]);
	}
}
*/
/*
//--------------------------------------------------------------------
void Display_vol(unsigned char x,unsigned char y,unsigned char vol)
{
	unsigned char i;

	OLED_GOTO(x,y);
	for(i=0;i<38;i++)LCD_SendByte(DATA_TYPE,0);
	OLED_GOTO(x,y);
	vol+=1;
	vol>>=1;
	for (i=0;i<6;i++)
	{
		LCD_SendByte(DATA_TYPE,Font6x8['z'+1-32][i]);
	}
	LCD_SendByte(DATA_TYPE,0);

	for(i=0;i<vol;i++){
		LCD_SendByte(DATA_TYPE,vol_bar[i]);
		LCD_SendByte(DATA_TYPE,vol_bar[i]);
		LCD_SendByte(DATA_TYPE,vol_bar[i]);
		LCD_SendByte(DATA_TYPE,0);
	}
}
*/
/*
void Display_Freq(unsigned char x,unsigned char y,unsigned int Freq)
{
	//if(Freq/1000!=0)
	Display_TimeFont(x,y,Freq/1000);
	Display_TimeFont(x+12,y,Freq%1000/100);
	Display_TimeFont(x+24,y,Freq%100/10);
	Display_TimeFont(x+36,y,10);
	Display_TimeFont(x+42,y,Freq%10);
	display_one_char((x+42+12)/6+0,y+1,'M');
	display_one_char((x+42+12)/6+1,y+1,'h');
	display_one_char((x+42+12)/6+2,y+1,'z');
}
*/
/****************************以下是新增加的函数********************************************************
**函数信息 ：void Lcd_DrawPicture(............)         // WAN.CG // 2010.8.22
**功能描述 ：以指定的图片填充LCD指定的区域,图片取模格式为水平扫描，16位颜色模式
**输入参数 ：StartX     行起始座标
*            StartY     列起始座标
*            EndX       行结束座标
*            EndY       列结束座标
*            pic        图片头指针
**输出参数 ：
********************************************************************************************************/
/*
void Lcd_DrawPicture(u8 StartX,u8 StartY,u8 EndX,u8 EndY,int8u *pic)
{
	int16u i,j;
	int8u  temp, xLen;
  int8u  x, y, a;
  
	j = (EndX-StartX+1)*(EndY-StartY+1);
  xLen = (EndX-StartX+1);
  x = StartX;
  y = StartY;
  a = 0;
  OLED_GOTO(x,y);
  for (i=0;i<j;i++)
  {
    temp = (*pic++);
    LCD_SendByte(DATA_TYPE,temp);
    a++;
    if(a==xLen)
    {
      a = 0;
      x = StartX;
      y++;
      OLED_GOTO(x,y);
    }
  }
}
*/
/*
void LCD_ClearArea(u8 Startx, u8 Endx, u8 Starty, u8 Endy)
{                  //x=0~127 y=0~7       
  u8 Posx, Posy;

  for (Posy = Starty; Posy < Endy - Starty+1; Posy++)
  {
    // Select the line to be cleared 
    OLED_GOTO(Startx,Posy);
    for (Posx = Startx; Posx < Endx - Startx+1; Posx++)
    {
      // Clear the selected line 
      LCD_SendByte(DATA_TYPE, 0x00);
    }
  }
}
*/
/*********************************************************************************************************
**函数信息 ：void LCD_PutChar()       // WAN.CG // 2010.8.22
**功能描述 ：字符显示
**输入参数 ：x         行座标
*            y         列座标
**输出参数 ：
********************************************************************************************************/
/*
void LCD_PutChar(unsigned char x,unsigned char y,unsigned char str8)
{
  //display_list_char(x, y, (unsigned char *)str8);
  //display_one_char(x, y, *data_s);
  unsigned char i;
  
	OLED_GOTO(x+3,y+1);
	str8 -= 32;
	for (i=0;i<6;i++)
	{
		LCD_WDATA(ASCII_8_16[str8][i]);
	}
	LCD_WDATA(0);
} 
*/
/*********************************************************************************************************
**函数信息 ：void LCD_PutHZ()       // WAN.CG // 2010.8.22
**功能描述 ：汉字显示，汉字已事先读入缓冲区
**输入参数 ：x         行座标
*            y         列座标
**输出参数 ：
********************************************************************************************************/

/*
void LCD_PutHZ(u8 x,u8 y,u16 hz)
{
  u8  i, j;  
	u8  HZ_addr;
  u16 temp;
  //
  //for(i=0;i<45;i++)                                     //汉字有限库索引查询
	//{
	//	HZ_addr = i;
	//	temp = GB_12[HZ_addr].Index[1]<<8; 
	//	temp |= (GB_12[HZ_addr].Index[0] & 0x00FF);
	//	if(temp == hz)
	//		break;
	//}
	
	////HZ_addr=SEC_FONT16*SEC_SIZE+((c1-0xb0)*94+(c2-0xa1))*32;	//计算该汉字字库在spi falsh中的地址

  //for (i=0;i<2;i++)
  //{
  //  OLED_GOTO(x+3,y+i);
  //  for (j=0;j<12;j++)
  //  {
  //   LCD_WDATA(GB_12[HZ_addr].Msk[j+12*i]);                // 字符颜色
  //  }
  //}
	//
	
	OLED_GOTO(x*16,y*2);
	for (j=0;j<8;j++)
	{
		LCD_WDATA(Hanzi[hz][j]);
	}
	OLED_GOTO(x*16+8,y*2);
	for (j=8;j<16;j++)
	{
		LCD_WDATA(Hanzi[hz][j]);
	}
	OLED_GOTO(x*16,y*2+1);
	for (j=16;j<24;j++)
	{
		LCD_WDATA(Hanzi[hz][j]);
	}
	OLED_GOTO(x*16+8,y*2+1);
	for (j=24;j<32;j++)
	{
		LCD_WDATA(Hanzi[hz][j]);
	}
	LCD_WDATA(0);
}  
*/
//===========================================================================
/*
void LCD_PutDHZ(u8 x,u8 y,u16 hz)
{
  u8  i, j;  
	u8  HZ_addr;
  u16 temp;
	
	//Line 0
	OLED_GOTO(x*32,y*4);
	for (j=0;j<8;j++)
	{
		LCD_WDATA(DaHanzi[hz][j]);
	}
	OLED_GOTO(x*32+8,y*4);
	for (j=8;j<16;j++)
	{
		LCD_WDATA(DaHanzi[hz][j]);
	}
	OLED_GOTO(x*32+16,y*4);
	for (j=16;j<24;j++)
	{
		LCD_WDATA(DaHanzi[hz][j]);
	}
	OLED_GOTO(x*32+24,y*4);
	for (j=24;j<30;j++)
	{
		LCD_WDATA(DaHanzi[hz][j]);
	}
	
	//Line 1
	OLED_GOTO(x*32,y*4+1);
	for (j=30;j<38;j++)
	{
		LCD_WDATA(DaHanzi[hz][j]);
	}
	OLED_GOTO(x*32+8,y*4+1);
	for (j=38;j<46;j++)
	{
		LCD_WDATA(DaHanzi[hz][j]);
	}
	OLED_GOTO(x*32+16,y*4+1);
	for (j=46;j<54;j++)
	{
		LCD_WDATA(DaHanzi[hz][j]);
	}
	OLED_GOTO(x*32+24,y*4+1);
	for (j=54;j<60;j++)
	{
		LCD_WDATA(DaHanzi[hz][j]);
	}
	
	//Line 2
	OLED_GOTO(x*32,y*4+2);
	for (j=60;j<68;j++)
	{
		LCD_WDATA(DaHanzi[hz][j]);
	}
	OLED_GOTO(x*32+8,y*4+2);
	for (j=68;j<76;j++)
	{
		LCD_WDATA(DaHanzi[hz][j]);
	}
	OLED_GOTO(x*32+16,y*4+2);
	for (j=76;j<84;j++)
	{
		LCD_WDATA(DaHanzi[hz][j]);
	}
	OLED_GOTO(x*32+24,y*4+2);
	for (j=84;j<90;j++)
	{
		LCD_WDATA(DaHanzi[hz][j]);
	}
		
	//Line 3
	OLED_GOTO(x*32,y*4+3);
	for (j=90;j<98;j++)
	{
		LCD_WDATA(DaHanzi[hz][j]);
	}
	OLED_GOTO(x*32+8,y*4+3);
	for (j=98;j<106;j++)
	{
		LCD_WDATA(DaHanzi[hz][j]);
	}
	OLED_GOTO(x*32+16,y*4+3);
	for (j=106;j<114;j++)
	{
		LCD_WDATA(DaHanzi[hz][j]);
	}
	OLED_GOTO(x*32+24,y*4+3);
	for (j=114;j<120;j++)
	{
		LCD_WDATA(DaHanzi[hz][j]);
	}
	
	LCD_WDATA(0);
}  
*/

//===========================================================================

/*
void LCD_PutDSZ(u8 x,u8 y,u16 hz)
{
  u8  i, j;  
	u8  HZ_addr;
  u16 temp;
	
	//Line 0
	OLED_GOTO(x*16,y*4);
	for (j=0;j<8;j++)
	{
		LCD_WDATA(DaShuzi[hz][j]);
	}
	OLED_GOTO(x*16+8,y*4);
	for (j=8;j<15;j++)
	{
		LCD_WDATA(DaShuzi[hz][j]);
	}
	
	//Line 1
	OLED_GOTO(x*16,y*4+1);
	for (j=15;j<23;j++)
	{
		LCD_WDATA(DaShuzi[hz][j]);
	}
	OLED_GOTO(x*16+8,y*4+1);
	for (j=23;j<30;j++)
	{
		LCD_WDATA(DaShuzi[hz][j]);
	}
	
	//Line 2
	OLED_GOTO(x*16,y*4+2);
	for (j=30;j<38;j++)
	{
		LCD_WDATA(DaShuzi[hz][j]);
	}
	OLED_GOTO(x*16+8,y*4+2);
	for (j=38;j<45;j++)
	{
		LCD_WDATA(DaShuzi[hz][j]);
	}
		
	//Line 3
	OLED_GOTO(x*16,y*4+3);
	for (j=45;j<53;j++)
	{
		LCD_WDATA(DaShuzi[hz][j]);
	}
	OLED_GOTO(x*16+8,y*4+3);
	for (j=53;j<60;j++)
	{
		LCD_WDATA(DaShuzi[hz][j]);
	}
	
	LCD_WDATA(0);
}  
*/

/*********************************************************************************************************
**函数信息 ：LCD_Print_Str       // WAN.CG // 2010.8.22
**功能描述 ：字符串显示，汉字和字符显示
**输入参数 ：x         行座标
*            y         列座标
**输出参数 ：
********************************************************************************************************/
/*
void LCD_Print_Str(u8 x,u8 y,char* str)
{
	u16 hz;

	while ((*str != '\0'))
	{
		//if ((*str <= 0x80) && (*str >= 0x20)) // ASCII	
		//{
		//	Lcd_PutChar(x,y,*str);
		//	x += 6 ;                                
		//	str += 1 ;
		//}
		//else if (*str >= 0xb0)                //汉字
		//{
			hz=*str++;
			hz+=(*str++)<<8;				            //读取完整汉字内码
			Lcd_PutHZ(x,y,hz);                  //调用汉字显示函数
			x += 16;
		//}
		//else
		//	return;
	}       
}
*/
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
