#include "SpeedCtrol.h"
#include "interface.h"
#include "OLED_Driver.h"

//轮子直径66mm，光电码盘齿数为20，轮子周长 207mm = 20.7cm 
//程序采用判断高低电平变化次数计数，也就是说轮子转一周计数次数为40
//一个计数变化表示轮子跑过的距离为 20.7/40 = 0.5175cm

unsigned char front_left_speed=0;
unsigned char front_right_speed=0;

unsigned char avg_speed=0;

unsigned char front_left_speed_temp=0;
unsigned char front_right_speed_temp=0;

static unsigned char front_left_io=0;
static unsigned char front_right_io=0;
static unsigned char count_5ms=0;

void MeasureInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = FRONT_RIGHT_S_PIN;//配置使能GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//配置GPIO端口速度
	GPIO_Init(FRONT_RIGHT_S_GPIO , &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = FRONT_LEFT_S_PIN;//配置使能GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//配置GPIO端口速度
	GPIO_Init(FRONT_LEFT_S_GPIO , &GPIO_InitStructure); 
}

/*******************************************************************************
* 函 数 名 ：ClearMeasure
* 函数功能 ：清除测量，重新开始测量
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void ClearMeasure(void)
{
	count_5ms = 0;
	front_left_speed_temp = 0;
	front_right_speed_temp = 0;
}

/*******************************************************************************
* 函 数 名 ：MeasureSpeed
* 函数功能 ：速度测量，计算IO变化次数，该函数必须每5ms调用一次
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void MeasureSpeed(void)
{
	count_5ms++;
	if(FRONT_LEFT_S_IO != front_left_io)//发生电平变化
	{
		front_left_speed_temp++;
		front_left_io = FRONT_LEFT_S_IO;
	}
	
	if(FRONT_RIGHT_S_IO != front_right_io)//发生电平变化
	{
		front_right_speed_temp++;
		front_right_io = FRONT_RIGHT_S_IO;
	}
	
	if(count_5ms == 25)//每500ms获取一次速度 //100
	{
		count_5ms = 0;
		front_left_speed = front_left_speed_temp *2 *6;//获取1s的高低电平变化次数,500mS则*2，*6修正
		front_right_speed = front_right_speed_temp *2 *6;
		front_left_speed_temp = 0;
		front_right_speed_temp = 0;
		
		front_left_speed = (unsigned char)(0.5175 * (double)front_left_speed + 0.5);//计算速度 cm/s 四舍五入
		front_right_speed = (unsigned char)(0.5175 * (double)front_right_speed + 0.5);//计算速度 cm/s 四舍五入
		//码盘：20格/圈 -> 40次变化/圈；车轮：直径65mm -> 周长204.2mm = 20.42cm
		//1S内电平变化n次，即n/40圈，即20.42*n/40cm 
		//车速 = (20.42/40)n = 0.5105*n
		avg_speed = (front_left_speed + front_right_speed)/2;

		if(avg_speed >= 100)
		{
			display_num(10,1,avg_speed);							//显示整数部分	  	    
		}
		else if (avg_speed >= 10 && avg_speed < 100)
		{
			display_num(10,1,0);												//显示整数部分	
			display_num(11,1,avg_speed);							//显示整数部分	
		}
		else
		{
			display_num(10,1,0);											//显示整数部分	
			display_num(11,1,0);											//显示整数部分	
			display_num(12,1,avg_speed);							//显示整数部分	
		}
		ClearMeasure();
	}
}




