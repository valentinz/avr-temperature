#include "temperature.h"
#include <string.h>

uint8_t temperature_init() {
	uint8_t result = 0x00;
	uint8_t nSensors = 0;
	uint8_t diff, i;
        uint8_t id[OW_ROMCODE_SIZE];

	ow_set_bus(&PIND,&PORTD,&DDRD,PD4);

	ow_reset();

	_delay_ms(1000);
	diff = OW_SEARCH_FIRST;
	while ( diff != OW_LAST_DEVICE && nSensors < MAXSENSORS ) {
		DS18X20_find_sensor( &diff, &id[0] );

		if( diff == OW_PRESENCE_ERR ) {
			result = 0xFF;
			break;
		}

		if( diff == OW_DATA_ERR ) {
			result = 0xFE;
			break;
		}

                for ( i=0; i < OW_ROMCODE_SIZE; i++ ) {
                        gSensorIDs[nSensors][i] = id[i];
                }
                nSensors++;
        }
        _delay_ms(1000);

	if (result == 0x00) {
		result = nSensors;
	}

	return result;
}


uint8_t temperature_measure(uint8_t *result, uint8_t length) {
        int16_t decicelsius;
	char s[10];
	uint8_t temperatureLength = 10;
	DS18X20_start_meas(DS18X20_POWER_PARASITE, NULL);
	_delay_ms( DS18B20_TCONV_12BIT );
	DS18X20_read_decicelsius_single(gSensorIDs[0][0], &decicelsius);
	DS18X20_format_from_decicelsius(decicelsius, s, temperatureLength);

	if (strlen(s) < temperatureLength) {
		temperatureLength = strlen(s);
	}
	uint8_t *temperature = (uint8_t *) &s;
	if (s[0] == '+') {
		temperatureLength--;
		temperature++;
	}

	if (temperatureLength > length + 1) {
		temperatureLength = length;
	}

	memcpy(result, temperature, temperatureLength + 1);

	return temperatureLength;
}
