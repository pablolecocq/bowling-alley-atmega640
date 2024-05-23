/*
 * Timer_0.h
 *
 * Created: 13/05/2024 13:22:21
 *  Author: Admin
 */ 

#ifndef TIMER_0_H_
#define TIMER_0_H_

int cont_ms;
extern int tantirrebotes6;
extern int flag_sw6;
extern int flag_sw5;
extern int flag_sw1;
extern int flag_sw1_correcto;
extern int t_espera_sw5;
extern int t_carga_bola;

void setup_timer()
{
	cli();
	TCCR0A = (1 << WGM01);					// Echo a la derecha un 1 respecto de WGM01 -- MODO CTC
	TCCR0B = (1 << CS01) | (1 << CS00);		// Preescalado de 64; Tclk = 8 us
	OCR0A = 125 - 1;						// Cuenta 125 ciclos; Ints cada 1000 us
	TIMSK0 = (1 << OCIE0A);					// Interrupciones por comparacion (máscara)
	TIFR0 = (1 << OCF0A);					// Bajar banderas
	sei();
	
	
	cont_ms = 0;
	tantirrebotes6 = 0;
	t_espera_sw5 = 0;
	t_carga_bola = 0;
	
}


ISR ( TIMER0_COMPA_vect ){
	cont_ms++;              //sume 1 ms
	
	if( flag_sw6 == 1 ){
		tantirrebotes6++;
	}
	
	if( flag_sw5 == 1 ){
		t_espera_sw5++;
	}
	
	if( flag_sw1 == 1 ){
		t_carga_bola++;
	}	
}


#endif /* TIMER_0_H_ */