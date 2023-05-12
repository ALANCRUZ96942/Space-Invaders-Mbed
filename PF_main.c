#include "PF_header.h"




int main(void) {

		
	//inicializar variables del acelerometro
	ARM_DRIVER_I2C* i2cDrv2 = &Driver_I2C2;
	volatile int8_t xValue = 0;
	
	// Initialize system
	SystemInit();
	GPIO_PortClock(1);
	
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
		//Inicializar i2c
	// Initialize and configure the accelerometer.
	// It needs a small delay ~13 ms to apply the configuration.		
		I2CACCEL_Init(i2cDrv2);
	I2CACCEL_Configure(i2cDrv2);
	delay(13 * MS);
		
	// Reset buffer
	Buffer_Reset(buffer);

	//funcion declarada anteriormente de la configuracion del timmer 0
	timmer0_configuration();
	//habilitar las interrupciones del timer 0
	NVIC_EnableIRQ(TIMER0_IRQn);
			
  /*Inicializar nave en posicion inicial*/
	draw_ship(buffer, x_ship, y_ship);
 
 /*Inicializar enemigos*/
	
  draw_all_enemies(buffer,Enemies);
 
	// Transfer buffer to LCD
	SPILCD_Transfer(spiDrv1, buffer);
	
	//inicializar el ADC
	adc_configuration();
	
	//inicializar variables iniciales de la bala en y de la nave, es decir en su parte superior
	i = 32-height_ship;
	
	//elegir un enemigo random
	do{
		random =  (rand() % (upper - lower + 1)) + lower; //enemigo random para disparar
		x_bullet_enemie = Enemies[random][0]; //coordenada en x
		j = Enemies[random][1]; //coordenada en y
	}while((Enemies[random][2] != 1)); //solo si ese enemigo esta vivo
	
	x_bullet = x_ship+4; //primer posicion en x de la bala de la nave 
	
	while(1){
		
		if(pause == 1 && life > 0){ //si la pantalla esta en  pausa
			state_game = 0; //juego apenas iniciado
			//pantalla de inicio/pausa
			Buffer_Reset(buffer);
			draw_pause_screen(buffer);
			SPILCD_Transfer(spiDrv1, buffer); 	// Transfer buffer to LCD
			
			//encender led en blanco
			GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_RED,state_led_off); 
			GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_GREEN,state_led_off);
			GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_BLUE,state_led_off); 
		}
		else if(updateLCD == 1 && life > 0 && pause == 0  && state_game == 0){ //si tenemos bandera de actualizacion de pantalla y si tenemos vida r iniciamos el juego
		
			updateLCD = 0; //limpiar bandera
			Buffer_Reset(buffer);//actualizar pantalla
			
			if(mode == 1){ //si estamos en modo potenciometro
				x_ship =  ADC_value(ADC4)/550; //valor de x mapeado al adc (0-127)
			}
			else{ //modo acelerometro
				xValue = I2CACCEL_GetValue(i2cDrv2, ACCEL_AXIS_X); //valor de -32 a 31
				x_ship = x_ship - xValue;			//la posicion de la nave le restamos la aceleracion
			}


			if(bullet_enemie == 1){ //si es momento de disparar por parte de los enemigos
				
				if(j<31){ //si la bala aun no toca el suelo
				 j++; //hacer que la bala baje
				 check_damage(x_bullet_enemie,j,x_ship); //verificar el daño de la nave
				 Buffer_DrawRect(buffer, x_bullet_enemie, j, width_bullet, height_bullet); //dibujar la bala
				}
				else{ //si no ha tocado el suelo
					bullet_enemie = 0; //limpiar bandera de bala
						do{ //preparar el proximo enemigo random
						random =  (rand() % (upper - lower + 1)) + lower; //enemigo random para disparar
						x_bullet_enemie = Enemies[random][0]; //coordenada en x
						j = Enemies[random][1]; //coordenada en y
					}while((Enemies[random][2] != 1)); //solo si ese enemigo esta vivo
				}
			}
				
			if(bullet == 1){ //si la nave del jugador dispara
				
				if(i == 32-height_ship){//si la bala apenas está iniciando desde la nave
					i--; //hacer que la bala suba
					x_bullet = x_ship+4; //guardar la posicion de donde fue disparada (centro de la nave en ese momento)
					check_enemies(x_bullet,i,Enemies); //verificar si le hizo daño a algun enemigo
					Buffer_DrawRect(buffer, x_bullet, i, width_bullet, height_bullet); //dibujar la bala
				}
				else if(i < 32-height_ship && i > 0){ //si aun no llega al limite superior
					i--; //seguir subiendo
					check_enemies(x_bullet,i,Enemies); //verificar si le hizo daño a algun enemigo
					Buffer_DrawRect(buffer, x_bullet, i, width_bullet, height_bullet); //dibujar la bala
				}
				else{
					x_bullet = x_ship+4; //poner la bala en el centro de la nave
					bullet = 0; //limpiar bandera de bala
					i = 32-height_ship; //inicializar i 
				}
			}
			
			if(life <= 21 && life > 14){
				//verde
				GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_RED,state_led_on); 
				GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_GREEN,state_led_on);
				GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_BLUE,state_led_off); 
			}
			else if(life <= 14 && life > 7){
				//amarillo
				GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_RED,state_led_off); 
				GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_GREEN,state_led_on);
				GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_BLUE,state_led_off); 	
			}
			else if(life <= 7 && life > 0){
				//magenta
			  GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_RED,state_led_off); 
				GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_GREEN,state_led_off);
				GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_BLUE,state_led_on);
				
			}
			else{
				//rojo
				GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_RED,state_led_off); 
				GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_GREEN,state_led_on);
				GPIO_PinWrite(PORT_LED_RGB, PIN_RGB_BLUE,state_led_on); 
			}
			
			/*Rangos para enemigos*/
			if(Enemies[0][0] >= (127-width_enemies)){ //si nos salimos del borde
				sentido_enemies = 1; //cambiar sentido a la izquierda
			}
			if(Enemies[0][0] == 0){ //si nos salimos del borde
				sentido_enemies = 0; //cambiar sentido a la derecha
			}
			
			/*Establecer limites en pantalla para la nave*/
			
			if(x_ship >= (127-width_ship) && x_ship > 0){ //si lo supera en x
				x_ship = 127-width_ship; //regresar la nave al limite
			}
			else if(x_ship <= limite_inf){ //si hay un underflow en x
				x_ship = limite_inf; //regresar la nave al limite
			}

			//mover a los enemigos cada tiempo 
			move_enemies(sentido_enemies,Enemies);

			//Buffer_DrawRect(buffer, x, y, width, height); 
			draw_ship(buffer, x_ship, y_ship); //actualizar la nave del jugador
			//actualizar enemigos
			draw_all_enemies(buffer,Enemies);			
			SPILCD_Transfer(spiDrv1, buffer); 	// Transfer buffer to LCD
	
		}
		//si hemos perdido
		else if(life <= 0 && pause == 0){
			state_game = 1;			//has perdido
			/*Dibujar pantalla de game over*/
			Buffer_Reset(buffer);
			draw_gameOver(buffer);
			SPILCD_Transfer(spiDrv1, buffer); 	// Transfer buffer to LCDW
		}
		//si hemos ganado
		else if(life > 0 && pause == 0 && win >= 10){
			/*Dibujar pantalla de win*/
			state_game = 2;			//has ganado
			Buffer_Reset(buffer);
			draw_gameWin(buffer);
			SPILCD_Transfer(spiDrv1, buffer); 	// Transfer buffer to LCDW
		}
	}
}
