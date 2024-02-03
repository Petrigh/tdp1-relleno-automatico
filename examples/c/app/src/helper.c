#include "helper.h"
#define OFFSET 10000

void initIRyLED(void);
void initGalga(void);
void initCinta(void);
void initTolva(void);
uint32_t value;
static char buffer[10];


uint8_t pinV[4] = {LCD4,LCDRS,LCD3,LCD2}; //Motor paso a paso
int i; //indice loop

//inicializa todos los perifericos
void initialize(void){
	initIRyLED();
	initGalga();
	initCinta();
	initTolva();
	uartConfig( UART_USB, 115200 );
}

void initIRyLED(void){
   gpioInit(CAN_TD , GPIO_INPUT );//IR_DATA
   gpioInit(GPIO1 , GPIO_OUTPUT );//LED AZUL
   gpioInit(GPIO3 , GPIO_OUTPUT );//LED AMARILLO
   gpioInit(GPIO5 , GPIO_OUTPUT );//LED VERDE
   gpioWrite(CAN_TD, ON);
   gpioWrite(GPIO1, ON);
   gpioWrite(GPIO3, OFF);
   gpioWrite(GPIO5, OFF);
}

void initGalga(void){
   gpioInit(RS232_RXD , GPIO_OUTPUT );//SK
   gpioInit(RS232_TXD , GPIO_INPUT );//DT
   gpioInit(GPIO7, GPIO_INPUT);
}

void initCinta(void){
   pwmConfig(0, PWM_ENABLE);
   pwmInit(PWM10, PWM_ENABLE_OUTPUT);
   pwmWrite(PWM10, 0);
}

void initTolva(void){
	gpioInit(LCD4, GPIO_OUTPUT);
	gpioInit(LCDRS, GPIO_OUTPUT);
	gpioInit(LCD3, GPIO_OUTPUT);
	gpioInit(LCD2, GPIO_OUTPUT);
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
}

uint32_t promedio(uint32_t* value, int max){
	uint32_t result = 0;
	uint32_t aux;
	for(i=0; i<max; i++){
		aux = value[i] - OFFSET;
      
      uartWriteString( UART_USB, "aux:\r\n" );
      itoa(aux, buffer, 10);
      uartWriteString( UART_USB, buffer );
      uartWriteString( UART_USB, "\r\n" );
		if(aux < 0){
			aux = 0;
		}
		result += aux;
	}
   result/max;
	return result;
}

void pasoTolva(void){
    for(i=0;i<4;i++){
       gpioWrite( pinV[i], ON);
       gpioWrite( pinV[((i+1) % 4)], ON);
       delay(2);
       gpioWrite( pinV[i], OFF);
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

void printNums(uint32_t promedio, char* buffer, uint32_t* result){
   uartWriteString( UART_USB, "Tara:\r\n" );
   for(i=0;i<10;i++){
      itoa(result[i], buffer, 10);
      uartWriteString( UART_USB, buffer );
      uartWriteString( UART_USB, "\r\n" );
   }
   uartWriteString( UART_USB, "promedio:\r\n" );
   itoa(promedio, buffer, 10);
   uartWriteString( UART_USB, buffer );
   uartWriteString( UART_USB, "\r\n" );
}
