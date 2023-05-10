#include <stdint.h>
#include <LPC17xx.h>
#include <PIN_LPC17xx.h>
#include <GPIO_LPC17xx.h>
/**************Funciones globales*******************/
 
static uint32_t time = 0x2FAF08; //numero de ciclos de reloj a esperar en el timmer 61250000 obtenido mediante el calculo del tiempo necesitado (125 ms) 5F5E10 250ms

static uint32_t pres = 0; //valor del preescalador

void timmer0_configuration(void);