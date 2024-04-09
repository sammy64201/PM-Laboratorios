/*
 * Laboratorio_04.c
 *
 * Created: 4/8/2024 11:13:32 AM
 * Author : samue
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

uint8_t contador = 0; // Variable para almacenar el valor del contador

uint8_t valor_adc = 0x00;
uint8_t dig1 ;
uint8_t dig2 ;
uint8_t disp1 = 0x00;
uint8_t disp2 = 0x00;




void initADC(void);
void setup(){
	cli();
	// DECLARAR PUERTO D COMO SALIDA
	DDRD |= 0xFF;
	PORTD = 0;
	UCSR0B = 0;
	
	// DECLARAR LOS PUERTOS DE LOS TRANSISTORES COMO SALIDA 
	DDRC |= (1 << PC4) | (1 << PC5);
	PORTC &= ~(1 << PC4);
	PORTC &= ~(1 << PC5);
	
	initADC();
	sei();
	
}
void setup_P1(){
	// declarar que son entradas
	DDRC &= ~((1 << PC2) | (1 << PC3));
	PORTC |= (1 << PC2) | (1 << PC3);
	
	// declarar que son salidas
	DDRB |= 0x3F;
	DDRC |= 0x03;
}
void LEDS(){
	// funcion para cargar a los leds el valor del contador
	PORTB = (PORTB & 0xC0) | (contador & 0x3F);
	PORTC = (PORTC & 0xFC) | ((contador >> 6) & 0x03);
	display1();
	PORTD = disp1;
	PORTC |= (1 << PC4);
	_delay_ms(2);
	PORTC &= ~(1 << PC4);
	
	display2();
	PORTD = disp2;
	PORTC |= (1 << PC5);
	_delay_ms(2);
	PORTC &= ~(1 << PC5);	
}
void alarma(){
	
	// COMPARO EL VALOR DEL ADC Y EL CONTADOR
	if (valor_adc > contador){
		// SI ES MAYOR ENCEDER UNA LED DE ALARMA
		PORTD |= (1 << PD7);
	}
	else {
		// DE NO SER ASI APAGARLA 
		PORTD &= ~(1 << PD7);
	}
}


int main(void)
{
	// HACER EL SETUP DE LOS PUERTOS Y ENCENDER LAS INTERRUPCIONES 
	setup_P1();
	setup();
	// DECLARAR VARIABLES QUE SERVIRAN PARA SEPARAR EL VALOR DEL ADC EN 2 DISPLAYS
	dig1 = valor_adc >> 4;
	dig2 = valor_adc & 0x0F;
	
	
    while (1) 
    {
		ADCSRA |= (1<<ADSC);  // INICIAMOS LA SECUENCIA
		LEDS();
		alarma();
		// Verificar el boton de suma
		if (!(PINC & (1 << PC2))) {
			
			// antirebote
			_delay_ms(100);
			
			// verificar nuevamente si sigue presionado
			if (!(PINC & (1 << PC2))) {
				if (contador < 255){
				contador++;
				alarma();
				}
				//alarma();
				
				else
				contador = 0;
				LEDS();
				alarma();
			}
		}

		// Verifica si se presiona el botón de decremento
		if (!(PINC & (1 << PC3))) {
			
			// antirebote
			_delay_ms(100);
			
			// verificar nuevamente si sigue presionado
			if (!(PINC & (1 << PC3))) {
				if (contador > 0){
				contador--;
				alarma();
				}
				else
				contador = 255;
				LEDS();
				alarma();
			}
		}
	}
		
		
		
    }


void initADC(){
	ADMUX = 0; // INICIO EN 0 
	// REFERENCIA AVCC = 5V
	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << REFS1);
	// SELECCIONO EL ADC[5] = 0110
	ADMUX |= (1 << MUX2) | (1 << MUX1);
	
	// JUSTIFICACION A LA IZQUIERDA 
	ADMUX |= (1 << ADLAR);
	
	ADCSRA = 0;
	
	
	// HABILITAR LA INTERRUPCION DEL ADC
	ADCSRA |= (1 << ADIE);
	
	// HABILITAMOS EL PRESCALER 128  FADC = 125 KHz
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	
	// HABILITAMOS EL ADC
	ADCSRA |= (1 << ADEN);
		
}
void display1(){
	
	if (dig1 == 0x00){
		//disp1 = 0x3F;
		disp1 = (1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD2) | (1 << PD1) | (1 << PD0);
		}
	else if (dig1 == 0x01){
		//disp1 = 0x06;
		disp1 = (1 << PD2) | (1 << PD1);
	}
	else if (dig1 == 0x02){
		//disp1 = 0x5B;		
		disp1 =  (1 << PD6) | (1 << PD4) | (1 << PD3) | (1 << PD1) | (1 << PD0);
	}
	else if (dig1 == 0x03){
		//disp1 = 0x4F;
		disp1 =  (1 << PD6) | (1 << PD3) | (1 << PD2) | (1 << PD1) | (1 << PD0);
	}
	else if (dig1 == 0x04){
		//disp1 = 0x66;
		disp1 =  (1 << PD6) | (1 << PD5) | (1 << PD2) | (1 << PD1);
	}
	else if (dig1 == 0x05){
		//disp1 = 0x6D;
		disp1 =  (1 << PD6) | (1 << PD5) | (1 << PD3) | (1 << PD2) | (1 << PD0);
	}
	else if (dig1 == 0x06){
		//disp1 = 0x7D;
		disp1 =  (1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD2) | (1 << PD0);
	}
	else if (dig1 == 0x07){
		//disp1 = 0x07;
		disp1 =  (1 << PD2) | (1 << PD1) | (1 << PD0);
	}
	else if (dig1 == 0x08){
		//disp1 = 0x7F;
		disp1 =  (1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD2) | (1 << PD1) | (1 << PD0);
	}
	else if (dig1 == 0x09){
		//disp1 = 0x6F;
		disp1 =  (1 << PD6) | (1 << PD5) | (1 << PD3) | (1 << PD2) | (1 << PD1) | (1 << PD0);
	}
	else if (dig1 == 0x0A){
		//disp1 = 0x77;
		disp1 =  (1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD2) | (1 << PD1) | (1 << PD0);
	}
	else if (dig1 == 0x0B){
		//disp1 = 0x7F;
		disp1 =  (1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD2) | (1 << PD1) | (1 << PD0);
	}
	else if (dig1 == 0x0C){
		//disp1 = 0x39;
		disp1 =  (1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD0);
	}
	else if (dig1 == 0x0D){
		//disp1 = 0x3F;
		disp1 =  (1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD2) | (1 << PD1) | (1 << PD0);
	}
	else if (dig1 == 0x0E){
		//disp1 = 0x79;
		disp1 =  (1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD0);
	}
	else if (dig1 == 0x0F){
		//disp1 = 0x71;
		disp1 = (1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD0);
	}
}
void display2(){
	
	if (dig2 == 0x00){
		//disp2 = 0x3F;
		disp2 = (1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD2) | (1 << PD1) | (1 << PD0);
		}
	else if (dig2 == 0x01){
		//disp2 = 0x06;
		disp2 = (1 << PD2) | (1 << PD1);
	}
	else if (dig2 == 0x02){
		//disp2 = 0x5B;		
		disp2 =  (1 << PD6) | (1 << PD4) | (1 << PD3) | (1 << PD1) | (1 << PD0);
	}
	else if (dig2 == 0x03){
		//disp2 = 0x4F;
		disp2 =  (1 << PD6) | (1 << PD3) | (1 << PD2) | (1 << PD1) | (1 << PD0);
	}
	else if (dig2 == 0x04){
		//disp2 = 0x66;
		disp2 =  (1 << PD6) | (1 << PD5) | (1 << PD2) | (1 << PD1);
	}
	else if (dig2 == 0x05){
		//disp2 = 0x6D;
		disp2 =  (1 << PD6) | (1 << PD5) | (1 << PD3) | (1 << PD2) | (1 << PD0);
	}
	else if (dig2 == 0x06){
		//disp2 = 0x7D;
		disp2 =  (1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD2) | (1 << PD0);
	}
	else if (dig2 == 0x07){
		//disp2 = 0x07;
		disp2 =  (1 << PD2) | (1 << PD1) | (1 << PD0);
	}
	else if (dig2 == 0x08){
		//disp2 = 0x7F;
		disp2 =  (1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD2) | (1 << PD1) | (1 << PD0);
	}
	else if (dig2 == 0x09){
		//disp2 = 0x6F;
		disp2 =  (1 << PD6) | (1 << PD5) | (1 << PD3) | (1 << PD2) | (1 << PD1) | (1 << PD0);
	}
	else if (dig2 == 0x0A){
		//disp2 = 0x77;
		disp2 =  (1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD2) | (1 << PD1) | (1 << PD0);
	}
	else if (dig2 == 0x0B){
		//disp2 = 0x7F;
		disp2 =  (1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD2) | (1 << PD1) | (1 << PD0);
	}
	else if (dig2 == 0x0C){
		//disp2 = 0x39;
		disp2 =  (1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD0);
	}
	else if (dig2 == 0x0D){
		//disp2 = 0x3F;
		disp2 =  (1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD2) | (1 << PD1) | (1 << PD0);
	}
	else if (dig2 == 0x0E){
		//disp2 = 0x79;
		disp2 =  (1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD0);
	}
	else if (dig2 == 0x0F){
		//disp2 = 0x71;
		disp2 = (1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD0);
	}
}

ISR(ADC_vect){

	valor_adc = ADCH;
	dig1 = valor_adc >> 4;
	dig2 = valor_adc & 0x0F;
	alarma();
	
	// APAGAR LA BANDERA DE INTERRUPCION DEL ADC 
	ADCSRA |= (1 << ADIF);	
	
}

