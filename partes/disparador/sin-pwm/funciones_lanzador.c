#include <avr/io.h>
#include <avr/interrupt.h>

#include "funciones_lanzador.h"
#include "macros.h"

int dir_M2;
int cont_sw2;			// Contador de pasos por SW2
int flag_sw2 = 0;		// Bandera cambio de sentido motor 2
int flag_sw2_debounced;

//Setup de entradas, salidas e interrupciones
void setup_puertos(){   
	              
	// MOTOR 1 (SALIDAS)
	DDR_M1_EN |= (1 << M1_EN);
	DDR_M1_DI |= (1 << M1_DI);
	
	// MOTOR 2 (SALIDAS)
	DDR_M2_EN |= (1 << M2_EN);
	DDR_M2_DI |= (1 << M2_DI);
	DDR_M2_BK |= (1 << M2_BK);
	
	// MOTOR 3 (SALIDAS)
	DDR_M3_EN |= (1 << M3_EN);
	DDR_M3_DI |= (1 << M3_DI);
	
	// MOTOR 4 (SALIDAS)
	DDR_M4_EN |= (1 << M4_EN);
	DDR_M4_DI |= (1 << M4_DI);
	
	// MOTOR 5 (SALIDAS)
	DDR_M5_EN |= (1 << M5_EN);
	DDR_M5_DI |= (1 << M5_DI);	
	
	// LED (SALIDA) y SW (ENTRADAS)
	DDR_LED |= (1 << LED);
	
	DDR_SW1 |= (0 << SW1); 
	DDR_SW2 |= (0 << SW2);
	DDR_SW3 |= (0 << SW3);
	DDR_SW4 |= (0 << SW4);
	DDR_SW5 |= (0 << SW5);
	DDR_SW6 |= (0 << SW6);
	
	//INTERRUPCIONES
	
	cli();
	
	//Interrupción INT1 por flanco de bajada en SW2 
	EICRA |= (1 << ISC11);
	EIMSK |= (1 << INT1);
	
	sei();
	
}

void setup_pwm(){
	cli();
	TCCR5A |= (1 << COM5A1) | (1 << COM5B1) | (1 << COM5C1) | (1 << WGM51) | (1 << WGM50); 				// Modo Fast PWM + OCR5A + OCR5B + OCR5C
	TCCR5B |= (1 << WGM53) | (1 << WGM52) | (1 << CS50); 												// Preescalado de 1
	sei();
}

//CONTROL_MOTOR: controla los 5 motores pasandole los parametros
void control_motor(int n_motor, int power, int direccion, int brake){
	switch (n_motor) {
		
		// Modificar motor 1
		case 1: 
			switch (direccion){
				case 0:
				PORT_M1_DI &= ~(1 << M1_DI); break;  //Abajo
				case 1:
				PORT_M1_DI |= (1 << M1_DI); break;   //Arriba
			}
			switch (power){
				case 0:
				PORT_M1_EN &= ~(1 << M1_EN); break;  //Apagado
				case 1:
				PORT_M1_EN |= (1 << M1_EN); break;   //Encendido
			}
			
		break;
		
		// Modificar motor 2
		case 2: 
		
			switch (direccion){
				case 0:
				PORT_M2_DI &= ~(1 << M2_DI); break; //Izquierda
				case 1:
				PORT_M2_DI |= (1 << M2_DI); break;  //Derecha
			}
			switch (power){
				case 0:
				PORT_M2_EN &= ~(1 << M2_EN); break; //Apagado
				case 1:
				OCR5B = FREQ * DUTY_CYCLE; break;	//Encendido
			}
			switch (brake){
				case 0:
				PORT_M2_BK &= ~(1 << M2_BK); break; //Freno apagado
				case 1:
				PORT_M2_BK |= (1 << M2_BK); break;	//Freno encendido
			}
			
		break;
		
		// Modificar motor 3
		case 3: 
			
			switch (direccion){
				case 0:
				PORT_M3_DI &= ~(1 << M3_DI); break;  //Atrás
				case 1:
				PORT_M3_DI |= (1 << M3_DI); break;	 //Adelante
			}
			switch (power){
				case 0:
				PORT_M3_EN &= ~(1 << M3_EN); break;  //Apagado
				case 1:
				OCR5C = FREQ * DUTY_CYCLE; break;   //Encendido
			}
			
		break;
		
		// Modificar motor 4
		case 4: 
			
			switch (direccion){
				case 0:
				PORT_M4_DI &= ~(1 << M4_DI); break;	//Liberar
				case 1:
				PORT_M4_DI |= (1 << M4_DI); break;	//Retener
			}
			switch (power){
				case 0:
				PORT_M4_EN &= ~(1 << M4_EN); break;	//Apagado
				case 1:
				OCR5A = FREQ * DUTY_CYCLE; break;	//Encendido
			}
			
		break;
		
		// Modificar motor 5
		case 5: 
		/*OCR1A = (FREC_ATMEGA640 / DIV_FRECUENCIA) * DUTY_CYCLE;*/
			
			switch (direccion){
				case 0:
				PORT_M5_DI &= ~(1 << M5_DI); break;	//Abajo
				case 1:
				PORT_M5_DI |= (1 << M5_DI); break;	//Arriba
			}
			switch (power){
				case 0:
				PORT_M5_EN &= ~(1 << M5_EN); break;	//Apagado
				case 1:
				PORT_M5_EN |= (1 << M5_EN); break;  //Encendido
			}
			
		break;
		
	}
		
}

