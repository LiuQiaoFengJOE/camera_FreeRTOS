#include "AX329x.h"
#include "pwm/pwm.h"

#define T1BAUDRATE 32768 //(ADC_BAUDRATE/20)
#define T1PSR 1
#define T1PR (((APB_CLK_CONFIG / T1BAUDRATE) / (1 << T1PSR)) - 1)
#define T1PWM_RANGE 20

void timer_Timer1_PWM_Start(u32 pwm, u8 ch, u32 pin, u8 pwm_ch)
{
	if (pwm > T1PWM_RANGE || pwm < 0)
		return;

	ax32xx_gpioMapSet(ch, pin, GPIO_FUNC_SFR);
	ax32xx_gpioDirSet(ch, pin, GPIO_OUTPUT);

	// REG32(PDDIR)  &= ~BIT(10);
	// REG32(PDFMAP) |= BIT(10);
	REG32(PMAPCON1) &= ~(BIT(4) | BIT(5) | BIT(6) | BIT(7));
	REG32(PMAPCON1) |= pwm_ch << 4;
	REG32(TMR1CON) &= ~(BIT(11) | BIT(12) | BIT(13));
	REG32(TMR1CON) = (T1PSR << 11);
	REG32(TMR1PR) = T1PR;
	REG32(TMR1PWM) = T1PR / T1PWM_RANGE * pwm;
	REG32(TMR1CNT) = 0x0000;
	REG32(TMR1CPND) |= (BIT(1) | BIT(0));
	REG32(TMR1CON) &= ~(BIT(0) | BIT(1));
	REG32(TMR1CON) |= BIT(1);
}
