#include "bsp_fdetect.h"
#include "gd32f30x.h"
#include <stdint.h>

/*
 * BSP - Front Detector
 * Version = v1.0.0.0
 * Author = 9223020209
 * Comment = Detect front barriers.
 */

void BSP_FDetect_Init()
{
  rcu_periph_clock_enable(RCU_GPIOA);
  rcu_periph_clock_enable(RCU_AF);
  gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
  gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
  timer_oc_parameter_struct timer_ocintpara;
  timer_parameter_struct timer_initpara;
  rcu_periph_clock_enable(RCU_TIMER2);
  timer_deinit(TIMER2);
  timer_initpara.prescaler = 119;
  timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
  timer_initpara.counterdirection = TIMER_COUNTER_UP;
  timer_initpara.period = 4999;
  timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
  timer_initpara.repetitioncounter = 0;
  timer_init(TIMER2, &timer_initpara);
  timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
  timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
  timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
  timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
  timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
  timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
  timer_channel_output_config(TIMER2, TIMER_CH_0, &timer_ocintpara);
  timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_0, 1500);
  timer_channel_output_mode_config(TIMER2, TIMER_CH_0, TIMER_OC_MODE_PWM0);
  timer_channel_output_shadow_config(TIMER2, TIMER_CH_0, TIMER_OC_SHADOW_ENABLE);
  timer_auto_reload_shadow_enable(TIMER2);
  timer_enable(TIMER2);
}

void BSP_FDetect_SetAngle(int8_t angle)
{
  timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_0, 1500 - (angle * 100) / 9);
}

uint8_t BSP_FDetect_Read()
{
  return gpio_input_bit_get(GPIOA, GPIO_PIN_7) == RESET;
}
