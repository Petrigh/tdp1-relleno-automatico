#ifndef HELPER_H
#define HELPER_H

#include "sapi.h"

void initialize(void);
void configGalga(void);
void pasoTolva();
uint32_t readGalga(void);
uint32_t promedio(uint32_t* value, int i);
char* itoa(int value, char* result, int base);

enum tolvaState{E1, E2, E3, E4}; //Estados Tolva;
enum tolvaState estadoTolva;

bool llenandoTolva;
bool controlGalga;
static uint32_t pesoAnterior, pesoActual;
#endif // HELPER_H
