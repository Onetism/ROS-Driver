#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "motor.h"
#include "encoder.h"
#include "millisecondtime.h"
#include "DataScope_DP.h"
#include "PID.h"
#include "battery.h"
#include "time.h"
#include "usart3.h"
#include "odometry.h"
#include "math.h"
#include "string.h"
#include "stdlib.h"

u8 odometry_send_sta = 0;
u8 odometry_calculate_sta = 0;
u16 speed_too_fast_num =0;
char speed_set = 20;
float noisy_k = 1;

char odometry_data[21]={0};   //发送给串口的里程计数据数组
union recieveData  //接收到的数据
{
	float d;    //左右轮速度
	unsigned char data[4];
}leftdata,rightdata;       //接收的左右轮数据

union odometry  //里程计数据共用体
{
	float odoemtry_float;
	unsigned char odometry_char[4];
}x_data,y_data,theta_data,vel_linear,vel_angular;     //要发布的里程计数据，分别为：X，Y方向移动的距离，当前角度，线速度，角速度


float left_speed=0,right_speed=0;
float g_fcar_battery=0;
float left_aim_speed=0,right_aim_speed=0;;
float left_pid_pwm=0,right_pid_pwm=0;

LED led_blue(1);											//蓝色LED
MOTOR right_motor(MOTOR1,7199,0);							//左电机
MOTOR left_motor(MOTOR2,7199,0);							//右电机
PID left_motor_pid(-7199, 7199, 7, 10, 0);					//左电机PID控制
PID right_motor_pid(-7199, 7199, 7, 16, 0);				//右电机PID控制
ENCODER right_encoder(ENCODER1, 0xffff, 0, COUNTS_PER_REV);	//左编码器
ENCODER left_encoder(ENCODER2, 0xffff, 0, COUNTS_PER_REV);	//右编码器
Battery battery(25, 10.6, 12.4);							//电池电量
TIME timing_tim1(TIME1,50,7200-1);							//速度定时5ms控制
TIME timing_tim4(TIME2,500,7200-1);							//里程计定时50ms发布时间


