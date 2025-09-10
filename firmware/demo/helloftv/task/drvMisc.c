#include "task/app.h"
#include "gpio_ops/gpio.h"



#if AX329X_TYPE == 3297
void wifi_switch_init()
{
#if 0
	ax32xx_gpioMapSet(GPIO_PB, GPIO_PIN5, GPIO_FUNC_GPIO);
	ax32xx_gpioPullSet(GPIO_PB, GPIO_PIN5, GPIO_PULL_UP);
	ax32xx_gpioDirSet(GPIO_PB, GPIO_PIN5, GPIO_OUTPUT);
#endif
}
void wifi_switch_on(u8 mode)
{
#if 0
	if (mode)
		ax32xx_gpioDataSet(GPIO_PB, GPIO_PIN5, GPIO_HIGH);
	else
		ax32xx_gpioDataSet(GPIO_PB, GPIO_PIN5, GPIO_LOW);
#endif
}
#elif AX329X_TYPE == 3291

void wifi_switch_init()
{
#if 1
//	hal_gpioInit(GPIO_PA, GPIO_PIN7, GPIO_OUTPUT, GPIO_PULL_FLOATING);
  	hal_gpioInit(GPIO_PB, GPIO_PIN6, GPIO_OUTPUT, GPIO_PULL_FLOATING);
//	hal_gpioWrite(GPIO_PA, GPIO_PIN7, GPIO_HIGH);
  	hal_gpioWrite(GPIO_PB, GPIO_PIN6, GPIO_LOW);
	osal_task_sleep(100);
#endif
}
void wifi_switch_on(u8 mode)
{
#if 1

	if (mode)
		hal_gpioWrite(GPIO_PB, GPIO_PIN6, GPIO_HIGH);
	else
		hal_gpioWrite(GPIO_PB, GPIO_PIN6, GPIO_LOW);
#endif
}

#endif

