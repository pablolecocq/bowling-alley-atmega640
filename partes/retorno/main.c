#include <avr/io.h>
#include <avr/interrupt.h>
#include "Retorno.h"
#include "funciones_lanzador.h"
#include "macros.h"
#include "Timer_0.h"


//DECLARACIÓN DE VARIABLES
int bolaslistas = 3;			// Numero de bolas en la rampa
int flag_M2 = 0;				// Bandera que nos pasa el grupo lanzador avisando que esta el motor 2 a la derecha
int flagdisplayacabado = 1;		// Bandera que nos pasa el grupo displays para saber que la bola ya esta en posición de retorno


typedef enum {START,CARGAR_BOLA,PREPARAR_CARGA,RETORNO} States_bowling;
States_bowling estado=START;

int main(void) {
	setup_retorno();
	setup_PWM();
	setup_timer();
	while (1) {
		switch(estado){
			case START:
			while(!SW6_pulsado());
			estado = CARGAR_BOLA;
			break;

			case CARGAR_BOLA:
			if (start()==1){
				flag_M2 = 1;
				estado = PREPARAR_CARGA;
			}
			break;

			case PREPARAR_CARGA:
			if( flag_M2 == 1 ){
				carga();
				bolaslistas--;
				estado = RETORNO;
			}
			break;

			// 			case DISPARAR:
			// 			if( SW6_pulsado == 1 ){
			// 				disparo();
			// 				estado = RETORNO;
			// 			}
			// 			break;

			case RETORNO:
			if( flagdisplayacabado == 1 ){
				flagdisplayacabado = 0;
				retorno();
				bolaslistas++; // Aumento el número de bolas en la posición de carga
				estado = START;
			}

			break;

		}
	}
}

// typedef enum {START,CARGAR_BOLA} States_bowling;
// States_bowling estado=START;
// int main(void)
// {
//     setup_retorno();
//     setup_timer();
//     setup_PWM();
//
//     while (1)
//     {
//         switch(estado){
//             case START:
//             if (start()==1){
//                 estado = CARGAR_BOLA;
//             }
//
//             break;
//             case CARGAR_BOLA:
//             break;
//         }
//         //bajaM1();
//         //subeM5();
//         //start();
//
//         //carga();
//         //retorno();
//     }
// }



// int main(void)
// {
//     setup_retorno();
//     setup_timer();
//     setup_PWM();
//
//     //carga();
//     //retorno();
// }
