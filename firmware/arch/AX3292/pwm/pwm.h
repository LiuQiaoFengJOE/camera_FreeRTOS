#ifndef _PWM_H_
#define _PWM_H_

#include "basic_defs.h"
#include "board_config.h"
#include "gpio_ops/gpio.h"

void timer_Timer1_PWM_Start(u32 pwm, u8 ch, u32 pin, u8 pwm_ch);

#endif