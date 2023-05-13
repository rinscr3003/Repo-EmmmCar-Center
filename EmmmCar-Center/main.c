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
	
	
rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_AF);
    gpio_init(GPIOA,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_6);
	timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;
    rcu_periph_clock_enable(RCU_TIMER2);
    timer_deinit(TIMER2);
    timer_initpara.prescaler         = 119;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 4999;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2,&timer_initpara);

    /* CH0,CH1 and CH2 configuration in PWM mode */
    timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER2,TIMER_CH_0,&timer_ocintpara);

    /* CH0 configuration in PWM mode0,duty cycle 25% */
    timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_0,1500);
    timer_channel_output_mode_config(TIMER2,TIMER_CH_0,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER2,TIMER_CH_0,TIMER_OC_SHADOW_ENABLE);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER2);
    /* auto-reload preload enable */
    timer_enable(TIMER2);
		
		rcu_periph_clock_enable(RCU_GPIOB); 
    rcu_periph_clock_enable(RCU_SPI2);
    rcu_periph_clock_enable(RCU_AF);
		gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3 | GPIO_PIN_5);
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
		spi_parameter_struct spi_init_struct;
    /* SPI0 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_16;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI2, &spi_init_struct);
		gpio_bit_set(GPIOB,GPIO_PIN_6);
		spi_enable(SPI2);
    
    /* print out the clock frequency of system, AHB, APB1 and APB2 */
    printf("\r\nCK_SYS is %d", rcu_clock_freq_get(CK_SYS));
    printf("\r\nCK_AHB is %d", rcu_clock_freq_get(CK_AHB));
    printf("\r\nCK_APB1 is %d", rcu_clock_freq_get(CK_APB1));
    printf("\r\nCK_APB2 is %d", rcu_clock_freq_get(CK_APB2));

delay_1ms(1300);

for(uint16_t i=500;i<2500;i++){
					timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_0,i);
					delay_1ms(1);
				}
				for(uint16_t i=2500;i>500;i--){
					timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_0,i);
					delay_1ms(1);
				}
timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_0,1500);
				
				uint8_t spicmds[5]={SPICMD_SETSPEEDS,40,40,40,40};
				gpio_bit_reset(GPIOB,GPIO_PIN_6);
				delay_1ms(1);
				for(uint8_t i=0;i<5;i++){
				while(RESET == spi_i2s_flag_get(SPI2, SPI_FLAG_TBE)){;}
        spi_i2s_data_transmit(SPI2, spicmds[i]);
				}
				delay_1ms(1);
				gpio_bit_set(GPIOB,GPIO_PIN_6);
				uint8_t stopped=0;
    while (1){
			if(getSysPeriod()>=11000 && !stopped){
				gpio_bit_reset(GPIOB,GPIO_PIN_6);
				delay_1ms(1);
				spicmds[1]=0;
				spicmds[2]=0;
				spicmds[3]=0;
				spicmds[4]=0;
				for(uint8_t i=0;i<5;i++){
				while(RESET == spi_i2s_flag_get(SPI2, SPI_FLAG_TBE)){;}
        spi_i2s_data_transmit(SPI2, spicmds[i]);
				}
				delay_1ms(1);
				gpio_bit_set(GPIOB,GPIO_PIN_6);
				stopped=1;
			}

    }
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));

    return ch;
}
