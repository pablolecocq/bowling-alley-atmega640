#ifndef FUNCIONES_LANZADOR_H_
#define FUNCIONES_LANZADOR_H_

void setup_puertos();

void setup_pwm();

void control_motor(int n_motor, int power, int direccion, int brake);

void delay_ms(int milis);

void light_LED(int light);

void home_lanzador();

void mov_vastago();

int get_vastago_atras();

void giro_lanzador();

void disparo();

void lanzador_izquierda();

int get_derecha_M2();

int get_izquierda_M2();

void antirrebotes_sw2();

void antirrebotes_sw2_giro();

void antirrebotes_sw3();

void antirrebotes_sw4();

int get_disparo();

#endif /* FUNCIONES_LANZADOR_H_ */