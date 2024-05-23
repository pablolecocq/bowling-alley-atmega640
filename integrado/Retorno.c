/*
 * Retorno.c
 *
 * Created: 13/05/2024 13:21:12
 *  Author: Admin
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Retorno.h"
#include "funciones_lanzador.h"
#include "macros.h"
#include "Timer0.h"
#include "displays.h"


// DECLARACIÓN DE VARIABLES 
 int bolaslistas;
 int tantirrebotes6 = 0;
 int t_espera_sw5 = 0;
 int t_carga_bola = 0;
 int flag_sw1 = 0;
 int flag_sw5 = 0;
 int flag_sw6 = 0;
 int flag_sw6_correcto = 0;

 


void setup_retorno(){
	
	// SETUP DE LAS ENTRADAS Y SALIDAS
	DDR_M1_EN |= (1 << M1_EN); //Establece pin como salida
	DDR_M5_EN |= (1 << M5_EN); //Establece pin como salida
	DDR_M1_DI |= (1 << M1_DI); //Establece pin como salida
	DDR_M5_DI |= (1 << M5_DI); //Establece pin como salida
    
    DDR_SW1 &= ~(1 << SW1); //Configura SW5 como entrada
    DDR_SW5 &= ~(1 << SW5); //Configura SW5 como entrada
    DDR_SW6 &= ~(1 << SW6); //Configura SW5 como entrada
    PORTD |= (1 << PD0);  //Resistencia pull-up interna
	
	// SETUP DE LAS INTERRUPCIONES
	cli();
	// SW1
	EICRA |= (1 << ISC01); //Flanco de bajada para la INT0
	EIMSK |= (1 << INT0); //Habilita INT0
	EIFR  |= (1 << INT0); //Limpia bandera de INT0
	//SW5
	PCICR |= (1 << PCIE0); //Habilita interrupciones de cambio de pin para PCIE0 (PCINT7)
	PCMSK0 |= (1 << PCINT7); //Máscara para PCINT7
	//SW6
	PCICR |= (1 << PCIE2);  //Habilita interrupciones de cambio de pin para PCIE2 (PCINT16)
	PCMSK2 |= (1 << PCINT16); //Máscara para PCINT16
	sei();
}

void setup_PWM() {
	cli();
	TCCR1A |= ( 1 << COM1A1 ) | ( 1 << COM1B1 ) | ( 1 << WGM10 ); 	// Modo Fast PWM (8 bits) + OCR1A + OCR1B
	TCCR1B |= ( 1 << WGM12 ) | ( 1 << CS10 ) | ( 1 << CS12 ); 		// Preescalado de 1024 (8Mhz/1024*30 ~= 260)	
	//TIMSK1 |= ( 1 << OCIE1A ) | ( 1 << OCIE1B );										
	sei();
}

int start() {
	//Lógica para el inicio del motor 1
	
	if ( (PIND & (1 << PD0 )) == 0 ){                 //Comprobamos si está pulsado el PIN PD0, si lo está entra
        control_motor(1,1,1,0);                       //Sube el motor durante unos ms para salir del fin de carrera
		delay_ms(100);
        control_motor(1,0,1,0);                       //Tras salir,para
		delay_ms(100);
		if ((PIND & (1 << PD0 )) == 0){               //Volvemos a comprobar si está tocando
            control_motor(1,0,1,0);                   //Como lo está, el motor está arriba, asique apagate
		}
		else {
			control_motor(1,1,1,0);                   //No lo está por lo que estaba abajo, asique sube
			
		}
	}
	else {
		control_motor(1,1,1,0);                      //Desde un inicio no estaba pulsando, estaba en medio, sube
	}

	//Lógica para el inicio del motor 5
	
	if ( (PINB & (1 << PB7 )) == 0 ){               //Análogo para el motor 5 pero en lugar de subir, debe bajar.
        control_motor(5,1,0,0);
		delay_ms(300);
        control_motor(5,0,0,0);
		delay_ms(100);
		if ((PINB & (1 << PB7 )) == 0){
            control_motor(5,0,0,0);
		}
		else {
			control_motor(5,1,0,0);
            while ( t_espera_sw5 < 400 );
            t_espera_sw5 = 0;
            control_motor(5,0,0,0);
		}
	}
	else {
		control_motor(5,1,0,0);
        while ( t_espera_sw5 < 400 );
        t_espera_sw5 = 0;
        control_motor(5,0,0,0);
	}
	flag_sw1 = 0;                               //Ponemos las flags a 0 para no interferir durante el resto del programa
	flag_sw5 = 0;
    return 1;                                   // Devuelve un 1 para avisar al main que ya se ha acabado el inicio de los motores 1 y 5
}

int SW6_pulsado(){
	if(flag_sw6_correcto == 1){                 //Si ha comprobado que se ha pulsado correctamente el boton ponlo a 0
		flag_sw6_correcto = 0;
		return 1;                               //Devuelve un 1 para manejarlo en el main
	} else {
		return 0;
	}
}

void subeM1() {
	control_motor(1,1,1,0);
}

void bajaM1() {
	control_motor(1,1,0,0);	
}

void subeM5() {	
	control_motor(5,1,1,0);
}

void bajaM5() {
	control_motor(5,1,0,0);
}

int carga() {
	bajaM1();                            //Baja el motor 1
	delay_ms(2000);                      //Esperamos a que cargue la bola
	flag_sw1 = 0;
    subeM1();                            //Sube el motor 1
	delay_ms(2000);                      //Esperamos para luego apagar el motor 
	
	control_motor(1,0,0,0);              //Apagamos el motor 1 para que no haya sobretensiones
	
    if(flag_sw1 == 1){
        flag_sw1 = 0;
		t_carga_bola = 0;
    }
	
	//control_motor(1,0,0,0);  //Paro motor 1
	return 1;                            //Devuelve un 1 para avisar que está arriba y puede ser lanzada la bola (manejo en main)
}


int retorno () {
	subeM5();                            //Sube el ascensor
	while ( t_espera_sw5 < 530 );        //Esperamos 5300 ms para que el ascensor llegue a la posición de arriba y recoja la bola 
     t_espera_sw5 = 0;   
     bajaM5();                           // Baja el ascensor
     while ( t_espera_sw5 < 530 );       //Esperamos a que baje del todo para luego parar el motor
     t_espera_sw5 = 0;
     control_motor(5,0,0,0);             // Paramos el motor para que no haya sobretensiones
     if(flag_sw5 == 1){     
         flag_sw5 = 0;                   //Ponemos a 0 la bandera para no interferir en t_espera_sw5
     }
     return 1;
}

//Interrupción del motor M5

ISR ( PCINT0_vect ) { 
	flag_sw5 = 1;           //Interrupción que activa una bandera que luego se utilizará en el timer
}

// Interrupción del motor M1

ISR ( INT0_vect ) { 
		flag_sw1 = 1;       //Interrupción que activa una bandera que luego se utilizará en el timer
}

