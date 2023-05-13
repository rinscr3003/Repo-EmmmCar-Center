#ifndef __BSP_FDETECT_H__
#define __BSP_FDETECT_H__

#include <stdint.h>

/*
 * BSP - Front Detector
 * Version = v1.0.0.0
 * Author = 9223020209
 * Comment = Detect front barriers.
 */

void BSP_FDetect_Init();
void BSP_FDetect_SetAngle(int8_t angle);
uint8_t BSP_FDetect_Read();

#endif