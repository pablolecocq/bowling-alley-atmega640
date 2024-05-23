/*
 * CFile1.c
 *
 * Created: 10/05/2024 8:35:37
 *  Author: pablo
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "displays.h"
#include "Retorno.h"
#include "Timer0.h"
#include "macros.h"
long int t_disp, t_fin_bolos;
// parte retorno

extern int tantirrebotes6;
extern int flag_sw6;
extern int flag_sw5;
extern int flag_sw1;
extern int flag_sw6_correcto;

extern int t_espera_sw5;
extern int t_carga_bola;

int cont_ms = 0;
int tiempo_juego;

int est_parp;
int t_parpadeo;
int flag_ult_tirada=0;
int flag_parpadeo = 0;
int t_LED = 0;

void setup_timer0()
{
	cli();
	
	TCCR0A |= (1 << WGM01);					// Echo a la derecha un 1 respecto de WGM01 -- MODO CTC
	TCCR0B |= (1 << CS02) |(0 << CS01) | (1 << CS00);		// Preescalado de 1024; 
	OCR0A = 78 - 1;		// Cuenta 125 ciclos; Ints cada 10ms
	TIMSK0 |= (1 << OCIE0A);					// Interrupciones por comparacion (máscara)
	TIFR0 |= (1 << OCF0A);					// Bajar banderas
	
	
	sei();
		tantirrebotes6 = 0;
		t_espera_sw5 = 0;
		t_carga_bola = 0;
}

void tiempo_juego_cero(){
	tiempo_juego =0; //cuando llamo desde main, se pone a cero el tiempo de juego
	
}

void tiempo_fin_bolos(){
	t_fin_bolos =0; //cuando llamo a esta función desde el main, se pone a cero el tiempo de contar bolos(los habilita)
	
}
int ultima_tirada(){
    
    return flag_ult_tirada;//devuelve esta variable al main para comprobar si han pasado 30seg
}
void parp_disp(){
	est_parp=1;  // cuando llamo a esta funcion desde el main, en isr cambia el comportamiento de los displays, parpadean
}

ISR(TIMER0_COMPA_vect) {
	cont_ms++;              //suma 1 ms
	t_disp ++; //Tiempo para ir cambiando los displays no se usa
	t_fin_bolos++; // Tiempo para no contar más bolos
	t_parpadeo++; // parpadeo de display
	tiempo_juego++;
    
	if (flag_parpadeo == 1)
	{
		t_LED ++;
		if ( t_LED < 90 ) PORT_LED |= (1 << LED);
		if ( (t_LED > 90) & (t_LED < 100)) PORT_LED &= ~(1 << LED);
		if ( t_LED ==  100 ) t_LED = 0;
	}
	
	
	if(est_parp==1){
		if(t_parpadeo < 90){
			selector();
			//PORT_LED |= (1 << LED);// LED encendido 900ms
		}
		else{
			if(t_parpadeo < 100){
				PORTD |= (1 << PD7);
 		        PORTD &= ~((1 << PD4) | (1 << PD5) | (1 << PD6));
		        PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3));// pongo a cero el display

				
				//PORT_LED &= ~(1 << LED);//LED apagado 100ms
		    }
			else{
				t_parpadeo=0;
			}
		}
    }
 	else{
		selector();
    }
	
    // parte retorno

    if( flag_sw6 == 1 ){
		    tantirrebotes6++;
	    }
	
    if( flag_sw5 == 1 ){
		    t_espera_sw5++;
    }
	
    if( flag_sw1 == 1 ){
		    t_carga_bola++;
		    }
	//ultima tirada
    if(tiempo_juego>3000){
        flag_ult_tirada=1;
    }
}

