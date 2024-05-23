#include <avr/io.h>
#include <avr/interrupt.h>

#include "funciones_lanzador.h"
#include "macros.h"

//-----------------------------------------------//
//------DECLARACIÓN DE VARIABLES INTERNAS--------//
//-----------------------------------------------//

extern int contador;
int dir_M2 = 0;
int cont_sw2 = 0;			// Contador de pasos por SW2
int contador;

//-----------------------------------------------//
//-------------------BANDERAS--------------------//
//-----------------------------------------------//

int flag_sw2 = 0;		// Bandera final de carrera SW2
volatile int flag_sw3 = 0;		// Bandera final de carrera SW3
int flag_sw4 = 0;		// Bandera final de carrera SW4
int flag_sw2_derecha = 0;	// Bandera posición home M2 (M2 a la derecha del todo)
int flag_sw2_izquierda = 0; //Bandera M2 izquierda
int flag_vastago_atras = 0;
volatile int flag_sw2_giro = 0;
int flag_sw2_debounced = 0; //Bandera del antirrebotes SW2
int flag_sw3_debounced = 0;	//Bandera antirrebotes SW3
int flag_sw4_debounced = 0;	//Bandera antirrebotes SW4
//-----------------------------------------------//
//------------SETUP DE PUERTOS Y PWM-------------//
//-----------------------------------------------//

//Setup de entradas, salidas e interrupciones
void setup_puertos(){
	
	// MOTOR 1 (SALIDAS)
	DDR_M1_EN |= (1 << M1_EN);
	DDR_M1_DI |= (1 << M1_DI);
	
	// MOTOR 2 (SALIDAS)
	DDR_M2_EN |= (1 << M2_EN);		//Declaramos las entradas en control_motor para que no funcionen todos los motores al mismo tiempo
	DDR_M2_DI |= (1 << M2_DI);
	DDR_M2_BK |= (1 << M2_BK);
	
	// MOTOR 3 (SALIDAS)
	DDR_M3_EN |= (1 << M3_EN);		//Declaramos las entradas en control_motor para que no funcionen todos los motores al mismo tiempo
	DDR_M3_DI |= (1 << M3_DI);
	
	// MOTOR 4 (SALIDAS)
	DDR_M4_EN |= (1 << M4_EN);		//Declaramos las entradas en control_motor para que no funcionen todos los motores al mismo tiempo
	DDR_M4_DI |= (1 << M4_DI);
	
	// MOTOR 5 (SALIDAS)
	DDR_M5_EN |= (1 << M5_EN);
	DDR_M5_DI |= (1 << M5_DI);
	
	// LED (SALIDA) y SW (ENTRADAS)
	DDR_LED |= (1 << LED);
	
	DDR_SW1 &= ~(1 << SW1);
	DDR_SW2 &= ~(1 << SW2);
	DDR_SW3 &= ~(1 << SW3);
	DDR_SW4 &= ~(1 << SW4);
	DDR_SW5 &= ~(0 << SW5);
	DDR_SW6 &= ~(0 << SW6);
	
	//INTERRUPCIONES
	
	cli();
	
	//Interrupciones INT1, INT2, INT3 por flanco de bajada en SW2, SW3, SW4:
	EICRA |= (1 << ISC11) | (1 << ISC21) | (1 << ISC31);
	EIMSK |= (1 << INT1) | (1 << INT2) | (1 << INT3);
	EIFR |= (1 << INT1) | (1 << INT2) | (1 << INT3);
	
	sei();
	
}

//Setup PWM para los motores
void setup_pwm(){
	cli();
	TCCR5A |= (1 << COM5A1) | (1 << COM5B1) | (1 << COM5C1) | (1 << WGM50); 		// Modo Fast PWM (8 bits) + OCR5A + OCR5B + OCR5C
	TCCR5B |= (1 << WGM52) | (1 << CS50) | (1 << CS52); 							 // Preescalado de 1024 (8Mhz/1024*30 ~= 260)
	//TIMSK5 |= (1 << OCIE5A) | (1 << OCIE5B) | (1 << OCIE5C);
	sei();
}

//-----------------------------------------------//
//----------------INTERRUPCIONES-----------------//
//-----------------------------------------------//

//Interrupción SW2 para cambiar dirección M2 en funcion GIRO LANZADOR
ISR(INT1_vect){
	cont_sw2++;
	flag_sw2_giro = 1;
}

//Interrupción SW3 para indicar la llegada al final de carrera
ISR(INT2_vect){
	//flag_sw3 = !flag_sw3;
	flag_sw3 = (flag_sw3 == 0) ? 1 : 0;
}

