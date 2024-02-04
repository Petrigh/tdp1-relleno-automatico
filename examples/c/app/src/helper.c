#include "helper.h"
#define OFFSET 16000000

void initIRyLED(void);
void initGalga(void);
void initCinta(void);
void initTolva(void);
void initButtons(void);
uint32_t value;



uint8_t pinV[4] = {LCD4,LCDRS,LCD3,LCD2}; //Motor paso a paso
int i; //indice loop

//inicializa todos los perifericos
void initialize(void){
	initIRyLED();
	initGalga();
	initCinta();
	initTolva();
   initButtons();
   controlGalga = false;
   pesoAnterior = 0;
   pesoActual = 0;
   uartConfig(UART_USB, 115200);
}

void initIRyLED(void){
   gpioInit(CAN_TD , GPIO_INPUT );//IR_DATA
   gpioInit(GPIO1 , GPIO_OUTPUT );//LED AZUL
   gpioInit(GPIO3 , GPIO_OUTPUT );//LED AMARILLO
   gpioInit(GPIO5 , GPIO_OUTPUT );//LED VERDE
   gpioWrite(CAN_TD, ON);
   gpioWrite(GPIO1, OFF);
   gpioWrite(GPIO3, OFF);
   gpioWrite(GPIO5, OFF);
}

void initGalga(void){
   gpioInit(RS232_RXD , GPIO_OUTPUT );//SK
   gpioInit(RS232_TXD , GPIO_INPUT );//DT
}

void initButtons(void){
   gpioInit(GPIO8, GPIO_INPUT); //Boton Start
   gpioInit(GPIO7, GPIO_INPUT); //Boton Stop
}

void initCinta(void){
   pwmConfig(0, PWM_ENABLE);
}

void initTolva(void){
	gpioInit(LCD4, GPIO_OUTPUT);
	gpioInit(LCDRS, GPIO_OUTPUT);
	gpioInit(LCD3, GPIO_OUTPUT);
	gpioInit(LCD2, GPIO_OUTPUT);
   estadoTolva = E1;
}

void configGalga(void){
	gpioWrite(RS232_RXD , OFF);//SK
	gpioWrite(RS232_TXD , ON);//DT
	while(gpioRead(RS232_TXD) == ON);
	for(i=0;i<25;i++){//Canal A, amplificador de 128
	   gpioWrite(RS232_RXD , ON);
	   gpioWrite(RS232_RXD , OFF);
	}
	gpioWrite(RS232_TXD , ON);
}

uint32_t readGalga(void){
	value = 0;
	for(i=0;i<24;i++){ //24 pulsos para carga la data la buffer
	   gpioWrite(RS232_RXD , ON);
	   value=value<<1;
	   value+=gpioRead(RS232_TXD);
	   gpioWrite(RS232_RXD , OFF);
	}
   gpioWrite(RS232_RXD , ON); //1 pulso mas para elegir el canal
   gpioWrite(RS232_RXD , OFF);
   gpioWrite(RS232_TXD , ON);
   return value - OFFSET;
}

uint32_t promedio(uint32_t* value, int max){
	uint32_t result = 0;
	uint32_t aux;
	for(i=0; i<max; i++){
		aux = value[i];
		if(aux < 0){
			aux = 0;
		}
		result += aux;
	}
   result /= max;
	return result;
}

void pasoTolva(){
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

char* itoa(int value, char* result, int base) {
   // check that the base if valid
   if (base < 2 || base > 36) { *result = '\0'; return result; }

   char* ptr = result, *ptr1 = result, tmp_char;
   int tmp_value;

   do {
      tmp_value = value;
      value /= base;
      *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
   } while ( value );

   // Apply negative sign
   if (tmp_value < 0) *ptr++ = '-';
   *ptr-- = '\0';
   while(ptr1 < ptr) {
      tmp_char = *ptr;
      *ptr--= *ptr1;
      *ptr1++ = tmp_char;
   }
   return result;
}

