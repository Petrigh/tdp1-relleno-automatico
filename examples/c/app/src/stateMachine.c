#include "stateMachine.h"

//int main(void){
//	boardConfig();
//  initPoncho();
//   
//   while(1){
//      stMachine();
//   }
//   return 0;
//}

uint8_t pinV[4] = {LCD4,LCDRS,LCD3,LCD2}; //Motor paso a paso
uint8_t pinLED[3] = {GPIO1,GPIO3,GPIO5}; //3 LEDS (AZUL, AMARILLO, VERDE)
uint8_t pinButtons[2] = {GPIO7,GPIO8}; //3 botones
int i,system_call_count, suma;
int value = 0;
enum veltState estado;
enum galgaState galestado;


void initPoncho(void){
   system_call_count = 0;
   suma = 0;
   estado = NOHAYCAJA;
   galestado = TARE;
}


void stMachine (void){
   switch(estado){
      case NOHAYCAJA:
         if(gpioRead(CAN_TD)){
            gpioWrite(GPIO5, ON);
            pwmWrite(PWM10, 170);
         }else{
            gpioWrite(GPIO5, OFF);
            pwmWrite(PWM10, 0);
            estado = CONFIGURANDO;
         }
      break;
      case CONFIGURANDO:
         gpioWrite(RS232_RXD , OFF);//SK
         gpioWrite(RS232_TXD , ON);//DT
         while(gpioRead(RS232_TXD) == ON);
         for(i=0;i<25;i++){
            gpioWrite(RS232_RXD , ON);
            gpioWrite(RS232_RXD , OFF);
         }
         gpioWrite(RS232_TXD , ON);
         estado = TRANSICION;
      break;
      case TRANSICION:
         if(gpioRead(RS232_TXD) == OFF){
            switch(galestado){
               case LOAD:
                  estado = LLENANDO;
               break;
               case TARE:
                  estado = TARA;
               break;
         }
      break;
      break;
      case TARA:
         if(system_call_count++ > 3){
            suma /= system_call_count;
            system_call_count = 0;
            value = 0;
            galestado = LOAD;
            estado = TRANSICION;
         } else {
            for(i=0;i<24;i++){ //24 pulsos para carga la data la buffer
               gpioWrite(RS232_RXD , ON);
               value=value<<1;
               value+=gpioRead(RS232_TXD);            
               gpioWrite(RS232_RXD , OFF);
            }
            suma += value;
            gpioWrite(RS232_RXD , ON); //1 pulso mas para elegir el canal
            gpioWrite(RS232_RXD , OFF);
            gpioWrite(RS232_TXD , ON);
            galestado = TARE;
            estado = TRANSICION;
         }
      break;
      case LLENANDO:
         gpioWrite(GPIO3, ON);
         for(i=0;i<4;i++){
            gpioWrite( pinV[i], ON);
            gpioWrite( pinV[((i+1) % 4)], ON);
            delay(2);
            gpioWrite( pinV[i], OFF);
         }
         for(i=0;i<24;i++){ //24 pulsos para carga la data la buffer
               gpioWrite(RS232_RXD , ON);
               value=value<<1;
               value+=gpioRead(RS232_TXD);            
               gpioWrite(RS232_RXD , OFF);
            }
            gpioWrite(RS232_RXD , ON); //1 pulso mas para elegir el canal
            gpioWrite(RS232_RXD , OFF);
            gpioWrite(RS232_TXD , ON);
            if  ((value - suma ) > 1000) {
               galestado = TARE;
               estado = COMPLETADO;
            } else {
               galestado = LOAD;
               estado = TRANSICION;
            }
      break;
      case COMPLETADO:
         gpioWrite(GPIO3, OFF);
         if (gpioRead(CAN_TD)){
            gpioWrite(GPIO1, OFF);
            pwmWrite(PWM10, 0);
            estado = NOHAYCAJA;
         }else{
            gpioWrite(GPIO1, ON);
            pwmWrite(PWM10, 170);
         }
      break;
   }
}
}
/*Estado inicial: NO HAY CAJA
  la condicion de corte es el infrarrojo. El motor PWM activo, prender LED de movimiento.

  Detecta infrarrojo, el sig estado HAY CAJA
  Si el infrarrojo esta prendido, apagas el PWM.

  Sig estado, LLENANDO
  Se activa el motor paso a paso mientras se mide con la celda de carga

  Sig estado, COMPLETADO
  Frena el motor paso a paso y la celda, prende el motor PWM y y avanza hasta que el infrarrojo detecte que no haya caja


/*
void stMachine(void){
   switch(estado){
      case START:
         if(gpioRead(pinButtons[0]) == OFF){
            gpioWrite( pinLED[2], OFF);
            gpioWrite( pinLED[1], ON);
            estado = HALT;
            j = 0;
         }
         break;
      case HALT:
         delay(10);
         if(++j == 200){
            gpioWrite( pinLED[1], OFF);
            gpioWrite( pinLED[0], ON);
            j = 0;
            estado = RUN;
         }
         break;
      case RUN:
         for(i=0;i<4;i++){
            gpioWrite( pinV[i], ON);
            gpioWrite( pinV[((i+1) % 4)], ON);
            delay(2);
            gpioWrite( pinV[i], OFF);
         }
         if(++j == 200){
            gpioWrite( pinLED[0], OFF);
            gpioWrite( pinLED[1], ON);
            j = 0;
            estado = HALT;
         }
         break;
   }
} */
