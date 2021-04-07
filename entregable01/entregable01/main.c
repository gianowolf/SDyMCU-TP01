#include <avr/io.h>
#define F_CPU 16000000UL
#include "util/delay.h"

int main(void)
{
	DDRB = 0xFF; /* PUERTO B -> OUTPUT */
	DDRC = 0x00; /* PUERTO C -> INPUT  */
	PINC = 0x01;
	
	char shifted_bits = 0x01;
	char shift_direction = 0xFF;
	
	while(1)
	{
		/* READ BUTTON */
		if((PINC& (1<<PINC0)) == 0)
		{
			//_delay_ms(200);
			if((PINC& (1 << PINC0)) == 0)
			{
				//_delay_ms(20);
				shift_direction = ~ shift_direction;	
			}
		}
		
		/* SHIFTING */
		if (shift_direction) shifted_bits++;
			else shifted_bits--;

		shifted_bits %= 7;
		
		PORTB = 1 << shifted_bits;	
	}
}

