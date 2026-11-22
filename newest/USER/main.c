#include "stm32f10x.h"
#include "interface.h"
#include "OLED_Driver.h"
#include "IRCtrol.h"
#include "motor.h"
#include "UltrasonicCtrol.h"
#include "SpeedCtrol.h"
#include "sys.h"	
#include "key.h"
#include "delay.h"	
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "malloc.h" 
#include "usart.h"
#include "usart2.h"
#include "common.h" 
#include "usart3.h"	
#include "usmart.h" 
#include "ov7670.h"
#include "exti.h"
#include "timer.h"
#include "mpu6050.h"




//全局变量定义
unsigned int speed_count=0;			//占空比计数器 50次一周期
char i=0;
char j=0;
char front_left_speed_duty=SPEED_DUTY;
char front_right_speed_duty=SPEED_DUTY;
char behind_left_speed_duty=SPEED_DUTY;
char behind_right_speed_duty=SPEED_DUTY;

unsigned int dis_left,dis_right;
unsigned int dis_direct;

unsigned char tick_1ms = 0;								//1ms计数器，作为电机的基本计数器
unsigned char tick_5ms = 0;								//5ms计数器，作为主函数的基本周期
unsigned char tick_200ms = 0;							//刷新LED闪烁显示
unsigned char tick_500ms = 0;							//电机驱动延时

unsigned char HalfSpeed = 0;
unsigned char Stopped = 0;

char ctrl_comm = COMM_STOP;								//控制指令
char ctrl_comm_last = COMM_STOP;					//上一次的指令
char RC_comm = COMM_STOP;									//控制指令
char WF_comm = COMM_STOP;									//控制指令
unsigned char continue_time=0;
unsigned char wf_rec_flag=0;							//WIFI上位机控制标志位

unsigned char duoji_count=0;
unsigned char zhuanjiao = 11;

unsigned char CCD_UD_duoji_count=0;
unsigned char CCD_UD_zhuanjiao = 11;
unsigned char CCD_LR_duoji_count=0;
unsigned char CCD_LR_zhuanjiao = 11;

float pitch,roll,yaw; 					//欧拉角
short aacx,aacy,aacz;						//加速度传感器原始数据
short gyrox,gyroy,gyroz;				//陀螺仪原始数据
short tempIMU;									//温度
short temp;
char Bright_check = 0;					//亮度检测结果，1：明亮；0：黑暗
char Red_light = 0;							//红灯识别结果，1：已识别；0：未识别
char Green_light = 0;						//绿灯识别结果，1：已识别；0：未识别
char Identification = 0;				//图形识别结果，待定
char void_right = 0;						//右侧红外避障结果，1：无障碍；0：有障碍
char void_left = 0;							//左侧红外避障结果，1：无障碍；0：有障碍
char Search_L = 0;							//循迹左侧结果，1：有黑线；0：无黑线
char Search_M = 0;							//循迹中间结果，1：有黑线；0：无黑线
char Search_R = 0;							//循迹右侧结果，1：有黑线；0：无黑线
char LF_Light = 0;							//左前灯状态，1：开灯；0：关灯
char RF_Light = 0;							//右前灯状态，1：开灯；0：关灯
char LB_Light = 0;							//左后灯状态，1：开灯；0：关灯
char RB_Light = 0;							//右后灯状态，1：开灯；0：关灯

char Is_Display = 0;						//是否通过串口发送视频
char Is_Movebyline = 0;					//是否循迹
char key = 0;										//按键输入

extern u8 ov_sta;								//在exit.c里面定义






//环境光亮度检测
void BrightCheck(void)
{
	int status=BRIGHT_IO;
	if(status == BRIGHT_Y){
		LF_Light_RESET; //灭
		RF_Light_RESET;
		LB_Light_RESET;
		RB_Light_RESET;
	}
	else{
		LF_Light_SET;
		RF_Light_SET;
		LB_Light_SET;
		RB_Light_SET;
	}
}


//获取惯性测量单元数据
void MPU_RUN(void)
{
//	u8 t=0;

	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
	{ 
		tempIMU=MPU_Get_Temperature();								//得到温度值
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);			//得到加速度传感器数据
	}
	
}

