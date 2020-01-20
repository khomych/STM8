/*
 *  File name:  lib_pwm.c
 *  Date first: 08/20/2018
 *  Date last:  08/22/2018
 *
 *  Description: STM8 Library for PWM and servos
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *
 ******************************************************************************
 *
 */

#include "stm8s_conf.h"


#include "lib_pwm.h"

static void set_duty(char, unsigned int);	/* internal set duty cycle */


/******************************************************************************
 *
 *  Initialize PWM
 *  in: mode, channels to use
 */

void pwm_init(char mode, char chans)
{

    TIM2->PSCR = TIM2_PRESCALER_8;      /* prescaler = 8 for 16mhz */


    switch (mode) {
    case PWM_DUTY :
	TIM2->ARRH = 0;
	TIM2->ARRL = 199;	/* 100 uS = 10khz */
	break;
    case PWM_SERVO :
	TIM2->ARRH = 39999 >> 8;	/* 20000 uS = 50 hz */
	TIM2->ARRL = 39999 & 255;
	break;
    }
    if (chans & PWM_C1) {
	TIM2->CCR1H = 0;
	TIM2->CCR1L = 0;
	TIM2->CCMR1  = 0x68;	/* PWM mode 1, use preload register */
	TIM2->CCER1 |= 0x01;	/* output enable, normal polarity */
    }
    if (chans & PWM_C2) {
	TIM2->CCR2H = 0;
	TIM2->CCR2L = 0;
	TIM2->CCMR2  = 0x68;
	TIM2->CCER1 |= 0x10;
    }
    if (chans & PWM_C3) {

	TIM2->CCR3H = 0;
	TIM2->CCR3L = 0;
	TIM2->CCMR3  = 0x68;
	TIM2->CCER2 |= 0x01;
    }

    TIM2->CR1 = 0x81;		/* use TIM2_ARR preload register, enable */
}

/******************************************************************************
 *
 *  Set new PWM duty cycle
 *  in: channel(s), percentage/2: 0 to 200
 */

void pwm_duty(char chan, char duty)
{
    set_duty(chan, duty);
}

/******************************************************************************
 *
 *  Set new servo position
 *  in: channel(s), position: 0 to 250
 */

void pwm_servo(char chan, char pos)
{
    int		duty;

    duty  = pos * 16;		/* get 0 to 2000 milliseconds */
    duty += 1000;		/* now 500 to 2500 mS */

    set_duty(chan, duty);
}

/******************************************************************************
 *
 *  Set duty count
 *  in: channel(s), new duty count (times 1/2 microsecond)
 */

static void set_duty(char chan, unsigned int duty)
{
    char	ch, cl;

    ch = duty >> 8;
    cl = duty & 0xff;

    if (chan & PWM_C1) {
	TIM2->CCR1H = ch;
	TIM2->CCR1L = cl;
    }
    if (chan & PWM_C2) {
	TIM2->CCR2H = ch;
	TIM2->CCR2L = cl;
    }
    if (chan & PWM_C3) {
	TIM2->CCR3H = ch;
	TIM2->CCR3L = cl;
    }
}



