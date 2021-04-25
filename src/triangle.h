#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>
#include "vector.h"
#include "display.h"
#include "texture.h"

typedef struct {
	int a;
	int b;
	int c;
	tex2_t a_uv; //UV texture coordinates
	tex2_t b_uv;
	tex2_t c_uv;
	uint32_t color;
} face_t;

typedef struct {
	vec2_t points[3];
	tex2_t textcoords[3];
	uint32_t color;
	float avg_depth;
} triangle_t;


void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

//TODO: void draw_textured_triangle(....);


void int_swap(int* a, int* b);
void fill_flat_bottom_triangle(int x0, int y0,int  x1, int y1, int x2, int y2, uint32_t color);
void fill_flat_top_triangle(int x0, int y0,int  x1, int y1, int x2, int y2, uint32_t color);

#endif