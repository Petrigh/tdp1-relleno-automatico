#ifndef HELPER_H
#define HELPER_H

#include "sapi.h"

void initialize(void);
void configGalga(void);
void pasoTolva(void);
void readGalga(uint32_t value);
uint32_t promedio(uint32_t* value, int i);

#endif // HELPER_H
