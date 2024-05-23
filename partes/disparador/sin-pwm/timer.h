#ifndef TIMER_H_
#define TIMER_H_

void setup_timer()
{
	cli();
	TCCR0A = (1 << WGM01);					// Echo a la derecha un 1 respecto de WGM01 -- MODO CTC
	TCCR0B = (1 << CS01) | (1 << CS00);		// Preescalado de 64; Tclk = 8 us
	OCR0A = 125 - 1;						// Cuenta 125 ciclos; Ints cada 1000 us
	TIMSK0 = (1 << OCIE0A);					// Interrupciones por comparacion (máscara)
	TIFR0 = (1 << OCF0A);					// Bajar banderas
	sei();
}
 
 int cont_ms = 0;
 int t_LED = 0;
 int tiempo_juego = 0;
 int flag_parpadeo = 1;
 
ISR(TIMER0_COMPA_vect){
	cont_ms++;              //sume 1 ms
	tiempo_juego++;
	
	if (flag_parpadeo == 1)
	{
		t_LED ++;
		if ( t_LED < 900 ) PORT_LED |= (1 << LED);
		if ( (t_LED > 900) & (t_LED < 1000)) PORT_LED &= ~(1 << LED);
		if ( t_LED ==  1000 ) t_LED = 0;
	}

	
}


#endif /* TIMER_H_ */