#include "bsp_linefind.h"
#include "gd32f30x.h"
#include <stdint.h>
#include "systick.h"

/*
 * BSP - LineFind
 * Version = v1.0.0.0
 * Author = 9223020209
 * Comment = Finds lines on the ground.
 */

void BSP_LineFind_Init()
{
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9);
}

BSP_LineFind_Result BSP_Linefind_Read()
{
    BSP_LineFind_Result res;
    res.L4 = gpio_input_bit_get(GPIOB, GPIO_PIN_12);
    res.L3 = gpio_input_bit_get(GPIOB, GPIO_PIN_13);
    res.L2 = gpio_input_bit_get(GPIOB, GPIO_PIN_14);
    res.L1 = gpio_input_bit_get(GPIOB, GPIO_PIN_15);
    res.R1 = gpio_input_bit_get(GPIOC, GPIO_PIN_6);
    res.R2 = gpio_input_bit_get(GPIOC, GPIO_PIN_7);
    res.R3 = gpio_input_bit_get(GPIOC, GPIO_PIN_8);
    res.R4 = gpio_input_bit_get(GPIOC, GPIO_PIN_9);
    return res;
}
