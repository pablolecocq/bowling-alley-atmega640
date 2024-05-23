/*
 * bolos_sep.c
 *
 * Created: 10/05/2024 8:30:06
 * Author : pablo
 */ 

#include <avr/io.h>

#include "Timer0.h"
#include "displays.h"
#include "macros.h"
#include "funciones_lanzador.h"
#include "Retorno.h"


extern int flag_sw6_correcto;
extern int flag_parpadeo;

typedef enum {
	state_posicion0,
	state_home,
	state_vastago,
	state_izquierda,
	state_cabeceo,
	state_subir_bola,
	state_disparo,
	state_ascensor,
	
	
	
	state_cabeceo30,
	state_disparo30,
	state_ascensor30,
	state_FIN
} state;

state actual = state_posicion0;

//int dummy;
//int s6;

//int contador2=0;
int main()
{
	setup_timer0();
	setup_bolos();
	setup_puertos();
	setup_pwm();
	setup_PWM();
	setup_retorno();
	

    while(1){
    switch(actual){

//---------------------------------------------------------------------------------------------				
		case state_posicion0:// llevo los motores a posiciones iniciales. ascensor abajo, noria arriba
		        tiempo_juego_cero(); //poner timer juego a cero.
                while (!SW6_pulsado());
				while(!start());
				actual = state_home;
		break;
//---------------------------------------------------------------------------------------------		
		case state_home://llevamos el lanzador a la derecha

		       	home_lanzador();
				while (get_derecha_M2()==0);
				actual = state_subir_bola; 
		
		break;
		
//---------------------------------------------------------------------------------------------			
		case state_subir_bola: //baja y sube m1 para subir una bola
		     carga();                //llamo a funcion subir bola de la parte retorno
			 
		actual = state_vastago;
		break;
//---------------------------------------------------------------------------------------------			
		case state_vastago: //el vastago se mueve para estirar la goma
		          mov_vastago();        // vastago funcion de disparador
			  while(!get_vastago_atras());//cuando llega atras cambiamos de estado
			  actual = state_izquierda;
//---------------------------------------------------------------------------------------------
        case state_izquierda: //brazo a la izquierda
                 
		lanzador_izquierda(); 
	        while (!get_izquierda_M2());
                //verifico si han pasado 30 segundos
                if(ultima_tirada()){ 
                    actual = state_cabeceo30;	//nos vamos a la rama de ultima tirada
                    } 
				 else{   
                     actual = state_cabeceo;    //seguimos en el bucle principal
                     }	
			  
		break;
//---------------------------------------------------------------------------------------------
        case state_cabeceo: // 
            light_LED(1);
            giro_lanzador();
			while (!SW6_pulsado()){ // mientras no se pulse sw6
                giro_lanzador();
            }  
            light_LED(1);              
			disparo();   // dispara el bolo
            while(!get_disparo());
            light_LED(0);
			actual = state_disparo;
			 tiempo_fin_bolos();            // pongo a cero el tiempo de contar bolos antes de cambiar de estado
       
       break;
//---------------------------------------------------------------------------------------------			
		case state_disparo:         // poner a cero el timer de bolos t_fin_bolos, los bolos activos 10segundos
		     
		     actual = state_ascensor;
		break;
//---------------------------------------------------------------------------------------------	
		case state_ascensor: //funcion retorno sube y baja el escensor
		    while(!retorno());
		
		actual = state_home; //
		break;
		
//---------------------------------------------------------------------------------------------	
// caso han pasado 30 seg	

//---------------------------------------------------------------------------------------------
       case state_cabeceo30: // es casi lo mismo que en el bucle principal salvo que el led y los displays parpadean
           flag_parpadeo = 1;
           giro_lanzador();
           while (!SW6_pulsado()){ // mientras no se pulse sw6
           giro_lanzador();
           }
           disparo();   // dispara el bolo
           while(!get_disparo());
           light_LED(0);
        tiempo_fin_bolos();    // t_fin_bolos = 0 se activan los bolos para poder contarlos por última vez
          actual = state_disparo30;
          break;
//---------------------------------------------------------------------------------------------
       case state_disparo30:         // poner a cero el timer de bolos t_fin_bolos
          flag_parpadeo = 0;  // el led parpadea
         
          //espera de 5 sec y nos vamos a state_ascensor
         parp_disp();     // se ponen a parpadear los displays
          actual = state_ascensor30;
          
          break;
//---------------------------------------------------------------------------------------------
      case state_ascensor30: //funcion retorno sube y baja el escensor
          while(!retorno());

          actual = state_FIN; //
          break;

//---------------------------------------------------------------------------------------------
     case state_FIN: //funcion retorno sube y baja el escensor
         while (!SW6_pulsado()){ // cuando se pulse sw6
            
         }
         actual = state_posicion0; // nueva partida
         break;	
	}
	
 }
}


