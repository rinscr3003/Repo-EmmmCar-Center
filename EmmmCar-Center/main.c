/*!
    \file    main.c
    \brief   led spark with systick, USART print and key example

    \version 2017-02-10, V1.0.0, firmware for GD32F30x
    \version 2018-10-10, V1.1.0, firmware for GD32F30x
    \version 2018-12-25, V2.0.0, firmware for GD32F30x
    \version 2020-09-30, V2.1.0, firmware for GD32F30x
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "gd32f30x.h"
#include "systick.h"
#include <stdio.h>
#include <stdint.h>

#include "bsp_fdetect.h"
#include "bsp_spi.h"
#include "bsp_ledbuz.h"
#include "bsp_linefind.h"
#include "director.h"

void gd_log_com_init()
{
  /* enable COM GPIO clock */
  rcu_periph_clock_enable(RCU_GPIOA);

  /* enable USART clock */
  rcu_periph_clock_enable(RCU_USART0);

  /* connect port to USARTx_Tx */
  gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

  /* connect port to USARTx_Rx */
  gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

  /* USART configure */
  usart_deinit(USART0);
  usart_baudrate_set(USART0, 115200U);
  usart_receive_config(USART0, USART_RECEIVE_ENABLE);
  usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
  usart_enable(USART0);
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/

int main(void)
{
  SystemInit();
  /* configure systick */
  systick_config();
  gd_log_com_init();

  BSP_FDetect_Init();
  BSP_SPI_Init();
  BSP_SPI_AllBrake();
  BSP_LEDBUZ_Init();
  BSP_LineFind_Init();

  delay_1ms(10);

  /* print out the clock frequency of system, AHB, APB1 and APB2 */
  printf("\r\nCK_SYS is %d", rcu_clock_freq_get(CK_SYS));
  printf("\r\nCK_AHB is %d", rcu_clock_freq_get(CK_AHB));
  printf("\r\nCK_APB1 is %d", rcu_clock_freq_get(CK_APB1));
  printf("\r\nCK_APB2 is %d", rcu_clock_freq_get(CK_APB2));

  rcu_periph_clock_enable(RCU_GPIOC);
  gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
  gpio_bit_set(GPIOC, GPIO_PIN_13);

  delay_1ms(1000);
  BSP_FDetect_SetAngle(-90);
  delay_1ms(1000);
  BSP_FDetect_SetAngle(90);
  delay_1ms(1000);
  BSP_FDetect_SetAngle(0);

  BSP_LEDBUZ_Flash(BSP_LEDBUZ_LED, 500, 2);
  BSP_LEDBUZ_Flash(BSP_LEDBUZ_BUZ, 1000, 1);

  // float speeds[4] = {4.0, 4.0, 4.0, 4.0};
  //  BSP_SPI_SetSPIDState(0);
  //  BSP_SPI_SetMotorPWM(0,800,0);
  //  BSP_SPI_SetMotorPWM(1,800,1);
  //  BSP_SPI_SetMotorPWM(2,800,0);
  //  BSP_SPI_SetMotorPWM(3,800,1);
  //  delay_1ms(1600);
  BSP_SPI_SetSPIDState(0x00);
  delay_1ms(5);
  BSP_SPI_AllBrake();

  Director_ThisState state;
  Director_Init(&state, POS_START);

  state.steps = 27;

  uint32_t lastPeriod = getSysPeriod();
  while (1)
  {
    //_Director_LoopLineFollow();
    Director_Loop(&state);
    if (getSysPeriod() - lastPeriod >= 1000)
    {
      gpio_bit_set(GPIOC, GPIO_PIN_13);
      lastPeriod = getSysPeriod();
    }
    else if (getSysPeriod() - lastPeriod >= 500)
    {
      gpio_bit_reset(GPIOC, GPIO_PIN_13);
    }
  }
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
  usart_data_transmit(USART0, (uint8_t)ch);
  while (RESET == usart_flag_get(USART0, USART_FLAG_TBE))
    ;

  return ch;
}
