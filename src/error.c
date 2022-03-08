#include "error.h"
#include "stm32f3xx_hal.h"

void Error_Handler()
{
	__BKPT();

	for (;;) {

	}
}
