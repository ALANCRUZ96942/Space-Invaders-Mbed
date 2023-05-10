#include "PF_header.h"

void set_direction(void){   //funcion para leer el jostick y dirigir el rectángulo 
		
		state_joy_center = GPIO_PinRead(PORT_JOY, PIN_JOY_CENTER);  //lectura de la gpio del jostick "actual"
		if(state_joy_center == 1 &&  state_joy_center_prev == 0){ //lectura de flanco de subida (de 0 a 1)
			
				if(x > 0 && x < (127  - width_max ) && (y > 0 && y < 31  - height_max)){  //crecer dentro de pantalla para que no haya overflow

							if(width < width_max && height < height_max){   //si es menor que el maximo
								width = width + 5;   //aumentar el tamaño en 5
								height = height + 5;
							direccion = direccion; 
							}
							else {
								if(width > width_min && height > height_min)
								width = width - 5; //disminuir el tamaño en 5
								height = height - 5;
							direccion = direccion; 
							}
					
					}

			
			LPC_TIM0->IR |= 1 << 0;  //reiniciar contador limpiando la bandera de interrupcion

		}
		
		state_joy_up = GPIO_PinRead(PORT_JOY, PIN_JOY_UP); //lectura de la gpio del jostick "actual"
		if(state_joy_up == 1 &&  state_joy_up_prev == 0){
		//lectura de flanco de subida (de 0 a 1)
			direccion = 2; //direccion hacia arriba
			LPC_TIM0->IR |= 1 << 0;  //reiniciar contador limpiando la bandera de interrupcion

		}
		
		state_joy_down = GPIO_PinRead(PORT_JOY, PIN_JOY_DOWN); //lectura de la gpio del jostick "actual"
		if(state_joy_down  == 1 &&   state_joy_down_prev == 0){ 		//lectura de flanco de subida (de 0 a 1)
		//lectura de flanco de subida (de 0 a 1)
			direccion = 3; //direccion hacia abajo
			LPC_TIM0->IR |= 1 << 0;  //reiniciar contador limpiando la bandera de interrupcion
		}
		
		state_joy_left = GPIO_PinRead(PORT_JOY, PIN_JOY_LEFT); //lectura de la gpio del jostick "actual"
		if(state_joy_left  == 1 &&   state_joy_left_prev == 0){ 		//lectura de flanco de subida (de 0 a 1)
					//lectura de flanco de subida (de 0 a 1)
			direccion = 0; //direccion hacia la izquierda
			LPC_TIM0->IR |= 1 << 0;  //reiniciar contador limpiando la bandera de interrupcion
		}
		
		
		state_joy_right = GPIO_PinRead(PORT_JOY, PIN_JOY_RIGHT); //lectura de la gpio del jostick "actual"
		if(state_joy_right  == 1 &&   state_joy_right_prev == 0){ 		//lectura de flanco de subida (de 0 a 1)
								//lectura de flanco de subida (de 0 a 1)
			direccion = 1; //direccion hacia la derecha
			LPC_TIM0->IR |= 1 << 0;  //reiniciar contador limpiando la bandera de interrupcion
		}
		
		/*actualizar estado del boton jostick previo*/
		state_joy_center_prev = state_joy_center;  
		state_joy_up_prev = state_joy_up;
		state_joy_down_prev = state_joy_down;
		state_joy_left_prev = state_joy_left;
		state_joy_right_prev = state_joy_right;



}