int main(void)
{
	u16 t=0;
	RCC->APB2ENR|=1<<4;     //使能PORTC时钟
	GPIOC->CRL&=0XFFFFFFF0;	//PC0设置成输入	  
	GPIOC->CRL|=0X00000008;   
	GPIOC->ODR|=1<<0;	   	//PC0上拉 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	initia_millisecondtime();
	
	led_blue.init();
	
	battery.init();
	
	left_encoder.init();
	right_encoder.init();
	 
	left_motor.init();
	right_motor.init();
	uart_init(115200); 
	uart3_init(36,9600);	
	
	timing_tim1.init();
	timing_tim4.init();
	while(1)
	{
//		if(PCin(0) == 0 )
//		{
//			delay_millis(10);
//			if(PCin(0) == 0)
//			{
//				left_aim_speed = -20;
//				right_aim_speed = -20;
//			}
//				
//		}
		if(speed_too_fast_num>400)
		{
			TIM_SetCompare1(TIM8,7199);
			TIM_SetCompare2(TIM8,7199);
			TIM_SetCompare3(TIM8,7199);
			TIM_SetCompare4(TIM8,7199);
			left_aim_speed = 0;
			right_aim_speed = 0;
			return 1;
		}

//		if(speed_update == 1)
//		{

////			printf("{B%d:%d:%d:%d:%d}$",speed,0,0,0,0);
//			DataScope_Get_Channel_Data(left_speed, 1 );
//			DataScope_Get_Channel_Data(right_speed, 2);
//			int Send_Count = DataScope_Data_Generate(2);
//			for( int i = 0 ; i < Send_Count; i++)
//			{
//				while((USART2->SR&0X40)==0);
//				USART2->DR = DataScope_OutPut_Buffer[i];
//			}
//			speed_update = 0;			
//		}
		if(odometry_calculate_sta == 1)
		{

			odometry(right_encoder.last_time_diff ,left_encoder.last_time_diff );
			odometry_calculate_sta = 0;
		}

		if(odometry_send_sta == 1)
		{
			printf("g_fcar_oriention %d:%f\n",t++,g_fcar_oriention);
			printf("g_fposition_x %d:%f\n",t++,g_fposition_x);
			printf("g_fposition_y %d:%f\n",t++,g_fposition_y);
			battery.filter();
			if(battery.get_volt()<11.4)
			{
				led_blue.revese();
			}
            //里程计数据获取
			x_data.odoemtry_float=g_fposition_x;//单位mm
			y_data.odoemtry_float=g_fposition_y;//单位mm
			theta_data.odoemtry_float=g_foriention;//单位rad
			vel_linear.odoemtry_float=g_fvelocity_linear;//单位mm/s
			vel_angular.odoemtry_float=g_velocity_angular;//单位rad/s		
            //将所有里程计数据存到要发送的数组
			for(int j=0;j<4;j++)
			{
				odometry_data[j]=x_data.odometry_char[j];
				odometry_data[j+4]=y_data.odometry_char[j];
				odometry_data[j+8]=theta_data.odometry_char[j];
				odometry_data[j+12]=vel_linear.odometry_char[j];
				odometry_data[j+16]=vel_angular.odometry_char[j];
			}
            
			odometry_data[20]='\n';//添加结束符
            
			//发送数据要串口
			for(int i=0;i<21;i++)
			{
				USART_ClearFlag(USART2,USART_FLAG_TC);  //在发送第一个数据前加此句，解决第一个数据不能正常发送的问题				
				USART_SendData(USART2,odometry_data[i]);//发送一个字节到串口	
				while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	//等待发送结束			
			}
			odometry_send_sta = 0;	
		}
		if(USART_RX_STA&0x8000)  // 串口1接收函数
		{			
            //接收左右轮速度
            for(t=0;t<4;t++)
            {
                rightdata.data[t]=USART_RX_BUF[t];
                leftdata.data[t]=USART_RX_BUF[t+4];
            }
            left_aim_speed = leftdata.d;
			right_aim_speed = rightdata.d;
            
			USART_RX_STA=0;//清楚接收标志位
		}
		if(USART3_RX_STA&0x8000)
		{
			u8 reclen = USART3_RX_STA&0X7FFF;
			USART3_RX_BUF[reclen] = 0;
			if(strcmp((const char*)USART3_RX_BUF,"qian_left")==0)
			{
				left_aim_speed = 0;
				right_aim_speed = speed_set;
			}
			else if(strcmp((const char*)USART3_RX_BUF,"qian_right")==0)
			{
				left_aim_speed = speed_set;
				right_aim_speed = 0;				
			}
			else if(strcmp((const char*)USART3_RX_BUF,"hou_right")==0)
			{
				left_aim_speed = -speed_set;
				right_aim_speed = 0;				
			}
			else if(strcmp((const char*)USART3_RX_BUF,"hou_left")==0)
			{
				left_aim_speed =0;
				right_aim_speed = -speed_set;				
			}
			else if(strcmp((const char*)USART3_RX_BUF,"qian")==0)
			{
				left_aim_speed = speed_set;
				right_aim_speed = speed_set;				
			}
			else if(strcmp((const char*)USART3_RX_BUF,"hou")==0)
			{
				left_aim_speed = -speed_set;
				right_aim_speed = -speed_set;				
			}
			else if(strcmp((const char*)USART3_RX_BUF,"ting")==0)
			{
				left_aim_speed = 0;
				right_aim_speed = 0;				
			}
			else if(strcmp((const char*)USART3_RX_BUF,"jian")==0)
			{
				speed_set -= 2;				
			}
			else if(strcmp((const char*)USART3_RX_BUF,"jia")==0)
			{
				speed_set += 2;			
			}
			USART3_RX_STA = 0;
		}
		
//			printf("{A%d:%d:%d:%d}$",0,0,(int)((g_fcar_battery*100-1110)*0.6667),0);

	
	}
}

#ifdef __cplusplus
 extern "C" {
#endif 
void TIM1_UP_IRQHandler(void)//里程计发布定时器中断函数
{
	if( TIM_GetITStatus(TIM1 , TIM_IT_Update) != RESET ) 
	{	
		odometry_calculate_sta = 1;	
		right_speed = right_encoder.get_PRM();
		left_speed = left_encoder.get_PRM();
		left_pid_pwm = left_motor_pid.compute(left_aim_speed,left_speed);
		right_pid_pwm = right_motor_pid.compute(right_aim_speed,right_speed);
		left_motor.setPWM(left_pid_pwm);
		right_motor.setPWM(right_pid_pwm);
			

		TIM_ClearITPendingBit(TIM1 , TIM_FLAG_Update);//清除中断标志位  		 
	}		 
}


void TIM4_IRQHandler(void)//小车速度计算定时器中断函数
{
	if ( TIM_GetITStatus(TIM4 , TIM_IT_Update) != RESET ) 
	{		
		odometry_send_sta = 1;
		if(abs(left_speed)>50||abs(right_speed)>50)
		{
			speed_too_fast_num++;
		}
		else
			speed_too_fast_num = 0;
			
		TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);//清除中断标志位    		 
	}		 
}

#ifdef __cplusplus
}
#endif
