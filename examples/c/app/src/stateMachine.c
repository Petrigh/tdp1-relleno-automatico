#include "stateMachine.h"
#include "helper.h"

uint8_t pinLED[3] = {GPIO1,GPIO3,GPIO5}; //3 LEDS (AZUL, AMARILLO, VERDE)
enum veltState {START, NOHAYCAJA, TARA, CONFIGURANDO, TRANSICION, LLENANDO, COMPLETADO, STOP};
enum veltState estado = STOP;
enum galgaState {LOAD, TARE};
enum galgaState galestado;
int i,system_call_count;
int32_t medidas[10];
int32_t average;
int toogleLED = 0;
int preescalerGalga = 0;
static char buffer[10];


void initRoutine(void){
   system_call_count = 0;
   estado = COMPLETADO;
   galestado = TARE;
   gpioWrite(GPIO1, OFF);
   gpioWrite(GPIO3, OFF);
   gpioWrite(GPIO5, OFF);
   pwmWrite(PWM10, 0);
   llenandoTolva = false;
}


void stMachine (void){
   switch(estado){
	  case NOHAYCAJA:
		 if(gpioRead(CAN_TD)){
			gpioWrite(GPIO5, ON);
          arrancarCinta();
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
      uartWriteString( UART_USB, "medidas::\r\n" );
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
         uartWriteString( UART_USB, "promedio:\r\n" );
         itoa(average, buffer, 10);
         uartWriteString( UART_USB, buffer );
         uartWriteString( UART_USB, "\r\n" );
		} else {
			medidas[system_call_count] = readGalga();
			galestado = TARE;
			estado = TRANSICION;
         itoa(medidas[system_call_count], buffer, 10);
         uartWriteString( UART_USB, buffer );
         uartWriteString( UART_USB, "\r\n" );
		 }
	  break;
	  case LLENANDO:
      llenandoTolva = true;
		gpioWrite(GPIO3, ON);
      medidas[0] = readGalga();
		if  ((medidas[0] - average ) > 1000) {
		   galestado = TARE;
		   estado = COMPLETADO;
         llenandoTolva = false;
		} else {
		   galestado = LOAD;
		   estado = TRANSICION;
		}
      if(preescalerGalga++ > 300){
         controlGalga = true;
         preescalerGalga = 0;
         pesoActual = medidas[0];
         
      }
      if(preescalerGalga > 20){
         itoa(medidas[0] - average, buffer, 10);
         uartWriteString( UART_USB, buffer );
         uartWriteString( UART_USB, "\r\n" );
      }
      if( controlGalga == true) {
         if((pesoAnterior != 0) && ((pesoActual - pesoAnterior) <  100)){
            stop();
         }
		   pesoAnterior = pesoActual;
         controlGalga = false;
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
         arrancarCinta();
		 }
	  break;
     case STOP:
        if(toogleLED++ > 100){
            toogleLED = 0;
            gpioToggle(GPIO1);
            gpioToggle(GPIO3);
            gpioToggle(GPIO5);
         }
     break;
   }
}



void start(void){
   if(estado == START || estado == STOP){
      initRoutine();
   }
}

void stop(void){
   system_call_count = 0;
   estado = STOP;
   galestado = TARE;
   gpioWrite(GPIO1, OFF);
   gpioWrite(GPIO3, OFF);
   gpioWrite(GPIO5, OFF);
   pwmWrite(PWM10, 0);
   llenandoTolva = false;
   controlGalga = false;
   pesoAnterior = 0;
   pesoActual = 0;
   preescalerGalga = 0;
}
