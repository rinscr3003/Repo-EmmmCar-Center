#ifndef __BSP_LEDBUZ_H__
#define __BSP_LEDBUZ_H__

#include <stdint.h>

/*
 * BSP - LEDBUZ
 * Version = v1.0.0.0
 * Author = 9223020209
 * Comment = Controls the LED and buzzer.
 */

typedef enum
{
    BSP_LEDBUZ_NONE = 0,
    BSP_LEDBUZ_LED = 1,
    BSP_LEDBUZ_BUZ = 2,
    BSP_LEDBUZ_BOTH = 3
} BSP_LEDBUZ_COMBO;

void BSP_LEDBUZ_Init();
void BSP_LEDBUZ_Flash(BSP_LEDBUZ_COMBO comboType, uint16_t t, uint8_t cycle);

#endif