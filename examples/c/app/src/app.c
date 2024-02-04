#include "app.h"
#include "helper.h"
#include "stateMachine.h"

void onTimer(void *param);
bool exe= false;
int preescaler = 0;
int tocoStart = 0;
int tocoStop = 0;

int main(void) {
   //Inicializar la placa
   boardConfig();
   initialize();
   //Configuracion de conteo de interrupcion periodica (cada 10 ms)
   tickInit( 2 );
   //Configuracion de llamado de una funcion
   tickCallbackSet ( onTimer,  0);
   while( TRUE ){
      if(exe){
         stMachine();
         exe = false;
      }
   }
   return 0 ;
}

void onTimer(void *param){
   if(preescaler++ > 5){
      exe = true;
      preescaler = 0;
   }
   
   if(gpioRead(GPIO7) == OFF){
      tocoStart++;
      if(tocoStart > 50){
         start();
      }
   }else{
      tocoStart = 0;
   }
   
   if(gpioRead(GPIO8) == OFF){
      tocoStop++;
      if(tocoStop > 50){
         stop();
      }
   }else{
      tocoStop = 0;
   }
   
   if(llenandoTolva){
      switch(estadoTolva){
         case E1:
            gpioWrite(LCD4, ON);
            gpioWrite(LCDRS, OFF);
            gpioWrite(LCD3, OFF);
            gpioWrite(LCD2, OFF);
            estadoTolva = E2;
         break;
         case E2:
            gpioWrite(LCD4, OFF);
            gpioWrite(LCDRS, ON);
            gpioWrite(LCD3, OFF);
            gpioWrite(LCD2, OFF);
            estadoTolva = E3;
         break;
         case E3:
            gpioWrite(LCD4, OFF);
            gpioWrite(LCDRS, OFF);
            gpioWrite(LCD3, ON);
            gpioWrite(LCD2, OFF);
            estadoTolva = E4;
         break;
         case E4:
            gpioWrite(LCD4, OFF);
            gpioWrite(LCDRS, OFF);
            gpioWrite(LCD3, OFF);
            gpioWrite(LCD2, ON);
            estadoTolva = E1;
         break;
      }
   }
}
