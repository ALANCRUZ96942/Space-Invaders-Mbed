#include <LPC17xx.h>
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
#define PIN_RGB_GREEN 2
#define PIN_RGB_BLUE 1

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


/*Variables nave*/
uint8_t x_ship = 60; //posición x de la nave
uint8_t y_ship = 26; //posición y de la nave
static uint32_t width_ship = 9; //ancho de la nave del jugador 
static uint32_t height_ship = 7; //alto ancho de la nave del jugador 
uint8_t life = 21;

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


uint8_t direccion = 0; //direccion del rectangulo, 0 equivale a izquierda (inicial)
										// 1 a la derecha, 2 hacia arriba y 3 hacia abajo
										//5 en pausa

	/* Variables de tipo boolean inicializadas a cero que cumplen la misma funcion para diferentes casos.
     * Realizaran la funcion de cambiar su valor cuando sea necesario. Es decir, seran la variable que
	 * apague o prenda los leds.
     */
bool state_led_on = 1;
bool state_led_off = 0;