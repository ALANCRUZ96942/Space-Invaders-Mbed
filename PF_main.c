#include "PF_header.h"
int bullet_enemie;
int bullet = 0;
int pause = 1;
int i,x_bullet;

uint8_t j;
uint8_t x_bullet_enemie = 0;
int lower = 0; 
int upper = 9;
int random;
int dis_en = 0;


uint8_t Enemies[15][3] ={{ 0, 0,1},{ 12,0,1},{ 24,0,1},{ 36,0,1},{48,0,1},
										 { 0, 9,1},{ 12,9,1},{ 24,9,1},{ 36,9,1},{48,9,1},
										 { 0, 18,1},{ 12,18,1},{ 24,18,1},{ 36,18,1},{48,18,1},
											}; //factores enemigos

void set_direction(void){   //funcion para leer el jostick y dirigir el rectángulo 
		
		state_joy_center = GPIO_PinRead(PORT_JOY, PIN_JOY_CENTER);  //lectura de la gpio del jostick "actual"
		if(state_joy_center == 1 &&  state_joy_center_prev == 0){																					
					bullet = 1;	
		}
		
		state_joy_up = GPIO_PinRead(PORT_JOY, PIN_JOY_UP); //lectura de la gpio del jostick "actual"
		if(state_joy_up == 1 &&  state_joy_up_prev == 0){
		//lectura de flanco de subida (de 0 a 1)
			pause = !pause;
		}
		
		state_joy_down = GPIO_PinRead(PORT_JOY, PIN_JOY_DOWN); //lectura de la gpio del jostick "actual"
		if(state_joy_down  == 1 &&   state_joy_down_prev == 0){ 		//lectura de flanco de subida (de 0 a 1)
		
		}
		
		state_joy_left = GPIO_PinRead(PORT_JOY, PIN_JOY_LEFT); //lectura de la gpio del jostick "actual"
		if(state_joy_left  == 1 &&   state_joy_left_prev == 0){ 		//lectura de flanco de subida (de 0 a 1)

		}
		
		
		state_joy_right = GPIO_PinRead(PORT_JOY, PIN_JOY_RIGHT); //lectura de la gpio del jostick "actual"
		if(state_joy_right  == 1 &&   state_joy_right_prev == 0){ 		//lectura de flanco de subida (de 0 a 1)
		
		}
		
		/*actualizar estado del boton jostick previo*/
		state_joy_center_prev = state_joy_center;  
		state_joy_up_prev = state_joy_up;
		state_joy_down_prev = state_joy_down;
		state_joy_left_prev = state_joy_left;
		state_joy_right_prev = state_joy_right;



}









//variable para actualizar pantalla
bool updateLCD = 1;

static uint32_t range = 0x333; //valor de una quinta parte del rango del adc

void move_enemies(bool sentido, uint8_t posiciones[15][3] ){
    for (int a = 0; a < 15; a++) {
				if(sentido == 0){ //si vamos a la derecha
					posiciones[a][0] = posiciones[a][0] + 1;
				}
				else{ //mover a la izquierda 
					posiciones[a][0] = posiciones[a][0] - 1;
				}
    }
}
void check_enemies(uint8_t bullet_x,uint8_t bullet_y,uint8_t posicions[15][3]){ // verificar si la bala le dio a algún enemigo
	  for (int a = 0; a < 15; a++) { //checar todos los enemigos 
			if(bullet_x >= posicions[a][0] && bullet_x <= (posicions[a][0] + 10) && bullet_y >= (posicions[a][1] + 7) && posicions[a][2] == 1){ //si la bala esta en un rango de algun enemigo 
					posicions[a][2] = 0;  //marcar como eliminado el enemigo para desaparecerlo
				}
    }
	
}

void check_damage(uint8_t bullet_enemie_x,uint8_t bullet_enemie_y,uint8_t x_ship){ // verificar si la bala le dio a algún enemigo
			
	if(bullet_enemie_x >= x_ship && bullet_enemie_x <= (x_ship + width_ship) && bullet_enemie_y >= 31-height_ship ){ //si la bala esta en el rango de la nave 
		life = life-1; // restar una vida
	}
 	
}


   


