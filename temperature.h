#include <avr/io.h>
#include <util/delay.h>

#include "ds18x20_demo/onewire.h"
#include "ds18x20_demo/ds18x20.h"

#define MAXSENSORS 1
uint8_t gSensorIDs[MAXSENSORS][OW_ROMCODE_SIZE];

uint8_t temperature_init();
uint8_t temperature_measure(uint8_t *result, uint8_t length);
