#include <avr/io.h>
#define F_CPU 8000000UL
#include "util/delay.h"

int RisingEdge();
int FallingEdge();

int GetDatoPIN0();

int main(void)
{
	
	DDRB = 0xFF; /* PUERTO B -> OUTPUT */
	DDRC = 0x00; /* PUERTO C -> INPUT  */
	PORTC = 1<<PORTC0; /* Activa Pull-Up */
	
	register char shifted_bits = 0x01;
	char shift_direction = 0xFF;
	char t = 12;
	char pulso = 0;
	
	/* Application */
	while(1)
	{
		/* Lectura de Pulsador */
		t = 12;
		while(t--) { // intenta 12 veces
			if (!pulso) {
				if (FallingEdge()) { // Si se detecta un flanco de bajada, se inicia el pulso
					pulso = 1;
				}
				} else {
				if (RisingEdge()) { // Si se detecta un flanco de subida dentro del pulso, cambia la dirección de desplazamiento
					shift_direction = !shift_direction;
					pulso = 0;
				}
			}
			
			/* El polling se realiza cada 2ms */
			_delay_ms(2);
		}
		
		/*  Desplazamiento para transición 'suave' encendido de LEDs */
		if(shift_direction)
		{
			shifted_bits++;
		}
		else
		{
			shifted_bits--;
		}
		
		/* Retardo de 100ms */
		for (uint32_t i = 80000; i > 0; i--);
		
		shifted_bits %= 8;
		
		/* Desplazamiento */
		PORTB = 1 << shifted_bits; // Escribe en el puerto los bits que se deben prender
		
	}
}

int GetDatoPIN0() {
	return (PINC & (1<<PINC0));
}

int FallingEdge() {
	static uint8_t Count = 0;	// Contador actual
	
	if (GetDatoPIN0() == 0) { // Leo el Dato del PIN y veo si es 0
		Count++;
		} else {
		Count = 0;
	}
	
	if (Count == 12) {  // Si por 12 veces seguidas leí 0, entonces es un flanco de bajada
		Count = 0;
		return 1;
	}
	return 0;
}

int RisingEdge () {
	static uint8_t Count = 0;	// Contador actual
	
	if (GetDatoPIN0() == 1) { // Leo el Dato del PIN y veo si es 1
		Count++;
		} else {
		Count = 0;
	}
	
	if (Count == 12) {  // Si por 12 veces seguidas leí 1, entonces es un flanco de subida
		Count = 0;
		return 1;
	}
	return 0;
}