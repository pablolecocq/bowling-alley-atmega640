/*
 * CFile1.c
 *
 * Created: 10/05/2024 8:35:37
 *  Author: pablo
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

long int t_disp, t_fin_bolos;

int cont_ms = 0;
int tiempo_parpadeo_display;
int tiempo_juego;

int est_parp;
int t_parpadeo;
int flag_ult_tirada=0;

void setup_timer()
{
	cli();
	
	TCCR0A |= (1 << WGM01);					// Echo a la derecha un 1 respecto de WGM01 -- MODO CTC
	TCCR0B |= (1 << CS02) |(0 << CS01) | (1 << CS00);		// Preescalado de 1024; 
	OCR0A = 78 - 1;		// Cuenta 78 ciclos; Ints cada 10ms
	TIMSK0 |= (1 << OCIE0A);					// Interrupciones por comparacion (máscara)
	TIFR0 |= (1 << OCF0A);					// Bajar banderas
	
	
	sei();

}


int dummy;
ISR(TIMER0_COMPA_vect){
	cont_ms++;              //sume 1 ms
	t_disp ++; //Tiempo para ir cambiando los displays
	t_fin_bolos++; // Tiempo para no contar más bolos
	t_parpadeo++;
	tiempo_juego++;

	
	if(est_parp==1){

		if(t_parpadeo < 90){
			selector();
		}
		else{
			if(t_parpadeo < 100){
				PORTD |= (1 << PD7);
		        PORTD &= ~((1 << PD4) | (1 << PD5) | (1 << PD6));
		        PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3));// pongo a cero el display
		    }
			else{
				t_parpadeo=0;
			}
		}
    }
	else{
		selector();
	}
	
	if(tiempo_juego>3000){
		flag_ult_tirada=1;
	}
	
	dummy++;// quitar es para prueba de cuanto tarda 10.021,00 desde que entra en isr  hasta 10.044,50(incluye todo display)
	
}



void tiempo_juego_cero(){
	tiempo_juego =0;
	
}

void tiempo_fin_bolos(){
	t_fin_bolos =0;
	
}

int ultima_tirada(){
	return flag_ult_tirada;
}

void parp_disp(){
	est_parp=1;  // cuando llamo a esta funcion desde el main, en isr cambia el comportamiento de los displays, parpadean
}