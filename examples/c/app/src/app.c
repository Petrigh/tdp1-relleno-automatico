#include "app.h"
#include "helper.h"
#include "stateMachine.h"

void onTimer(void *param);

int main(void) {
   //Inicializar la placa
   boardConfig();
   initialize();
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
