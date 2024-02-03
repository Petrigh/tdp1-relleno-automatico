#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "sapi.h"

enum veltState {START, NOHAYCAJA, TARA, CONFIGURANDO, TRANSICION, LLENANDO, COMPLETADO};
enum galgaState {LOAD, TARE};
void stMachine(void);


#endif // STATEMACHINE_H
