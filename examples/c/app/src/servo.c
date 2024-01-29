#include "servo.h"

void servo(void){
   //Cambiar este while e integrarlo en la maquina de estado
   while(1){
      if(gpioRead(CAN_TD)){
         gpioWrite(GPIO1, ON);
         gpioWrite(GPIO5, OFF);
         pwmWrite(PWM10, 170);
      }else{
         gpioWrite(GPIO1, OFF);
         gpioWrite(GPIO5, ON);
         pwmWrite(PWM10, 0);
      }
   }
}


void initCinta(void){
   pwmConfig(0, PWM_ENABLE);
   pwmInit(PWM10, PWM_ENABLE_OUTPUT);
   pwmWrite(PWM10, 0);
}

void initIRyLED(void){
   gpioInit(CAN_TD , GPIO_INPUT );//IR_DATA
   gpioInit(GPIO1 , GPIO_OUTPUT );//LED AZUL
   gpioInit(GPIO5 , GPIO_OUTPUT );//LED VERDE
   gpioWrite(CAN_TD, ON);
   gpioWrite(GPIO1, ON);
   gpioWrite(GPIO5, OFF);
}