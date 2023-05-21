#include "bsp_spi.h"
#include "gd32f30x.h"
#include <stdint.h>
#include "systick.h"
#include <math.h>

/*
 * BSP - SPI
 * Version = v1.0.0.0
 * Author = 9223020209
 * Comment = Communicate to sub-board.
 */

void BSP_SPI_Init()
{
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_SPI2);
    rcu_periph_clock_enable(RCU_AF);
    gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3 | GPIO_PIN_5);
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    gpio_bit_set(GPIOB, GPIO_PIN_6);
    spi_i2s_deinit(SPI2);
    spi_parameter_struct spi_init_struct;
    spi_init_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode = SPI_MASTER;
    spi_init_struct.frame_size = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_struct.nss = SPI_NSS_SOFT;
    spi_init_struct.prescale = SPI_PSC_64;
    spi_init_struct.endian = SPI_ENDIAN_MSB;
    spi_init(SPI2, &spi_init_struct);
    spi_enable(SPI2);
}

void BSP_SPI_SetMotorPWM(uint8_t motorId, uint16_t pwm, uint8_t direction)
{
    uint8_t pwmh = pwm >> 8;
    uint8_t pwml = pwm & 0x00ff;
    pwmh = (pwmh & 0x7f) | (direction ? 0x80 : 0x00);
    uint8_t spicmds[4] = {SPICMD_SETMOTORPWM, motorId, pwmh, pwml};
    gpio_bit_reset(GPIOB, GPIO_PIN_6);
    delay_1ms(1);
    for (uint8_t i = 0; i < 4; i++)
    {
        while (RESET == spi_i2s_flag_get(SPI2, SPI_FLAG_TBE))
        {
            ;
        }
        spi_i2s_data_transmit(SPI2, spicmds[i]);
    }
    while (SET == spi_i2s_flag_get(SPI2, SPI_FLAG_TRANS))
    {
        ;
    }
    delay_1ms(1);
    gpio_bit_set(GPIOB, GPIO_PIN_6);
    delay_1ms(5);
}

void BSP_SPI_SetGivenSpeed(uint8_t motorId, float givenSpeed)
{
    uint8_t mSpd = (uint8_t)(fabs(givenSpeed) * 10.0f) & 0x7f;
    uint8_t spd = mSpd | ((givenSpeed < 0) ? 0x80 : 0x00);
    uint8_t spicmds[3] = {SPICMD_SETGIVENSPEED, motorId, spd};
    gpio_bit_reset(GPIOB, GPIO_PIN_6);
    delay_1ms(1);
    for (uint8_t i = 0; i < 3; i++)
    {
        while (RESET == spi_i2s_flag_get(SPI2, SPI_FLAG_TBE))
        {
            ;
        }
        spi_i2s_data_transmit(SPI2, spicmds[i]);
    }
    while (SET == spi_i2s_flag_get(SPI2, SPI_FLAG_TRANS))
    {
        ;
    }
    delay_1ms(1);
    gpio_bit_set(GPIOB, GPIO_PIN_6);
    delay_1ms(5);
}

void BSP_SPI_SetSpeeds(float *speeds)
{
    uint8_t spicmds[5] = {SPICMD_SETSPEEDS, 0, 0, 0, 0};
    for (uint8_t i = 0; i < 4; i++)
    {
        uint8_t mSpd = (uint8_t)(fabs(speeds[i]) * 10.0f) & 0x7f;
        uint8_t spd = mSpd | ((speeds[i] < 0) ? 0x80 : 0x00);
        spicmds[1 + i] = spd;
    }
    gpio_bit_reset(GPIOB, GPIO_PIN_6);
    delay_1ms(1);
    for (uint8_t i = 0; i < 5; i++)
    {
        while (RESET == spi_i2s_flag_get(SPI2, SPI_FLAG_TBE))
        {
            ;
        }
        spi_i2s_data_transmit(SPI2, spicmds[i]);
    }
    while (SET == spi_i2s_flag_get(SPI2, SPI_FLAG_TRANS))
    {
        ;
    }
    delay_1ms(1);
    gpio_bit_set(GPIOB, GPIO_PIN_6);
    delay_1ms(5);
}

void BSP_SPI_AllBrake()
{
    BSP_SPI_SetMotorPWM(0,0,0);
    BSP_SPI_SetMotorPWM(1,0,0);
    BSP_SPI_SetMotorPWM(2,0,0);
    BSP_SPI_SetMotorPWM(3,0,0);
}

void BSP_SPI_SetSPIDState(uint8_t pidOn)
{
    uint8_t spicmds[2] = {SPICMD_SETPIDSTATE, pidOn};
    gpio_bit_reset(GPIOB, GPIO_PIN_6);
    delay_1ms(1);
    for (uint8_t i = 0; i < 2; i++)
    {
        while (RESET == spi_i2s_flag_get(SPI2, SPI_FLAG_TBE))
        {
            ;
        }
        spi_i2s_data_transmit(SPI2, spicmds[i]);
    }
    while (SET == spi_i2s_flag_get(SPI2, SPI_FLAG_TRANS))
    {
        ;
    }
    delay_1ms(1);
    gpio_bit_set(GPIOB, GPIO_PIN_6);
    delay_1ms(5);
}
