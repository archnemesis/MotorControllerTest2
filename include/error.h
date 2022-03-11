/**
 * Error Definitions and Handlers
 */

#include <stdint.h>

typedef uint32_t status_t;

#define STATUS_OK					0x00
#define STATUS_ERR_MEM				0x01
#define STATUS_ERR_INVALID_STATE	0x02
#define STATUS_ERR_UNKOWN			0x03
#define STATUS_ERR_HAL				0x04

void Error_Handler(void);
