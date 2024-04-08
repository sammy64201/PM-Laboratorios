/*
 * Prelab_04.c
 *
 * Created: 4/7/2024 10:02:41 PM
 * Author : samue
 */ 

#include <avr/io.h>
#include <util/delay.h>

uint8_t contador = 0; // Variable para almacenar el valor del contador


void setup(){
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
}

int main(void) {
	setup();

	while (1) {
		// Verificar el boton de suma 
		if (!(PINC & (1 << PC2))) {
			
			// antirebote
			_delay_ms(1500); 
			
			// verificar nuevamente si sigue presionado
			if (!(PINC & (1 << PC2))) { 
				if (contador < 255) 
				contador++;
				else
				contador = 0; 
				LEDS();
			}
		}

		// Verifica si se presiona el botón de decremento
		if (!(PINC & (1 << PC3))) {
			
			// antirebote
			_delay_ms(1500); 
			
			// verificar nuevamente si sigue presionado
			if (!(PINC & (1 << PC3))) { 
				if (contador > 0) 
				contador--;
				else
				contador = 255; 
				LEDS();
			}
		}
	}

}