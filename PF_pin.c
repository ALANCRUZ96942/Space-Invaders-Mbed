#include "PF_pin.h"

void initPin_LED(uint32_t port_number, uint32_t pin_number)
{
	// configura el pin como GPIO de salida con resistencia de pulldown como GPIO (func 0)
	GPIO_SetDir(port_number, pin_number, GPIO_DIR_OUTPUT);
	PIN_Configure(port_number, pin_number, PIN_FUNC_0,
	              PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);
}

void initPin_JOY(uint32_t port_number, uint32_t pin_number)
{
	// configura el pin como GPIO de entrada con resistencia de pulldown como GPIO (func 0)
	GPIO_SetDir(port_number, pin_number, GPIO_DIR_INPUT);
	PIN_Configure(port_number, pin_number, PIN_FUNC_0,
	              PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);
}

void initPin_ADC(uint32_t port_number, uint32_t pin_number)
{
	// Configurar el pin para que funcione como ADC
	PIN_Configure(port_number, pin_number, PIN_FUNC_3,
	              PIN_PINMODE_TRISTATE, PIN_PINMODE_NORMAL);
}
