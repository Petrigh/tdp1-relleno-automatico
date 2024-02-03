#include "app.h"

#include "../c/app/inc/helper.h"
#include "servo.h"
#include "hx711.h"
#include "stateMachine.h"

int main(void) {
   //Inicializar la placa
   boardConfig();
   //Inicializa la cinta
   initCinta();
   //Inicializa IR y LEDs azul y verde
   initIRyLED();
   //Inicializa galga
   initGalga();
   //Configuracion de conteo de interrupcion periodica (cada 10 ms)
   tickInit( 10 );
   //Configuracion de llamado de una funcion
   tickCallbackSet ( onTimer,  0);
   while( TRUE ){

   }
   return 0 ;
}

void onTimer(void *param){
   stMachine();
}
