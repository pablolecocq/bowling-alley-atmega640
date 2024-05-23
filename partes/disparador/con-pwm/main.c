#include <avr/io.h>
#include <avr/interrupt.h>

#include "macros.h"
#include "timer.h"
#include "funciones_lanzador.h"



//------------------------------------------------------------------------------------------------//
//                                       MÁQUINA DE ESTADOS                                       //
//------------------------------------------------------------------------------------------------//

// typedef enum {CARGAR_BOLA,HOME_LANZADOR,CARGA_LISTA,VASTAGO,GIRO_LANZADOR} States_bowling;
// States_bowling estado=CARGAR_BOLA;
// 
// //Declaracion de variables
int flag_M1=1;                //si 1 -> M1 arriba, si 0-> M1 abajo
int flag_M4= 0;		  //si 1 -> retencion M4 puesta, si 0-> retencion M4 liberada
// 
// extern int cont_ms;
// 
// 
// int main(void){
// 	
// 	setup_puertos();
// 	setup_pwm();
// 	setup_timer();
// 	
// 	while(1){
// 		
// 		switch(estado){
// 	
// 			case CARGAR_BOLA:
// 				//MOTOR 1 arriba y preparado para carga
// 				estado = HOME_LANZADOR;
// 			break;
// 	
// 			case HOME_LANZADOR:
// 				// MOTOR 2 completamente a la derecha para cargar bola
// 				if(flag_M1==1)home_lanzador();
// 					else {
// 						control_motor(2,1,1,0);
// 						delay_ms(1000);
// 						home_lanzador();
// 					}
// 					estado = CARGA_LISTA;
// 			break;
// 			
// 			case CARGA_LISTA:
// 				//MOTOR 1 abajo, carga bola y sube
// 				estado = VASTAGO;
// 			break;
// 			
// 			case VASTAGO:
// 				//Sistema de retencion y tensado de goma
// 				if(flag_M4==0) mov_vastago();
// 					else {
// 						control_motor(4,1,0,0);
// 						delay_ms(1000);
// 						mov_vastago();
// 					}
// 				estado = GIRO_LANZADOR;
// 			break;
// 	
// 			case GIRO_LANZADOR:
// 				//Lanzador cargado y preparado para disparar
// 				//M2 izquierda del todo y oscila entre punto izquierdo y medio
// 				lanzador_izquierda();
// 				if(get_izquierda_M2()){
// 					while(1)
// 					giro_lanzador();
// 				}
// 				
// 				// RESTO DE ESTADOS
// 
// 		}
// 	}
// }

int main(void){
	setup_puertos();
	setup_pwm();
	setup_timer();
	
	//home_lanzador();			//OK
	//mov_vastago();
	//lanzador_izquierda();		//OK
	//disparo();

	while(1){ 
		//home_lanzador();			//OK
		//mov_vastago();
		//lanzador_izquierda();		//OK
		//giro_lanzador();
		//disparo();
		}
}
