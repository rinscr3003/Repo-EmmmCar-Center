#include "bsp_ledbuz.h"
#include "gd32f30x.h"
#include <stdint.h>
#include "systick.h"

/*
 * BSP - LEDBUZ
 * Version = v1.0.0.0
 * Author = 9223020209
 * Comment = Controls the LED and buzzer.
 */

void BSP_LEDBUZ_Init()
{
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4 | GPIO_PIN_5);
    gpio_bit_reset(GPIOC, GPIO_PIN_4 | GPIO_PIN_5);
}

void BSP_LEDBUZ_Flash(BSP_LEDBUZ_COMBO comboType, uint16_t t, uint8_t cycle)
{
    uint32_t pin = (((comboType & BSP_LEDBUZ_LED) == BSP_LEDBUZ_LED) ? GPIO_PIN_4 : 0) | (((comboType & BSP_LEDBUZ_BUZ) == BSP_LEDBUZ_BUZ) ? GPIO_PIN_5 : 0);
    for (uint8_t i = 0; i < cycle; i++)
    {
        gpio_bit_set(GPIOC, pin);
        delay_1ms(t / 2);
        gpio_bit_reset(GPIOC, pin);
        if (i + 1 < cycle)
            delay_1ms(t / 2);
    }
}