//Interrupción SW4 para indicar la llegada al final de carrera
ISR(INT3_vect){
	flag_sw4 = 1;
}

//-----------------------------------------------//
//-----------------ANTIRREBOTES------------------//
//-----------------------------------------------//

//ANTIRREBOTES SW2: función interna, no se usa en la máquina de estados
void antirrebotes_sw2(){
	cli();					//Desactivo interrupciones
	delay_ms(10);			//Espero
	if (!(PIN_SW2 & ( 1 << SW2))) flag_sw2_debounced = 1;		//Si el botón sigue pulsado bandera de SW2 antirrebotes a 1
	sei();
}

//ANTIRREBOTES SW3: función interna, no se usa en la máquina de estados
void antirrebotes_sw3(){
	cli();
	delay_ms(7);
	if (!(PIN_SW3 & ( 1 << SW3))) flag_sw3_debounced = 1;
	sei();
}

//ANTIRREBOTES SW4: función interna, no se usa en la máquina de estados
void antirrebotes_sw4(){
	cli();
	delay_ms(7);
	if (!(PIN_SW4 & ( 1 << SW4))) flag_sw4_debounced = 1;
	sei();
}

//-----------------------------------------------//
//----------------CONTROL MOTOR------------------//
//-----------------------------------------------//

//CONTROL_MOTOR: controla los 5 motores pasandole los parametros con PWM
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
			PORT_M2_DI &= ~(1 << M2_DI); break; //DERECHA. SE CAMBIO POR ARTE DE MAGIA
			case 1:
			PORT_M2_DI |= (1 << M2_DI); break;  //IZQUIERDA
		}
		switch (power){
			case 0:
			OCR5B = 0; break; //Apagado
			case 1:
			OCR5B = 150; break;					//Encendido (VALOR ENTRE 0 Y 250) IMPORTANTE !!!!!!!!!!!!!!!
			default:
			OCR5B = 0; break;
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
			OCR5C = 0; break;  //Apagado
			case 1:
			OCR5C = OCR_PWM; break;   //Encendido (VALOR ENTRE 0 Y 250) IMPORTANTE !!!!!!!!!!!!!!!
			default:
			OCR5C = 0; break;
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
			OCR5A = 0; break;	//Apagado
			case 1:
			OCR5A = OCR_PWM; break;	//Encendido (VALOR ENTRE 0 Y 250) IMPORTANTE !!!!!!!!!!!!!!!
			default:
			OCR5A = 0; break;
		}
		
		break;
		
		// Modificar motor 5
		case 5:		
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

//-----------------------------------------------//
//----------------FUNCION DELAY------------------//
//-----------------------------------------------//

//DELAYMS: retardo bloqueante de n milisegundos
void delay_ms(int milis){
	for(int i=0; i<milis;i++){
		for(volatile int j=0;j<444;j++);
	}
}


//-----------------------------------------------//
//----------------HOME DISPARADOR----------------//
//-----------------------------------------------//

//HOME DISPARADOR:pongo lanzador en posición de carga (a la derecha)
void home_lanzador(){
	cont_sw2 = 0;
	// Tenemos que poner bandera que nos pasa el grupo del cargador de las bolas a 1, de momento pongo un 1, bandera de motor 1 arriba
	control_motor(2,1,0,0);		// Todo a la derecha
	
	while(cont_sw2 < 1){
		if (flag_sw2_giro){					//Condicional para el cambio de dirección de M2
			flag_sw2_giro = 0;					//Bandera de la interrupción SW2 a 0
			antirrebotes_sw2();				//Si se detecta una pulsación en SW2, filtro el posible rebote
			if (flag_sw2_debounced)			//En caso de que la pulsación "sea buena"
			{
				flag_sw2_debounced = 0;		//Bandera del filtro de nuevo a 0
				cont_sw2++;
			}
		}
	}
	
	if(cont_sw2 == 2){
		control_motor(2,1,0,0);				// Apago M2
		flag_sw2_derecha = 1;
	}
}

//GET DERECHA: devuelve un 1 cuando el disparador está listo para cargar la bola (a la derecha del todo)
//Pensado para que integración sepa cuandoe está a la derecha el lanzador y pueden cargar la bola
int get_derecha_M2(){
	return flag_sw2_derecha;
}

//GET IZQUIERDA: devuelve un 1 cuando el disparador está a la izquierda del todo
//Está pensado para que en integración sepan cuando está a la izquierda el lanzador y puede empezar a rotar
int get_izquierda_M2(){
	return flag_sw2_izquierda;
}

//-----------------------------------------------//
//------------MOVIMIENTO DEL VÁSTAGO-------------//
//-----------------------------------------------//

