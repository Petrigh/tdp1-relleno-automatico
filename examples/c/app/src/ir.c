#include "ir.h"

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
