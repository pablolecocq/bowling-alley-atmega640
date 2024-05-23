#ifndef FUNCIONES_LANZADOR_H_
#define FUNCIONES_LANZADOR_H_

void setup_puertos();

void setup_pwm();

void control_motor(int n_motor, int power, int direccion, int brake);

void delay_ms(int milis);

int delay_timer(int milis);

void home_lanzador();

void mov_vastago();

void giro_lanzadorr();

void giro_lanzador();

void parada();

void lanzador_izquierda();

int get_derecha_M2();

int get_izquierda_M2();

void antirrebotes_sw2();

void antirrebotes_sw3();

void antirrebotes_sw4();

#endif /* FUNCIONES_LANZADOR_H_ */