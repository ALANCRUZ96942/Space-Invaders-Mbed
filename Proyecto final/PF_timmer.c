#include "PF_timmer.h"

//configuracion del timmer 0
void timmer0_configuration(){
	LPC_TIM0->MCR  = LPC_TIM0->MCR  | (0x00000001 << 0);  // Activar el bit MR0I del registro MCR del timer 0
	LPC_TIM0->MCR = LPC_TIM0->MCR | (0x00000001 << 1);// Activar el bit MR0R del registro MCR del timer 0
	//Las dos activaciones anteriores se realizan para que se genere una interrupción y se reinicie el contador 
	//cuando se alcance el valor del registro de comparación MR0.
	LPC_TIM0->MR0 = time << 0;// Match register del timer 0 (cuantos ciclos de reloj debe esperar
	LPC_TIM0->PR = pres; //// Prescaler del timer 0
	LPC_TIM0->TCR = LPC_TIM0->TCR | (0x00000001 << 0); // Activar Counter Enable del registro TCR del timer 0
	//Esta ultima activación pone en marcha el contador del temporizador, el cual se reinicia cuando alcanza el
	//el valor guardado en MR0
}

