#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "sapi.h"
#include "servo.h"


enum veltState {NOHAYCAJA, TARA, CONFIGURANDO, TRANSICION, LLENANDO, COMPLETADO};
enum galgaState {LOAD, TARE};
void initPoncho(void);
void stMachine(void);


#endif // STATEMACHINE_H