//VÁSTAGO: Después de llevar a home el disparador, llevo el vástago y la retención a la posicion que nos interesa
//MOVIMIENTO DEL VÁSTAGO CON ANTIRREBOTES
void mov_vastago(){
	
	control_motor(3,1,1,0); //M3 adelante
	delay_ms(200);
	while (!flag_sw3);
	if (flag_sw3) {
		flag_sw3 = 0;
		antirrebotes_sw3();
		if (flag_sw3_debounced){
			flag_sw3_debounced = 0;
			control_motor(3,0,0,0); //M3 apagado
		}
		
	}
	
	control_motor(4,1,1,0); //M4 adelante (retener vastago)
	delay_ms(200);
	while(!flag_sw4);
	if (flag_sw4) {
		flag_sw4 = 0;
		antirrebotes_sw4();
		if (flag_sw4_debounced){
			flag_sw4_debounced = 0;
			control_motor(4,0,0,0); //M4 apagado
		}
	}
	
	control_motor(3,1,0,0); //M3 atras (tensar vastago)
	delay_ms(200);
	while(!flag_sw3);
	if (flag_sw3) {
		flag_sw3 = 0;
		antirrebotes_sw3();
		if (flag_sw3_debounced){
			flag_sw3_debounced = 0;
			control_motor(3,0,0,0); //M3 apagado
		}
	}
	control_motor(3,0,0,0); //M3 apagado
	flag_vastago_atras = 1;
}

int get_vastago_atras(){
	return flag_vastago_atras;
}

//-----------------------------------------------//
//-------------MOVIMIENTOS LANZADOR--------------//
//-----------------------------------------------//

//LANZADOR_IZQUIERDA: desde la posición home, una vez está cargada la bola, el lanzador se mueve a la izquierda del todo
void lanzador_izquierda(){
	flag_sw2_derecha = 0;
	cont_sw2 = 0;						//Contador del sensor SW2 para no parar en el punto medio
	control_motor(2,1,1,0);				// Motor 2 a la izquierda
	dir_M2 = 0;
	
	while(cont_sw2 < 2){
		if (flag_sw2_giro){					//Condicional para el cambio de dirección de M2
			flag_sw2_giro = 0;					//Bandera de la interrupción SW2 a 0
			antirrebotes_sw2();				//Si se detecta una pulsación en SW2, filtro el posible rebote
			if (flag_sw2_debounced)			//En caso de que la pulsación "sea buena"
			{
				flag_sw2_debounced = 0;		//Bandera del filtro de nuevo a 0
				cont_sw2++;
			}
		}
	}
	
	if(cont_sw2 == 1){
		control_motor(2,1,0,0);				// Apago M2
		flag_sw2_izquierda = 1;
	}
}

//GIRO LANZADOR: gira el lanzador completamente a la izquierda y oscila en la posición de disparo
void giro_lanzador(){
	flag_sw2_giro = 0;
	flag_sw2_izquierda = 0;
	
	control_motor(2,1,dir_M2,0);
	while (!flag_sw2_giro);
	if (flag_sw2_giro){					//Condicional para el cambio de dirección de M2
		flag_sw2_giro = 0;					//Bandera de la interrupción SW2 a 0
		antirrebotes_sw2();				//Si se detecta una pulsación en SW2, filtro el posible rebote
		if (flag_sw2_debounced)			//En caso de que la pulsación "sea buena"
		{
			flag_sw2_debounced = 0;		//Bandera del filtro de nuevo a 0
			control_motor(2,0,0,0);		//Paro el motor
			delay_ms(200);				//Espero para no crear sobretensiones en M2
			dir_M2 = !dir_M2;
			
			control_motor(2,1,dir_M2,0);	//Empiezo a girar en el nuevo sentido
			delay_ms(400);					//Espero un tiempo para garantizar un tiempo mínimo de giro en la nueva dirección y superar siempre SW2
		}
	}
	
}


//-----------------------------------------------//
//-------------PARADA DEL LANZADOR---------------//
//-----------------------------------------------//

//PARADA: para el giro del motor 2 cuando SW6 se pulsa
void disparo  (){
	control_motor(2,0,0,1);		//Apago M2
	delay_ms(500);				//Espero para poder lanzar la bola correctamente
	control_motor(4,1,0,0);		//Libero la retención para poder disparar
	while (!flag_sw4);			//Espero a que llegue el motor al final de carrera
	control_motor(4,0,0,0);		//Apago el motor 4
}


void light_LED(int light){
	if(light){ 
		PORT_LED |= (1<<LED);
		}
	else{
		PORT_LED &= ~(1<<LED);
		}
}