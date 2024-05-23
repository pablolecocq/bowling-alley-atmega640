/*
 * bolos_sep.c
 *
 * Created: 10/05/2024 8:30:06
 * Author : pablo
 */ 

#include <avr/io.h>

#include "Timer0.h"
#include "displays.h"

typedef enum {
	state_normal,
	state_fin,
	state_nuevo_disparo
	
} state;
state actual;

int cambio=0;
int cambio2;
int apagado=0;
extern int est_parp;
int dummy;
int main()
{
    setup_timer();
	setup_bolos();
	while(1){
	dummy++;
        switch(actual){
	        case state_normal:
	        
	        while(cambio==0);//cambio manualmente para cambiar de estado con watch
	       // tiempo_juego_cero();
	        actual = state_nuevo_disparo;
	        tiempo_fin_bolos();// pongo a cero el tiempo de contar bolos justo antes de cambiar de estado
	        break;
			
			
			case state_nuevo_disparo:
			              //como hemos ejecutado  tiempo_fin_bolos(); antes de entrar en este estado, están habilitados los bolos durante 10 segundos
			 while(cambio2==0);// lo cambio a mano para simular cambio de estado
			 actual = state_fin;
			
			break;
			
	        case state_fin://aqui debe parpadear los displays, 
	        parp_disp();// esta funcion cambia la variable est_parp a 1 para que parpadee el display
	        while(apagado==0)//cambio manualmente para cambiar de estado
	        
	        break;
			
			
	        
        }
}

}