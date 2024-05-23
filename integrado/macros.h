#ifndef MACROS_H_
#define MACROS_H_

// Declaración de macros para facilitar el desarollo y entendimiento del código

//PWM

#define DIV_FRECUENCIA 30
#define FREC_ATMEGA640 8000000UL

#define OCR_CUENTA 255
#define CICLO 1
#define OCR_PWM OCR_CUENTA * CICLO
//#define OCR_PWM 200

//MOTOR 1
#define DDR_M1_EN DDRB
#define DDR_M1_DI DDRL

#define M1_EN DDB6
#define M1_DI DDL0

#define PORT_M1_EN PORTB
#define PORT_M1_DI PORTL

// MOTOR 2
#define DDR_M2_EN DDRL
#define DDR_M2_DI DDRL
#define DDR_M2_BK DDRB

#define M2_EN DDL4
#define M2_DI DDL1
#define M2_BK DDB4

#define PORT_M2_EN PORTL
#define PORT_M2_DI PORTL
#define PORT_M2_BK PORTB

// MOTOR 3
#define DDR_M3_EN DDRL
#define DDR_M3_DI DDRL

#define M3_EN DDL5
#define M3_DI DDL2

#define PORT_M3_EN PORTL
#define PORT_M3_DI PORTL

// MOTOR 4
#define DDR_M4_EN DDRL
#define DDR_M4_DI DDRL

#define M4_EN DDL3
#define M4_DI DDL6

#define PORT_M4_EN PORTL
#define PORT_M4_DI PORTL

// MOTOR 5
#define DDR_M5_EN DDRB
#define DDR_M5_DI DDRL

#define M5_EN DDB5
#define M5_DI DDL7

#define PORT_M5_EN PORTB
#define PORT_M5_DI PORTL

//Botones SW y LED

#define DDR_LED DDRK
#define LED DDK1
#define PORT_LED PORTK

#define DDR_SW1 DDRD
#define DDR_SW2 DDRD
#define DDR_SW3 DDRD
#define DDR_SW4 DDRD

#define SW1 DDD0
#define SW2 DDD1
#define SW3 DDD2
#define SW4 DDD3

#define PIN_SW1 PIND
#define PIN_SW2 PIND
#define PIN_SW3 PIND
#define PIN_SW4 PIND

#define DDR_SW5 DDRB
#define SW5 DDB7
#define PIN_SW5 PINB

#define DDR_SW6 DDRK
#define SW6 DDK0
#define PIN_SW6 PINK



#endif /* MACROS_H_ */