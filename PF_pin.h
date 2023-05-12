
#include <LPC17xx.h>
#include <PIN_LPC17xx.h>
#include <GPIO_LPC17xx.h>
/*metodos para inicializar pines*/
void initPin_LED(uint32_t port_number, uint32_t pin_number);
void initPin_JOY(uint32_t port_number, uint32_t pin_number);
void initPin_ADC(uint32_t port_number, uint32_t pin_number);