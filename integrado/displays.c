/*
 * CFile1.c
 *
 * Created: 10/05/2024 8:31:15
 *  Author: pablo
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

#include "Retorno.h"
#include "Timer0.h"

/*static volatile*/ unsigned int bolos, unit, dec, s1, s2, s3, s4, s5, s6, sel;
//long int t_disp, t_fin_bolos;

extern int tantirrebotes6;
extern int flag_sw6;
extern int flag_sw5;
extern int flag_sw1;
extern int flag_sw6_correcto;

extern int t_espera_sw5;
extern int t_carga_bola;



void setup_bolos(){
	cli();
	// Configurar PK2-PK7 como entradas
	DDRK &= ~((1 << PK2) | (1 << PK3) | (1 << PK4) | (1 << PK5) | (1 << PK6) | (1 << PK7));
	// Habilitar pull-up interno si es necesario
	PORTK |= (1 << PK2) | (1 << PK3) | (1 << PK4) | (1 << PK5) | (1 << PK6) | (1 << PK7);
	// Habilitar interrupciones para PK2-PK7
	PCMSK2 |= (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21) | (1 << PCINT22) | (1 << PCINT23);
	// Habilitar el grupo de interrupción PCINT2
	PCICR |= (1 << PCIE2);
	DDRB |= 0x0F  ; // mitad de los displays  Da, Db, Dc, Dd
	DDRD |= 0xF0; // la otra mitad          De, Df, Dg, y el selector Ds
	PORTD &= ~((1 << PD4) | (1 << PD5) | (1 << PD6));   //los pongo todos a cero
	PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3));
	sei();
	
	
	bolos=0;// inicializo las variables
	unit=0;
	dec=0;
	s1=0 , s2=0, s3=0, s4=0, s5=0, s6=0;
	sel = 0;
	
}

void sumar(){ //funcion sumar
	if (unit > 9){// si unit es mayor a 9
		unit = 0;// lo pongo a cero
		dec++;// sumo 1 a los decimales
	}
}



ISR(PCINT2_vect){
	
    flag_sw6 = 1;
	if( (tantirrebotes6 > 20) & (PINK & (1 << PK0))){
			tantirrebotes6 = 0;
			flag_sw6_correcto = 1;
			flag_sw6 = 0;
		} 	
	
	
	if(t_fin_bolos > 1000){ // dejamos 10 segundos para contar los bolos
		s1 = 0;   // banderas de los sensores de los bolos. Nos sirven para evitar contar los bolos mas de una vez por tirada
		s2 = 0;   
		s3 = 0;
		s4 = 0;
		s5 = 0;
		s6 = 0;
		
		t_fin_bolos = 0; // esta es la salida para mandar la señal al ascensor.
		
	}
	
	if (((PINK | 0xFB) == 0xFB) && (s1==0) ){// entro si en el pin2 del puerto K hay un cero y la bandera S1 no se ha subido
		s1 = 1; // subo la bandera para no volver a entrar.
		unit++;// sumo 1 a las unidades.
		sumar();// ejecuto la funcion sumar por si estoy en el caso unit=9 
	}
	if (((PINK | 0xF7) == 0xF7) && (s2==0)){// igual pero con el pin3
		
		s2 = 1;
		unit ++;
		sumar();
	}
	if (((PINK | 0xEF) == 0xEF) && (s3==0)){// igual pero con el pin4
		
		s3 = 1;
		unit++;
		sumar();
	}
	if (((PINK | 0xDF) == 0xDF) && (s4==0)){// igual pero con el pin5
		
		s4 = 1;
		unit++;
		sumar();
	}
	if (((PINK | 0xBF) == 0xBF) && (s5==0)){// igual pero con el pin6
		
		s5 = 1;
		unit++;
		sumar();
	}
	if (((PINK | 0x7F) == 0x7F) && (s6==0)){// igual pero con el pin7
		
		s6 = 1;
		unit++;
		sumar();
	}
}


void Display(int digit){
	     	PORTD &= ~((1 << PD4) | (1 << PD5) | (1 << PD6)); // pongo a cero las barras LED conectadas en el puerto D
         	PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3)); // lo mismo con el puerto B
		switch(digit){// según que digito dibujemos en los 7 segmentos entramos en cada caso. 
			case 0: PORTD |= ( (1 << PD4) | (1 << PD5)); // pongo a 1 los segmentos que quiero encender.
			        PORTB |= ( (1 << PB0) | (1 << PB1)| (1 << PB2)| (1 << PB3));
			break;
			case 1:
			        PORTB |= ( (1 << PB1) | (1 << PB2));
			break;
			case 2: PORTD |= ( (1 << PD4) | (1 << PD6));
			        PORTB |= ( (1 << PB0) | (1 << PB1)| (1 << PB3));
			break;
			case 3: PORTD |= ( (1 << PD6) );
			        PORTB |= ( (1 << PB0) | (1 << PB1)| (1 << PB2)| (1 << PB3));
			break;
			case 4: PORTD |= ( (1 << PD5) | (1 << PD6));
			        PORTB |= ( (1 << PB1) | (1 << PB2));
			break;
			case 5: PORTD |= ( (1 << PD5) | (1 << PD6));
			        PORTB |= ( (1 << PB0) | (1 << PB2)| (1 << PB2) | (1 << PB3));
			break;
			case 6: PORTD |= ( (1 << PD4) | (1 << PD5) | (1 << PD6));
			        PORTB |= ( (1 << PB0) | (1 << PB2) | (1 << PB3));
			break;
			case 7: 
			        PORTB |= ( (1 << PB0) | (1 << PB1) | (1 << PB2));
			break;
			case 8: PORTD |= ( (1 << PD4) | (1 << PD5) | (1 << PD6));
			        PORTB |= ( (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3));
			break;
			case 9: PORTD |= ( (1 << PD5) | (1 << PD6));
			        PORTB |= ( (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3));
			break;
		}
}	


void selector(){

	if (sel==1){
		PORTD |= (1 << PD7); // pongo el pin7 del puerto D a 1(display de decenas)
		Display(dec);// envio el valor de dec a la función Display
		sel = 0;// cambio sel para que en la siguiente interrupción temporal entremos en el else de debajo.
	}
	else {
		PORTD &= ~ (1 << PD7);// pongo el pin7 del puerto D a 0(display de unidades)
		Display(unit);        //envio el valor de unit a la función Display
		sel =1;               // cambio sel para entrar en la parte de arriba en la siguiente interrupción.
	}
}