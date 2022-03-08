/*
 * sixstep.h
 *
 *  Created on: Mar 5, 2022
 *      Author: Robin
 */

#ifndef INCLUDE_SIXSTEP_H_
#define INCLUDE_SIXSTEP_H_

#include <stdint.h>

typedef struct {
	uint8_t is_running;
} sixstep_state_t;

#define SIXSTEP_MODE_STANDBY 	0
#define SIXSTEP_MODE_RAMPUP		1
#define SIXSTEP_MODE_CLOSED		2


#endif /* INCLUDE_SIXSTEP_H_ */