//小车运动控制
void Move(void) //
{
		switch(ctrl_comm)
		{
			case COMM_UP:  //小车前进  
			{	
				CarGo();
				break;
			}
			case COMM_DOWN:  //小车后退
			{
				CarBack();
				break;
			}
			case COMM_LEFT:  //小车左转
			{
				CarLeft();//motor.c里编程
				break;
			}
			case COMM_RIGHT: //小车右转
			{
				CarRight();//motor.c里编程
				break;
			}
			case COMM_STOP:  //小车停车
			{
				CarStop();
				break;
			}
			case COMM_SLOW:
			{
				CarSlow();
				break;
			}
			default : break;
		}
		Delayms(2);//防抖
}




void SearchLine(void)
{	

	if(j==1)
	{
		MPU_RUN();
		if(pitch > 12)
			j=0;
		
		if(SEARCH_M_IO & SEARCH_L_IO & SEARCH_R_IO)
		{
			ctrl_comm =COMM_UP;
			i=1;
			CarGo();
			return;
		}
		
		if((SEARCH_L_IO | SEARCH_R_IO) == 0)
		{
			if(SEARCH_M_IO == 1){
				if(i == 2) 
				{
					ctrl_comm = COMM_LEFT;
					i=1;
				}
				else if(i == 3) 
				{
					ctrl_comm = COMM_RIGHT;
					i=1;
				}
			
				else{ 
					ctrl_comm = COMM_UP;
					i=1;
				}
			}
		
			else
				switch(i)
				{
				case 1:ctrl_comm = COMM_UP;i=1;break;
				case 2:ctrl_comm = COMM_LEFT;i=2;break;
				case 3:ctrl_comm = COMM_RIGHT;i=3;break;
				}
		}
	
		else  
		{  
			if(SEARCH_R_IO == 0) 
			{
				ctrl_comm = COMM_LEFT;i=2; 
			}
			else 
			{
				ctrl_comm = COMM_RIGHT;i=3;
			}
		}
		return;		
	}
	
	
	else{		
		if(SEARCH_M_IO & SEARCH_L_IO & SEARCH_R_IO)  //如果三黑
		{
			ctrl_comm =COMM_STOP;
			i=0;
			CarStop();
			return;
		}
	
		Distance();
		if((distance_cm < 13 && distance_cm > 0)||(VOID_R_IO==0 || VOID_L_IO==0))
		{
			ctrl_comm = COMM_STOP;
			i=0;
			return;
		}
		if(distance_cm < 40 && distance_cm > 0)
		{
			ctrl_comm = COMM_SLOW;
			i=1;
			return;
		}	
	
		MPU_RUN();
		if(pitch < -12)
			j=1;
	
		if((SEARCH_L_IO | SEARCH_R_IO) == 0)  //如果左侧和右侧都是白色
		{
			if(SEARCH_M_IO == 1){  //如果中间是黑色，即判断为直行
				if(i == 2)  //如果之前在左转
				{
					ctrl_comm = COMM_LEFT;
					i=1;
				}
				else if(i == 3)  //如果之前是右转
				{
					ctrl_comm = COMM_RIGHT;
					i=1;
				}
			
				else{ //否则直行
					ctrl_comm = COMM_UP;
					i=1;
				}
			}
		
			else //中间也是白色
			{
				switch(i)
				{
				case 1:ctrl_comm = COMM_UP;i=1;break;
				case 2:ctrl_comm = COMM_LEFT;i=2;break;
				case 3:ctrl_comm = COMM_RIGHT;i=3;break;
				}
			}
		
		}
	
		else  //左侧和右侧存在黑色
		{  
			if(SEARCH_R_IO == 0) //如果是右白 左黑，左转
			{
				ctrl_comm = COMM_LEFT;i=2; 
			}
			else //如果左白右黑，右转
			{
				ctrl_comm = COMM_RIGHT;i=3;
			}
		}
		return;
	}
}

