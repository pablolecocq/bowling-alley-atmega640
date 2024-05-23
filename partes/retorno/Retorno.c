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


// Declaración de macros para facilitar el desarrollo y entendimiento del código
// MOTOR 1

#define DDR_M1_EN DDRB
#define DDR_M1_DI DDRL

#define M1_EN DDB6
#define M1_DI DDL0

#define PORT_M1_DI PORTL
#define PORT_M1_EN PORTB

//MOTOR 5
#define DDR_M5_EN DDRB
#define DDR_M5_DI DDRL

#define M5_EN DDB5
#define M5_DI DDL7

#define PORT_M5_DI PORTL
#define PORT_M5_EN PORTB

// BOTONES SW
#define DDR_SW1 DDRD
#define DDR_SW5 DDRB
#define DDR_SW6 DDRK

#define SW1 DDD0
#define SW5 DDB7
#define SW6 DDK0

#define PIN_SW1 PIND
#define PIN_SW5 PINB
#define PIN_SW6 PINK

// PWM
// #define CICLO_TRABAJO 0.4
// #define DIV_FRECUENCIA 4000
// #define FREC_ATMEGA640 8000000UL


//DECLARACIÓN DE VARIABLES
int bolaslistas;
int tantirrebotes6 = 0;
int t_espera_sw5 = 0;
int t_carga_bola = 0;
int flag_sw1;
int flag_sw5;
int flag_sw6;
int flag_sw6_correcto;


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
	
	if ( (PIND & (1 << PD0 )) == 0 ){
        control_motor(1,1,1,0);
		delay_ms(100);
        control_motor(1,0,1,0);
		delay_ms(100);
		if ((PIND & (1 << PD0 )) == 0){
            control_motor(1,0,1,0);
		}
		else {
			control_motor(1,1,1,0);
			
		}
	}
	else {
		control_motor(1,1,1,0);
	}

	//Lógica para el inicio del motor 5
	
	if ( (PINB & (1 << PB7 )) == 0 ){
        control_motor(5,1,0,0);
		delay_ms(300);
        control_motor(5,0,0,0);
		delay_ms(100);
		if ((PINB & (1 << PB7 )) == 0){
            control_motor(5,0,0,0);
		}
		else {
			control_motor(5,1,0,0);
            while ( t_espera_sw5 < 5300 );
            t_espera_sw5 = 0;
            control_motor(5,0,0,0);
		}
	}
	else {
		control_motor(5,1,0,0);
        while ( t_espera_sw5 < 5300 );
        t_espera_sw5 = 0;
        control_motor(5,0,0,0);
	}
	flag_sw1 = 0;
	flag_sw5 = 0;
    return 1;
}

int SW6_pulsado(){
	if(flag_sw6_correcto == 1){
		flag_sw6_correcto = 0;
		return 1;
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

void carga() {
	bajaM1();
	while ( t_carga_bola < 2000 );
    t_carga_bola = 0;
	subeM1();
    if(flag_sw1 == 1){
        flag_sw1 = 0;
    }
}


void retorno () {
	subeM5();
	while ( t_espera_sw5 < 5300 );
     t_espera_sw5 = 0;
     bajaM5();  
     while ( t_espera_sw5 < 5300 );
     t_espera_sw5 = 0;
     control_motor(5,0,0,0);
     if(flag_sw5 == 1){
         flag_sw5 = 0;
     }
}


//Interrupción del motor M5

ISR ( PCINT0_vect ) { 
	flag_sw5 = 1;
}

// Interrupción del motor M1

ISR ( INT0_vect ) { 
		flag_sw1 = 1;
        control_motor(1,0,1,0);
}

// Interrupción del SW6

ISR ( PCINT2_vect ) { 
	flag_sw6 = 1;
	if( (tantirrebotes6 > 20) & (flag_sw6 == 1)){
		tantirrebotes6 = 0;
		flag_sw6_correcto = 1;
		flag_sw6 = 0;
	}
}