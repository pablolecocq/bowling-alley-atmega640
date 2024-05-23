#ifndef FUNCIONES_LANZADOR_H_
#define FUNCIONES_LANZADOR_H_

void setup_puertos();

void setup_pwm();

void control_motor(int n_motor, int power, int direccion, int brake);

void delay_ms(int milis);

void home_lanzador();

void mov_vastago();

void giro_lanzador();




#endif /* FUNCIONES_LANZADOR_H_ */