void adc_configuration(){
	
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


 
 void TIMER0_IRQHandler(void){
	
	
		if((LPC_TIM0->IR & 0x01) == 0x01){//si el timmer ha llegado al valor mr0
				
			  LPC_TIM0->IR |= 1 << 0; //reiniciar contador limpiando la bandera de interrupcion
				
			set_direction();
		
					updateLCD = 1; //asignar 1 para la bandera de refresco de pantalla
					x_ship =  ADC_value(ADC4)/550; //valor de x mapeado al adc (0-127)
				

				
				if(dis_en < 13){
				  dis_en = dis_en + 1;
				}

				else{
					bullet_enemie = 1;
					dis_en = 0;
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
	

  /*Inicializar nave en posicion inicial*/
	draw_ship(buffer, x_ship, y_ship);
 
 /*Inicializar enemigos*/
 
  draw_all_enemies(buffer,Enemies);
 
	// Transfer buffer to LCD
	SPILCD_Transfer(spiDrv1, buffer);
	
	//inicializar el ADC
	adc_configuration();
	
	i = 32-height_ship;
	
	do{
		random =  (rand() % (upper - lower + 1)) + lower; //enemigo random para disparar
		x_bullet_enemie = Enemies[random][0]; //coordenada en x
		j = Enemies[random][1]; //coordenada en y
	}while((Enemies[random][2] != 1)); //solo si ese enemigo esta vivo
	x_bullet = x_ship;
	while(1){
		
	
		
		
		if(pause == 1){ //pantalla de pausa
			Buffer_Reset(buffer);
			draw_pause_screen(buffer);
			SPILCD_Transfer(spiDrv1, buffer); 	// Transfer buffer to LCD
		}
		else if(updateLCD == 1 && life > 0 && pause == 0){ //si tenemos bandera de actualizacion de pantalla y si tenemos vida
		
			updateLCD = 0; //limpiar bandera
					//actualizar pantalla
			  Buffer_Reset(buffer);
				//set_direction();
			 //x_ship =  ADC_value(ADC4)/550; //valor de x mapeado al adc (0-127)

			

			 
			if(bullet_enemie == 1){ //si es momento de disparar
				
				if(j<31){
				 j++;
				 check_damage(x_bullet_enemie,j,x_ship);
				 Buffer_DrawRect(buffer, x_bullet_enemie, j, width_bullet, height_bullet); //bala de jugador (+4)
				}
				else{
					bullet_enemie = 0; //limpiar bandera de bala
						do{
						random =  (rand() % (upper - lower + 1)) + lower; //enemigo random para disparar
						x_bullet_enemie = Enemies[random][0]; //coordenada en x
						j = Enemies[random][1]; //coordenada en y
					}while((Enemies[random][2] != 1)); //solo si ese enemigo esta vivo
				}
				
			}
			
			if(bullet == 1){ //si presionamos por el boton de enmedio es decir si disparamos 
				 if(i == 32-height_ship){
				 i--;
				 x_bullet = x_ship+4;
				 check_enemies(x_bullet,i,Enemies);
				 Buffer_DrawRect(buffer, x_bullet, i, width_bullet, height_bullet); //bala de jugador (+4)
				}
				else if(i < 32-height_ship && i > 0){ //si aun no llega al limite superior
				 i--;
				 check_enemies(x_bullet,i,Enemies);
				 Buffer_DrawRect(buffer, x_bullet, i, width_bullet, height_bullet); //bala de jugador (+4)
					
				}

				else{
					x_bullet = x_ship+4; //poner la bala en el centro de la nave
					bullet = 0; //limpiar bandera de bala
					i = 32-height_ship; //inicializar i 
				}

			}
			
			/*Rangos para enemigos*/
			if(Enemies[0][0] >= (127-width_enemies)){ //si nos salimos del borde
				sentido_enemies = 1; //cambiar sentido a la izquierda
			}
			if(Enemies[0][0] == 0){ //si nos salimos del borde
				sentido_enemies = 0; //cambiar sentido a la derecha
			}
			
			/*Establecer limites en pantalla para x y 'y'*/
			if(x_ship >= (127-width_ship)){ //si lo supera en x
				x_ship = 127-width_ship; //regresar el rectangulo al limite
			}
			

			//mover a los enemigos cada tiempo 
			move_enemies(sentido_enemies,Enemies);

			//Buffer_DrawRect(buffer, x, y, width, height); 
			draw_ship(buffer, x_ship, y_ship); //actualizar la nave del jugador
			//actualizar enemigos
			draw_all_enemies(buffer,Enemies);			
			SPILCD_Transfer(spiDrv1, buffer); 	// Transfer buffer to LCD
	
		}

	
	
	}
}
