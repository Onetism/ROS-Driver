#include "odometry.h"
#include "encoder.h"
#include "math.h"

const float pi = 3.141593f;
const float pi_2_1 = 6.283186f;

//int32_t g_fodometry_right_speed=0,g_fodometry_left_speed=0;

float g_fdelta_time = 0.005f;
float g_fdelta_frquency = 200;

float g_fwheel_interval = 162;
float g_fwheel_diameter = 63.5;


float g_fposition_x=0,g_fposition_y=0;
float g_foriention=0,g_fvelocity_linear=0,g_velocity_angular=0;

float g_fsin = 0;
float g_fcos = 0;

float g_fdelta_distance=0,g_fdelta_oriention=0;

float g_fconst_frame=0,g_fconst_angle=0;
//float g_frun_distance=0,g_frun_angle=0;

float g_fcar_oriention=0;

u8 g_cIsfirst = 1;

void odometry(int32_t right_ticks,int32_t left_ticks)
{
	if(g_cIsfirst == 1)
	{
		g_fconst_frame = g_fwheel_diameter*pi/COUNTS_PER_REV;
		g_fconst_angle = g_fconst_frame/g_fwheel_interval;
		g_cIsfirst = 0;
	}
	
	if((right_ticks>0)&&(left_ticks<0))
	{
		g_fdelta_distance = (right_ticks+left_ticks);
		g_fdelta_oriention = right_ticks-left_ticks;
	}
	else if((right_ticks<0)&&(left_ticks>0))
	{
		g_fdelta_distance = (right_ticks+left_ticks);
		g_fdelta_oriention = right_ticks-left_ticks;	
	}
	else
	{
		g_fdelta_distance = 0.5f*(right_ticks+left_ticks);
		g_fdelta_oriention = right_ticks-left_ticks;
	}
	
	g_fdelta_oriention = g_fdelta_oriention*g_fconst_angle;
	g_foriention = g_foriention+g_fdelta_oriention;
	g_fcar_oriention =g_foriention+0.5f*g_fdelta_oriention;
	
	g_fsin = sin(g_fcar_oriention);
	g_fcos = cos(g_fcar_oriention);
	
	g_fposition_x = g_fposition_x+g_fdelta_distance*g_fcos*g_fconst_frame;
	g_fposition_y = g_fposition_y+g_fdelta_distance*g_fsin*g_fconst_frame;
	
	g_fvelocity_linear = g_fdelta_distance*g_fconst_frame*g_fdelta_frquency;
	g_velocity_angular = g_fdelta_oriention*g_fdelta_frquency;
	
	if(g_foriention>pi)
	{
		g_foriention -= pi_2_1;
	}
	else if(g_foriention<-pi)
	{
		g_foriention += pi_2_1;
	}
}
