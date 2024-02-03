#include "stateMachine.h"
#include "helper.h"

uint8_t pinLED[3] = {GPIO1,GPIO3,GPIO5}; //3 LEDS (AZUL, AMARILLO, VERDE)
uint8_t pinButtons[2] = {GPIO7,GPIO8}; //3 botones
int i,system_call_count;
int32_t medidas[10];
int32_t average;
static int indexTolva = 0;
enum veltState estado = START;
enum galgaState galestado;
char bufferMachine[10];


void initRoutine(void){
   system_call_count = 0;
   estado = NOHAYCAJA;
   galestado = TARE;
}


void stMachine (void){
   switch(estado){
   	  case START:
   		initRoutine();
	  break;
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
		 configGalga();
		 estado = TRANSICION;
		 galestado = TARE;
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
		 }
	  break;
	  case TARA:
		if(system_call_count++ > 8){
			average = promedio(medidas,system_call_count);
			system_call_count = 0;
			galestado = LOAD;
			estado = TRANSICION;
		} else {
			medidas[system_call_count] = readGalga();
			galestado = TARE;
			estado = TRANSICION;
		 }
	  break;
	  case LLENANDO:
		gpioWrite(GPIO3, ON);
		if(indexTolva > 3500){
			indexTolva = 0;
		}
		pasoTolva(indexTolva++);
		medidas[0] = readGalga();
		if  ((medidas[0] - average ) > 15000) {
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
