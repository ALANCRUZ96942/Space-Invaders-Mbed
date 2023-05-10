#include <LPC17xx.h>
#include <PIN_LPC17xx.h>
#include <GPIO_LPC17xx.h>
#include "SPI_LCD.h"
#include "Buffer.h"
#include "PF_timmer.h"
#include "PF_pin.h"




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


uint8_t x = 60; //posición x del rectángulo
uint8_t y = 10; //posición y del rectángulo

static uint32_t width = 30; //ancho del rectángulo
static uint32_t height = 10; //alto del rectángulo

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