#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdint.h>

// Set all positions in buffer to zero
void Buffer_Reset(uint8_t* buffer);

// Draw pixel in buffer at (x,y)
void Buffer_SetPixel(uint8_t* buffer, uint8_t x, uint8_t y);

// Draw horizontal line in buffer from (xi,y) to (xf,y)
void Buffer_DrawLineH(uint8_t* buffer, uint8_t xi, uint8_t xf, uint8_t y);

// Draw vertical line in buffer from (x,yi) to (x,yf)
void Buffer_DrawLineV(uint8_t* buffer, uint8_t x, uint8_t yi, uint8_t yf);

// Draw rectangle in buffer at (x,y) with size (width,height)
void Buffer_DrawRect(uint8_t* buffer, uint8_t x, uint8_t y, uint8_t width, uint8_t height);

//dibujar un enemigo
void draw_enemy_one(uint8_t* buffer, uint8_t x, uint8_t y);

//dibujar la nave
void draw_ship(uint8_t* buffer, uint8_t x, uint8_t y);

//dibujar todos los enemigos
void draw_all_enemies(uint8_t* buffer,uint8_t posiciones[15][3]);

//pantalla de pausa/inicio
void draw_pause_screen(uint8_t* buffer);

//dibujar pantalla de game over
void draw_gameOver(uint8_t* buffer);

//dibujar pantalla de ganar
void draw_gameWin(uint8_t* buffer);
#endif /* BUFFER_H_ */
