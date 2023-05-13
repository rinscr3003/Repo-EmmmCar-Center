#ifndef __BSP_LINEFIND_H__
#define __BSP_LINEFIND_H__

#include <stdint.h>

/*
 * BSP - LineFind
 * Version = v1.0.0.0
 * Author = 9223020209
 * Comment = Finds lines on the ground.
 */

typedef struct
{
    uint8_t L4 : 1;
    uint8_t L3 : 1;
    uint8_t L2 : 1;
    uint8_t L1 : 1;
    uint8_t R1 : 1;
    uint8_t R2 : 1;
    uint8_t R3 : 1;
    uint8_t R4 : 1;
} BSP_LineFind_Result;

void BSP_LineFind_Init();
BSP_LineFind_Result BSP_Linefind_Read();

#endif