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
#include "stm32f3xx_hal.h"
#include "sixstep.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int
main(int argc, char* argv[])
{
  BSP_Init();

  SIXSTEP_Start();

  while (1)
    {
    }
}

#pragma GCC diagnostic pop