//控制信号接收
/*void SignalReceive(void)
{
	atk_8266_wifista_Rece();
	
	if(wf_rec_flag == 1)	//接收到WIFI信号
	{
		wf_rec_flag = 0;
		switch(WF_comm)
		{
			case COMM_UP:    	ctrl_comm = COMM_UP;break; //前进指令
			case COMM_DOWN:  	ctrl_comm = COMM_DOWN;break; //后退指令
			case COMM_LEFT:  	ctrl_comm = COMM_LEFT;break; //左转指令
			case COMM_RIGHT: 	ctrl_comm = COMM_RIGHT;break; //右转指令
			case COMM_STOP:  	ctrl_comm = COMM_STOP;break; //停车指令

			default : break;
		}
	}
	if(ir_rec_flag == 1)	//接收到红外信号
	{
		ir_rec_flag = 0;
		switch(RC_comm)
		{
			case COMM_UP:    	ctrl_comm = COMM_UP;break; //同上
			case COMM_DOWN:  	ctrl_comm = COMM_DOWN;break;
			case COMM_LEFT:  	ctrl_comm = COMM_LEFT;break;
			case COMM_RIGHT: 	ctrl_comm = COMM_RIGHT;break;
			case COMM_STOP:  	ctrl_comm = COMM_STOP;break;

			default : break;
		}
	}
}*/










//主函数
int main(void)
{

	delay_init_O(); 			//外部时钟选择
	delay_init();					//初始化延迟函数
	GPIOCLKInit();				//使能所有GPIO时钟
	UserLEDInit();				//初始化用户指示灯(PC13端口)
	//IRCtrolInit();				//初始化红外遥控配置 多车时，前车会受后车的红外避障发射影响
	TIM2_Init();					//初始化定时器2
	MotorInit();					//初始化电机控制接口
	UltraSoundInit();			//初始化超声波测距模块
	RedRayInit();					//初始化循迹模块及避障模块的红外对管
	ServoInit();					//初始化3路舵机控制接口
	SPI1_Init();					//初始化SPI1接口		
	//OLED_Init();					//初始化液晶显示
	BrightInit();					//初始化亮度检测接口
	LightInit();					//初始化车灯接口
	KEY_Init();						//初始化按键接口
	mem_init();						//初始化内存池
	uart_init(115200);  	//初始化串口1，波特率为115200，用于USMART
	USART3_Init(115200);  //初始化串口3，波特率为115200，用于WIFI模块
	//usmart_init(72);		//初始化USMART@72MHz
	MPU_Init();						//初始化惯性测量模块
	mpu_dmp_init();				//初始化惯性测量模块DMP
	
	
	EXTI8_Init();													//使能定时器捕获
	
	//=====以下为OLED液晶屏显示内容======================
	LCD_All();
	Delayms(50);
	LCD_Clear();
	
	/*display_list_char(0,1,"D:000cm");
	display_list_char(8,1,"S:000c/s");
	display_list_char(0,2,"T:-00.0");
	display_list_char(8,2,"R:-000.0");
	display_list_char(0,3,"P:-00.0");
	display_list_char(8,3,"Y:-000.0");
	display_list_char(0,0,"WIFI");	*/
	//display_list_char(0,0,"0 00000");	
	//====================================================
	
	



	//=========================================================================
	//WIFI模块上电自动进入透传模式，如不需要修改WIFI配置，可将初始化部分注释掉
	atk_8266_wifista_Init();
	//=========================================================================

	display_list_char(0,0,"0 00000");	
	
 //=======以下为主函数大循环==============================================
 while(1)
 {	 
	  if(tick_5ms >= 5) //5ms定时到
		{
			tick_5ms = 0;
			tick_200ms++;
			
			key = KEY_Scan(0);
			if(key == KEY1_PRES) //按键1：是否通过串口发送图像
			{
				if(Is_Display == 0)
				{
					Is_Display = 1;
					display_list_char(14,0,"1");	
				}
				else
				{
					Is_Display = 0;
					display_list_char(14,0,"0");
				}
			}
			else if(key == KEY2_PRES) //按键2：是否循迹行车
			{
				if(Is_Movebyline == 0)
				{
					Is_Movebyline = 1;
					display_list_char(15,0,"1");	
				}
				else
				{
					Is_Movebyline = 0;
					display_list_char(15,0,"0");
				}	
			}
			
			if(tick_200ms >= 4)	//实测调整
			{
				tick_200ms = 0;
				LEDToggle(LED_PIN); //用户指示灯闪烁,表明程序运行正常
				
				atk_8266_wifista_Tran();					//wifi发送数据至上位机 中速						
			}
			
			//MeasureSpeed();							//测速
			
			
			
			SearchLine();				//执行循迹检测
			
			//BrightCheck();			//执行环境光亮度检测
			
			//SignalReceive();		//控制信号接收
			
			
			
			Move();							//小车运动控制
			
			
		}
				
	}
}