void TIMER0_IRQHandler(void){
	
	
	
	
		set_direction(); //funcion para leer el jostick y poner direccion

		if((LPC_TIM0->IR & 0x01) == 0x01){//si el timmer ha llegado al valor mr0
			
			LPC_TIM0->IR |= 1 << 0; //reiniciar contador limpiando la bandera de interrupcion
			
			
				
				if(x > 0 && x < (127  - width ) && (y > 0 && y < 31  - height)){

									//si se encuentra en el area de la pantalla
						if(direccion == 0){ //si la direccion se encuentra en 0 (hacia la izquierda)
							x = x-1; //desplazar a la izquierda  restando un pixel (por defecto)
						}
						else if(direccion == 1){  //si la direccion se encuentra en 1 (hacia la derecha)
							x = x+1; //desplazar a la derecha sumando un pixel
						}
						
						else if(direccion == 2){ //si la direccion se encuentra en 2 (hacia arriba)
							y = y-1; //desplazar hacia arriba  restando un pixel y
						}
						else if(direccion == 3){  //si la direccion se encuentra en 3 (hacia abajo)
							y = y+1; //desplazar hacia abajo sumando un pixel
						}
					
					}

				
				else{ //si se encuentra en algun borde, hacerlo rebotar
					if(x == 0){ //si llegamos al borde izquierdo cambiamos la direccion y actualizamos x
						direccion = 1;
						x = x+1;
						//Cuando choca a la izquieda enciende el led en rojo
						GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_RED,state_led_on); 
						GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_GREEN,state_led_off);
						GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_BLUE,state_led_off); 
					}
					 if(x == (127 - width)){ //si llegamos al borde derecho cambiamos la direccion y actualizamos x
						direccion = 0;
						x = x-1;
						//Cuando choca a la derecha enciende el led en amarillo
						GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_RED,state_led_on); 
						GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_GREEN,state_led_on);
						GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_BLUE,state_led_off); 
					}
					 if(y == 0){ //si llegamos al borde superior cambiamos la direccion y actualizamos 'y'
						direccion = 3;
						y = y+1;
						//Cuando choca a la derecha enciende el led en azul
						GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_RED,state_led_off); 
						GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_GREEN,state_led_off);
						GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_BLUE,state_led_on); 
					}
					 if(y == (31 - height)){ //si llegamos al borde inferior cambiamos la direccion y actualizamos 'y'
						direccion = 2;
						y = y-1;
						//Cuando choca a la derecha enciende el led en cian
						GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_RED,state_led_off); 
						GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_GREEN,state_led_on);
						GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_BLUE,state_led_on); 
					}
				}

			}

}




int main(void) {

	
	// Initialize system
	SystemInit();
	
	
	//inicializacion de los pines del led RGB
	initPin_LED(PORT_LED_RGB, PIN_RGB_RED);
	initPin_LED(PORT_LED_RGB, PIN_RGB_GREEN);
	initPin_LED(PORT_LED_RGB, PIN_RGB_BLUE);
	
	
	//inicializacion de los pines del jostick y el boton central 
	initPin_JOY(PORT_JOY, PIN_JOY_UP);
	initPin_JOY(PORT_JOY, PIN_JOY_DOWN);
	initPin_JOY(PORT_JOY, PIN_JOY_LEFT);
	initPin_JOY(PORT_JOY, PIN_JOY_RIGHT);
	initPin_JOY(PORT_JOY, PIN_JOY_CENTER);

	// Initialize, reset and configure LCD
	SPILCD_Init(spiDrv1);
	SPILCD_Reset(spiDrv1);
	SPILCD_Configure(spiDrv1);
	
	// Reset buffer
	Buffer_Reset(buffer);

	//funcion declarada anteriormente de la configuracion del timmer 0
	timmer0_configuration();
	//habilitar las interrupciones del timer 0
	NVIC_EnableIRQ(TIMER0_IRQn);
	
	
	//posición inicial del rectángulo
	// Draw rectangle to buffer at (x,y)=(60,10) with size (width,height)=(30,10)
	Buffer_DrawRect(buffer, x, y, width, height);

	// Transfer buffer to LCD
	SPILCD_Transfer(spiDrv1, buffer);

	while(1){
			
			Buffer_Reset(buffer);
			Buffer_DrawRect(buffer, x, y, width, height); //dibujamos el nuevo rectangulo
			draw_game(buffer); //DIBUJAR la palabra "GAME"
			SPILCD_Transfer(spiDrv1, buffer); //Actualizamos la pantalla 
	
	
	}
}
