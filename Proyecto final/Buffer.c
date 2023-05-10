#include "Buffer.h"
#include "SPI_LCD.h"

//codigo bueno
// Set all positions in buffer to zero
void Buffer_Reset(uint8_t* buffer) {
	// ------------------------------------------------------------
	int j;
	for( j = 0; j < LCD_MEM_SIZE; j++){ //limpiar el pixel en todos los espacios 
	 buffer[j] = 0x00;  //valor del pixel a 0
	}
	
	// ------------------------------------------------------------
}


// Draw pixel in buffer at (x,y)
void Buffer_SetPixel(uint8_t* buffer, uint8_t x, uint8_t y) {
	// ------------------------------------------------------------
	int sel = 0x00;
	int pagina = 0;
	
	if(y >= 0 && y < 8 ){  //y está en pagina 0
		y = y;
		sel = x  + (128*(pagina));
	}
	else if(y >= 8 && y < 16){ //y está en pagina 1
		y = y-8;
		pagina = 1;
		sel = x  + (128*(pagina));
	}
	else if(y >= 16 && y < 24){  //y está en pagina 2
		y = y-16;
		pagina = 2;
		sel = x  + (128*(pagina));
	}
	else if(y >= 24 && y < 32){ //y está en pagina 3
		y = y-24;
		pagina = 3;
		sel = x  + (128*(pagina));
	
	}
	
	buffer[sel] =  buffer[sel] | (0x01 << y); //poner el valor 1 del pixel en el valor de 'y' correspondiente
	

	// ------------------------------------------------------------
}

// Draw horizontal line in buffer from (xi,y) to (xf,y)
void Buffer_DrawLineH(uint8_t* buffer, uint8_t xi, uint8_t xf, uint8_t y) {
	// ------------------------------------------------------------
	// Add your code here ...
	int x; //variable iteradora
	
		for( x = xi; x <= xf; x++){ //bucle for que recorre todos los puntos del segmento dado de en x
		 Buffer_SetPixel(buffer, x, y); //coloca el pixel en la x iterada e y dada
		}
	
	// Note: you can use function Buffer_SetPixel() to draw the line
	// ------------------------------------------------------------
}

// Draw vertical line in buffer from (x,yi) to (x,yf)
void Buffer_DrawLineV(uint8_t* buffer, uint8_t x, uint8_t yi, uint8_t yf) {
	// ------------------------------------------------------------
	// Add your code here ...
	
		int y; //variable iteradora
	
		for( y = yi; y <= yf; y++){ //bucle for que recorre todos los puntos del segmento dado de en y
			Buffer_SetPixel(buffer, x, y); //coloca el pixel en la y iterada y x dada
		}
	// Note: you can use function Buffer_SetPixel() to draw the line
	// ------------------------------------------------------------
}

// Draw rectangle in buffer at (x,y) with size (width,height)
void Buffer_DrawRect(uint8_t* buffer, uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
	// ------------------------------------------------------------
	// Add your code here ...
	
		for(int j = y; j<=height+y;j++){
					Buffer_DrawLineH(buffer,x,x+width,j); //linea horizontal que va de la altura toal al ancho total
		}
	
		
		
	
	// Note: you can use function Buffer_SetPixel() to draw the rectangle
	// ------------------------------------------------------------
}

