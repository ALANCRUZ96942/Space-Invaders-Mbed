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

#endif /* BUFFER_H_ */
