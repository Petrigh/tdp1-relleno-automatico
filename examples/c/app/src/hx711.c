
// #include "hx711.h"
// #define MSBFIRST 0
// #define LSBFIRST 1

// uint8_t PD_SCK;
// uint8_t DOUT;
// uint8_t GAIN;
// long OFFSET;
// float SCALE;


// uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
//     uint8_t value = 0;

//     for (int i = 0; i < 8; ++i) {
//         gpioWrite(clockPin, ON);

//         if (bitOrder == MSBFIRST) {
//             value |= gpioRead(dataPin) << (7 - i);
//         } else {
//             value |= gpioRead(dataPin) << i;
//         }

//         gpioWrite(clockPin, OFF);
//     }

//     return value;
// }


// void HX711_Init(uint8_t dout, uint8_t pd_sck, uint8_t gain) {
// 	PD_SCK 	= pd_sck;
// 	DOUT 	= dout;

// 	gpioInit(PD_SCK, GPIO_OUTPUT);
// 	gpioInit(DOUT, GPIO_INPUT);
// 	HX711_set_gain(gain);
// 	DOUT = 1;
// }

// bool HX711_is_ready() {
// 	gpioWrite(PD_SCK, OFF);

// 	return gpioRead(DOUT) == OFF;
// }

// void HX711_set_gain(uint8_t gain) {
// 	switch (gain) {
// 		case 128:		// channel A, gain factor 128
// 			GAIN = 1;
// 			break;
// 		case 64:		// channel A, gain factor 64
// 			GAIN = 3;
// 			break;
// 		case 32:		// channel B, gain factor 32
// 			GAIN = 2;
// 			break;
// 	}
// 	   printf("aaaaaaaaaaaaaaay\n");

// 	gpioWrite(PD_SCK, OFF);
// 	// HX711_read();
// }

// long HX711_read() {
// 	// wait for the chip to become ready
//    printf("entrando al ready codigo\n");

//    delayWrite( &delay, 4 );
//    printf("saliendo	 al ready codigo\n");

//     unsigned long value = 0;
//     uint8_t data[3] = { 0 };
//     uint8_t filler = 0x00;

// 	// pulse the clock pin 24 times to read the data
//     data[2] = shiftIn(DOUT, PD_SCK, MSBFIRST);
//     data[1] = shiftIn(DOUT, PD_SCK, MSBFIRST);
//     data[0] = shiftIn(DOUT, PD_SCK, MSBFIRST);

// 	// set the channel and the gain factor for the next reading using the clock pin
// 	for (unsigned int i = 0; i < GAIN; i++) {
// 		gpioWrite(PD_SCK, ON);
// 		gpioWrite(PD_SCK, OFF);
// 	}

//     // Datasheet indicates the value is returned as a two's complement value
//     // Flip all the bits
//     data[2] = ~data[2];
//     data[1] = ~data[1];
//     data[0] = ~data[0];

//     // Replicate the most significant bit to pad out a 32-bit signed integer
//     if ( data[2] & 0x80 ) {
//         filler = 0xFF;
//     } else if ((0x7F == data[2]) && (0xFF == data[1]) && (0xFF == data[0])) {
//         filler = 0xFF;
//     } else {
//         filler = 0x00;
//     }

//     // Construct a 32-bit signed integer
//     value = ( (unsigned long)filler << 24
//                 | (unsigned long)data[2] << 16
//                 | (unsigned long)data[1] << 8
//                 | (unsigned long)data[0] );

//     // ... and add 1
//     return (long)(++value);
// }

// long HX711_read_average(uint8_t times) {
// 	long sum = 0;
// 	for (uint8_t i = 0; i < times; i++) {
// 		sum += HX711_read();
// 	}
// 	return sum / times;
// }

// double HX711_get_value(uint8_t times) {
// 	return HX711_read_average(times) - OFFSET;
// }

// float HX711_get_units(uint8_t times) {
// 	return HX711_get_value(times) / SCALE;
// }

// void HX711_tare(uint8_t times) {
// 	double sum = HX711_read_average(times);
// 	HX711_set_offset(sum);
// }

// void HX711_set_scale(float scale) {
// 	SCALE = scale;
// }

// float HX711_get_scale() {
// 	return SCALE;
// }

// void HX711_set_offset(long offset) {
// 	OFFSET = offset;
// }

// long HX711_get_offset() {
// 	return OFFSET;
// }

// void HX711_power_down() {
// 	gpioWrite(PD_SCK, OFF);
// 	gpioWrite(PD_SCK, ON);
// }

// void HX711_power_up() {
// 	gpioWrite(PD_SCK, OFF);
// }


#include "hx711.h"

int8_t dato = 0;
int32_t value = 0;

int princ(void){
   static char uartBuff[7];
   
   uartConfig( UART_USB, 115200 );
   initGalga();
   
   uartWriteString( UART_USB, "Buenas\r\n" );
   
   while(1){
      galgaMachine(uartBuff);
   }
   return 0;
}

void galgaMachine(char* result){
   switch(estadoG){
      case CONFIGURANDO:
         gpioWrite(RS232_RXD , OFF);//SK
         gpioWrite(RS232_TXD , ON);//DT
         while(gpioRead(RS232_TXD) == ON);
         for(i=0;i<25;i++){
            gpioWrite(RS232_RXD , ON);
            gpioWrite(RS232_RXD , OFF);
         }
         gpioWrite(RS232_TXD , ON);
         estadoG = LISTO;
      break;
      case LEYENDO:
         for(i=0;i<24;i++){ //24 pulsos para carga la data la buffer
            gpioWrite(RS232_RXD , ON);
            value=value<<1;
            value+=gpioRead(RS232_TXD);            
            gpioWrite(RS232_RXD , OFF);
         }
         gpioWrite(RS232_RXD , ON); //1 pulso mas para elegir el canal
         gpioWrite(RS232_RXD , OFF);
         estadoG = ESCRIBIENDO;
      break;
      case LISTO:
         if(gpioRead(RS232_TXD) == OFF){
            estadoG = LEYENDO;
         }         
      break;
      case ESCRIBIENDO:
         if(uartReadByte( UART_USB, &dato )){
               itoa( value, result, 10 ); // base 10 significa decimal
               uartWriteString( UART_USB, result );
               uartWriteString( UART_USB, "\r\n" ); // Enviar un Enter
               
               estadoG = CONFIGURANDO;
         } 
      break;
   }
}

void initGalga(void){
   gpioInit(RS232_RXD , GPIO_OUTPUT );//SK
   gpioInit(RS232_TXD , GPIO_INPUT );//DT
   gpioInit(GPIO7, GPIO_INPUT);
   estadoG = CONFIGURANDO;
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
