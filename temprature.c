#include "temprature.h"
#include "ds18x20_demo/onewire.h"
#include "ds18x20_demo/ds18x20.h"

#define MAXSENSORS 1
uint8_t gSensorIDs[MAXSENSORS][OW_ROMCODE_SIZE];

uint8_t temprature_init() {
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

void tempreture_measure(char *result) {
	uint8_t i;
	char s[10];
	uint8_t j=0;
        int16_t decicelsius;

	i = gSensorIDs[0][0]; // family-code for conversion-routine
	DS18X20_start_meas( DS18X20_POWER_PARASITE, NULL );
	_delay_ms( DS18B20_TCONV_12BIT );
	DS18X20_read_decicelsius_single( i, &decicelsius );
	DS18X20_format_from_decicelsius( decicelsius, s, 10 );
	/*while (s[j] != '\0' && j < 10) {
		*(result+j) = s[j];
		j++;
	}*/
}
