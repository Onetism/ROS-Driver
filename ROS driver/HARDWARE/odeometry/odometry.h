#ifndef __ODOMETRY_H
#define __ODOMETRY_H

#include "sys.h"

extern float g_fposition_x,g_fposition_y;
extern float g_foriention,g_fvelocity_linear,g_velocity_angular;
extern float g_fcar_oriention;

void odometry(int32_t right_ticks,int32_t left_ticks);

#endif
