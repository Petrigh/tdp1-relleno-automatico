#ifndef HX711_H
#define HX711_H

#include <stdint.h>
#include "sapi.h"

extern uint8_t PD_SCK;
extern uint8_t DOUT;
extern uint8_t GAIN;
extern long OFFSET;
extern float SCALE;
/*
void HX711_Init(uint8_t dout, uint8_t pd_sck, uint8_t gain);
bool HX711_is_ready();
void HX711_set_gain(uint8_t gain);
long HX711_read();
long HX711_read_average(uint8_t times);
double HX711_get_value(uint8_t times);
float HX711_get_units(uint8_t times);
void HX711_tare(uint8_t times);
void HX711_set_scale(float scale);
float HX711_get_scale();
void HX711_set_offset(long offset);
long HX711_get_offset();
void HX711_power_down();
void HX711_power_up();
*/

void initGalga(void);
/*
char* itoa(int value, char* result, int base);
void galgaMachine(char* result);

enum galga{CONFIGURANDO, LEYENDO, LISTO, ESCRIBIENDO};
enum galga estadoG;
int i;
*/


#endif // HX711_H
