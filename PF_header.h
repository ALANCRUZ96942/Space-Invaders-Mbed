#include <LPC17xx.h>
#include "I2C_ACCEL.h"
#include "Delay.h"
#include <PIN_LPC17xx.h>
#include <GPIO_LPC17xx.h>
#include "SPI_LCD.h"
#include "Buffer.h"
#include "PF_timmer.h"
#include "PF_pin.h"

#include <stdio.h>
#include <stdlib.h>



/* Estos puertos se han añadido en base a los esquemas aportados 
 * para el desarrollo de la practica
 */

//LED RGB
#define PORT_LED_RGB 2
#define PIN_RGB_RED 3
#define PIN_RGB_GREEN 1
#define PIN_RGB_BLUE 2

// JOYSTICK
#define PORT_JOY 0
#define PIN_JOY_UP 23
#define PIN_JOY_RIGHT 24
#define PIN_JOY_LEFT 15
#define PIN_JOY_DOWN 17
#define PIN_JOY_CENTER 16

//adc
//POTENCIOMETRO
#define PORT_POT 1
#define PIN_POT_1 30
#define PIN_POT_2 31
#define ADC4  4
#define ADC5	5


/********************************************* Variables*******************************************/

	/*Valores para el estado actual del jostick*/
static uint32_t state_joy_up; 
static uint32_t state_joy_right;
static uint32_t state_joy_left;
static uint32_t state_joy_down;
static uint32_t state_joy_center;

/*Valores para el estado anterior del jostick*/

static uint32_t state_joy_up_prev; 
static uint32_t state_joy_right_prev;
static uint32_t state_joy_left_prev;
static uint32_t state_joy_down_prev;
static uint32_t state_joy_center_prev;

extern ARM_DRIVER_SPI Driver_SPI1;
ARM_DRIVER_SPI* spiDrv1 = &Driver_SPI1;
uint8_t buffer[LCD_MEM_SIZE];

/**/

/*Drivers I2C acelerometro*/
extern ARM_DRIVER_I2C Driver_I2C2;


// Declare values as volatile to prevent the compiler from
// optimizing them, and ensure they are shown in the debugger.


/*Variables nave*/
int32_t x_ship = 60; //posición x de la nave
int32_t y_ship = 26; //posición y de la nave
static uint32_t width_ship = 9; //ancho de la nave del jugador 
static uint32_t height_ship = 7; //alto ancho de la nave del jugador 
uint8_t life = 21; //variable de la vida de la nave, en realidad son 3 puesto que 21/height_ship

/*Variables bala*/
static uint32_t width_bullet = 1; //ancho de la bala
static uint32_t height_bullet = 2; //alto de la bala

/*Variables enemigos*/
uint8_t x_enemies = 0; //posición x de todos los enemigos
uint8_t y_enemies = 0; //posición y de todos los enemigos
static uint32_t width_enemies = 59; //ancho de todos los enemigos en total
static uint32_t height_enemies = 26; //alto de todos los enemigos en total
bool sentido_enemies = 0; //direccion a la que se mueven los enemigos


/*rangos*/
static uint32_t width_max = 40; //ancho maximo
static uint32_t height_max= 20; //alto maximo
static uint32_t width_min = 30; //ancho del rectángulo
static uint32_t height_min = 10; //alto del rectángulo
int32_t limite_inf = 0;

uint8_t direccion = 0; //direccion del rectangulo, 0 equivale a izquierda (inicial)
										// 1 a la derecha, 2 hacia arriba y 3 hacia abajo
										//5 en pausa

	/* Variables de tipo boolean inicializadas a cero que cumplen la misma funcion para diferentes casos.
     * Realizaran la funcion de cambiar su valor cuando sea necesario. Es decir, seran la variable que
	 * apague o prenda los leds.
     */
bool state_led_off = 0;
bool state_led_on = 1;

/*Variables auxiliares */

bool bullet_enemie; //variable bandera para el disparo de enemigos
bool bullet = 0; //variable bandera para el disparo de la nave (jugador)
bool pause = 1; //variable bandera para pausar el juego 
int32_t x_bullet; //posicion x de la bala del jugador
int32_t i;//posicion  y de la bala del jugador

int32_t j; //posicion y de la bala del enemigo
int32_t x_bullet_enemie = 0;//posicion x de la bala del enemigo

int32_t lower = 0; //valor minimo para el disparo del enemigo random
int32_t upper = 9; //valor maximo para el disparo del enemigo random 
int32_t random; // variable que guarda un valor random que se pondra en el array  de enemigos

int32_t dis_en = 0; //contador de tiempo para que dispare el enemigo
int32_t state_game = 0; //estado del juego: inicio, perdiste o ganaste
int32_t win = 0; // variable que contabiliza enemigos abatidos



bool mode = 1; //variable para elegir modo acelerometro o potenciometro
static uint32_t range = 0x333; //valor de una quinta parte del rango del adc

//variable para actualizar pantalla
bool updateLCD = 1;

uint8_t Enemies[10][3] ={{ 0, 0,1},{ 12,0,1},{ 24,0,1},{ 36,0,1},{48,0,1},
										     { 0, 9,1},{ 12,9,1},{ 24,9,1},{ 36,9,1},{48,9,1},
											};  //factores enemigos x,y, estado



											
/*Funciones que se utilizan en el juego*/
											
