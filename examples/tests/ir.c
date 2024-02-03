#include "../c/app/inc/helper.h"

void initIRyLED(void){
   gpioInit(CAN_TD , GPIO_INPUT );//IR_DATA
   gpioInit(GPIO1 , GPIO_OUTPUT );//LED AZUL
   gpioInit(GPIO5 , GPIO_OUTPUT );//LED VERDE
   gpioWrite(CAN_TD, ON);
   gpioWrite(GPIO1, ON);
   gpioWrite(GPIO5, OFF);
}
/*
void ir(void){   
   
   while(1){
      if(gpioRead(CAN_TD)){
         gpioWrite(GPIO1, ON);
         gpioWrite(GPIO5, OFF);
      }else{
         gpioWrite(GPIO1, OFF);
         gpioWrite(GPIO5, ON);
      }
   }
}
*/