//DELAYMS: retardo bloqueante de n milisegundos
void delay_ms(int milis){
	for(int i=0; i<milis;i++){
		for(volatile int j=0;j<444;j++);
	}
}

//HOME DISPARADOR:pongo lanzador en posición de carga (a la derecha)
void home_lanzador(){
	cont_sw2 = 0;
	if (1){			// Tenemos que poner bandera que nos pasa el grupo del cargador de las bolas a 1, de momento pongo un 1, bandera de motor 1 arriba
		control_motor(2,1,0,0);		// Todo a la derecha
		while (cont_sw2 < 2);
		control_motor(2,0,0,0);				// Apago motor 2 al llegar a la derecha
	}
}

//VÁSTAGO: Después de llevar a home el disparador, llevo el vástago y la retención a la posicion que nos interesa
void mov_vastago(){
	control_motor(3,1,1,0); //M3 adelante
	delay_ms(2000);
	control_motor(4,1,1,0); //M4 adelante (retener vastago)
	delay_ms(1500);
	control_motor(3,1,0,0); //M3 atras (tensar vastago)
	delay_ms(1000);
}

//Interrupción SW2 para cambiar dirección M2 en funcion GIRO LANZADOR
ISR(INT1_vect){
	if (cont_sw2 > 0){
		dir_M2 = !dir_M2;
		flag_sw2 = 1;
	}
	cont_sw2++;
}

//ANTIRREBOTES SW2:
void antirrebotes_sw2(){
	cli();					//Desactivo interrupciones
	delay_ms(12);			//Espero
	if (DDR_SW2 & ( 1 << SW2)) flag_sw2_debounced = 1;		//Si el botón sigue pulsado bandera de SW2 antirrebotes a 1
	sei();
}

//GIRO LANZADOR: gira el lanzador completamente a la izquierda y oscila en la posición de disparo
void giro_lanzador(){

	cont_sw2 = 0;						//Contador del sensor SW2 para no parar en el punto medio
	control_motor(2,1,0,0);				// Motor 2 a la izquierda
	dir_M2 = 1;
	
	//while (PIN_SW6 & (1 << SW6)){ bandera de otra pareja
	while(1){
		if (flag_sw2 == 1){
			flag_sw2 = 0;					//Bandera de la interrupción SW2 a 0
			antirrebotes_sw2();				//Si se detecta una pulsación en SW2, filtro el posible rebote
			if (flag_sw2_debounced)			//En caso de que la pulsación "sea buena"
			{
				flag_sw2_debounced = 0;		//Bandera del filtro de nuevo a 0
				control_motor(2,0,0,0);		//Paro el motor
				delay_ms(200);				//Espero para no crear sobretensiones en M2
				//dir_M2 = !dir_M2;			//Cambio el sentido al motor
				control_motor(2,1,dir_M2,0);	//Empiezo a girar en el nuevo sentido
				delay_ms(400);					//Espero un tiempo para garantizar un tiempo mínimo de giro en la nueva dirección y superar siempre SW2
			}
		}
		
		PORT_LED |= (1 << LED);		    //LED encendido para avisar de lanzador preparado
		control_motor(2,1,dir_M2,0);
	}
}