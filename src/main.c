/*
 * main.c
 *
 *  Created on: Mar 9, 2022
 *      Author: Robin
 */

#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "diag/trace.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int
main(int argc, char* argv[])
{
  BSP_Init();

  while (1)
    {
      BSP_SetIndicator(0, 1);
      HAL_Delay(1000);
      BSP_SetIndicator(0, 0);
      HAL_Delay(1000);
    }
}

#pragma GCC diagnostic pop
