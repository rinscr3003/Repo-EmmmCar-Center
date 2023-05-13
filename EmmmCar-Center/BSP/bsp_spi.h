#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__

#include <stdint.h>

/*
 * BSP - SPI
 * Version = v1.0.0.0
 * Author = 9223020209
 * Comment = Communicate to sub-board.
 */

typedef enum
{
    SPICMD_SETMOTORPWM = 1,
    SPICMD_SETGIVENSPEED,
    SPICMD_SETPWMS,
    SPICMD_SETSPEEDS,
    SPICMD_GETSPEED,
    SPICMD_GETSPEEDS,
    SPICMD_GETPIDSTATE,
    SPICMD_SETPIDSTATE,
    SPICMD_MAXCMDINDEX
} _SPIPROC_CMDCODE;

void BSP_SPI_Init();
void BSP_SPI_SetMotorPWM(uint8_t motorId, uint16_t pwm);
void BSP_SPI_SetGivenSpeed(uint8_t motorId, float givenSpeed);
void BSP_SPI_SetSpeeds(float *speeds);
void BSP_SPI_AllBrake();
void BSP_SPI_SetSPIDState(uint8_t pidOn);

#endif