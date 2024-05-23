/*
 * Timer0.h
 *
 * Created: 10/05/2024 8:42:25
 *  Author: pablo
 */ 


#ifndef TIMER0_H_
#define TIMER0_H_

extern long int t_disp, t_fin_bolos;
int cont_ms;

void setup_timer0();
void tiempo_juego_cero();
void tiempo_fin_bolos();
int ultima_tirada();
void parp_disp();
#endif /* TIMER0_H_ */