void read_peripherals(void){   //funcion para leer el jostick 
		
	state_joy_center = GPIO_PinRead(PORT_JOY, PIN_JOY_CENTER);  //lectura de la gpio del jostick "actual"
	if(state_joy_center == 1 &&  state_joy_center_prev == 0){		//cuando presionamos el boton del centro, disparamos																			
		bullet = 1;	//estado de bala a 1
	}

	state_joy_up = GPIO_PinRead(PORT_JOY, PIN_JOY_UP); //lectura de la gpio del jostick "actual"
	if(state_joy_up == 1 &&  state_joy_up_prev == 0){ 
		//lectura de flanco de subida (de 0 a 1) del jostick hacia arriba
		pause = !pause; //poner o quitar pausa

		//si has ganado o perdido al reiniciar los enemigos, se restauran
		if(state_game == 1 || state_game == 2){
			for (int a = 0; a < 10; a++) {
				Enemies[a][2] = 1; //revivir a todos los enemigos
			}
			life = 21; //se restaura la vida
			win = 0; // y se reinicia la variable de enemigos abatidos 
		}
	}

	state_joy_down = GPIO_PinRead(PORT_JOY, PIN_JOY_DOWN); //lectura de la gpio del jostick "actual"
	if(state_joy_down  == 1 &&   state_joy_down_prev == 0){ 		//lectura de flanco de subida (de 0 a 1) hacia abajo
		mode = !mode; //se cambia modo acelerometro o jostick
	}

	/*actualizar estado del boton jostick previo*/
	state_joy_center_prev = state_joy_center;  
	state_joy_up_prev = state_joy_up;
	state_joy_down_prev = state_joy_down;
}



void move_enemies(bool sentido, uint8_t posiciones[10][3] ){ //funcion para mover a los enemigos
	for (int a = 0; a < 10; a++) {
			if(sentido == 0){ //si vamos a la derecha
				posiciones[a][0] = posiciones[a][0] + 1; //mover a la derecha
			}
			else{ //mover a la izquierda 
				posiciones[a][0] = posiciones[a][0] - 1;
			}
	}
}


void check_enemies(uint8_t bullet_x,uint8_t bullet_y,uint8_t posicions[10][3]){ // verificar si la bala le dio a algún enemigo
	for (int a = 0; a < 10; a++) { //checar todos los enemigos 
		if((bullet_x > posicions[a][0] && bullet_x < (posicions[a][0] + 10)) && (bullet_y < (posicions[a][1] + 7) && bullet_y > posicions[a][1]) && (posicions[a][2] == 1)){ //si la bala esta en un rango de algun enemigo 
				posicions[a][2] = 0;  //marcar como eliminado el enemigo para desaparecerlo
				bullet = 0; //desactivar la bala para que no siga avanzando
				i = 32-height_ship; //inicializar i para la siguiente vez que disparemos
				win = win + 1; //aumentar la variable de enemigos abatidos
			}
	}
}

void check_damage(uint8_t bullet_enemie_x,uint8_t bullet_enemie_y,uint8_t x_ship){ // verificar si la bala le dio a la nave del jugador		
	if(bullet_enemie_x >= x_ship && bullet_enemie_x <= (x_ship + width_ship) && bullet_enemie_y >= 31-height_ship ){ //si la bala esta en el rango de la nave 
		life = life-1; // restar una vida por cada pixel
	}
}


void adc_configuration(){ //configuracion del adc
	initPin_ADC(PORT_POT,PIN_POT_1);  //inicializa los dos pines adc a utilizar
	initPin_ADC(PORT_POT,PIN_POT_2);
	LPC_SC->PCONP = LPC_SC->PCONP | (0x1 << 12); 				//se habilita el pin 12 a 1 del registro pconp
	LPC_ADC->ADCR = LPC_ADC->ADCR  | (0x400 << 0); 		 // pone el valor 4 en los bits de (15:8) (0x04 << 8)
	LPC_ADC->ADCR = LPC_ADC->ADCR | (0x1 << 21);   		// Habilitar el conversor
	LPC_ADC->ADCR = LPC_ADC->ADCR & ~(0x7 << 24);  		// Asegurar que el conversor está parado
	
}

 
static uint32_t ADC_value(uint32_t channel){ //funcion para leer el valor del canal adc seleccionado

	uint32_t value,conf; 
	LPC_ADC->ADCR	 = LPC_ADC->ADCR  & ~(0xFF << 0); //Resetea los canales 
	conf	  = LPC_ADC->ADCR  |  (0x1 << channel);	 //seleccionar la entrada del bit del canal a 1
	LPC_ADC->ADCR = conf;
	LPC_ADC->ADCR = LPC_ADC->ADCR | (0x1 << 24); // //iniciar la conversion
	while((LPC_ADC->ADGDR & (0x1 << 31)) == 0); // Esperar a que termine la conversión
	value = (LPC_ADC->ADGDR & (0xFFF0 << 4));// Leer el valor medido em lo bits 4 a 15;
	return value;								
}


 
 void TIMER0_IRQHandler(void){ //funcion interrupcion del timmer
					
			  LPC_TIM0->IR |= 1 << 0; //reiniciar contador limpiando la bandera de interrupcion
				read_peripherals();		
				updateLCD = 1; //asignar 1 para la bandera de refresco de pantalla
				if(dis_en < 13){ //contar cada cuanto va a disprar un enemigo es deci 125ms*12
				  dis_en = dis_en + 1; 
				}

				else{ //cuando se cumpla el tiempo de este "segundo" contador 
					bullet_enemie = 1; //se activa la bandera para que los enemigos disparen
					dis_en = 0; //luego se reinicia el contador de disparo de enemigos
				}
}
											
											
											
