#ifndef HELPER_H
#define HELPER_H

#include "sapi.h"

void initialize(void);
void configGalga(void);
void pasoTolva(void);
uint32_t readGalga(void);
uint32_t promedio(uint32_t* value, int i);
char* itoa(int value, char* result, int base);
void printNums(uint32_t promedio, char* buffer, uint32_t* result);

#endif // HELPER_H
