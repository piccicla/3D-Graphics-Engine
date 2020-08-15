#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>


#define FPS 60  //number of frames per second
#define FRAME_TARGET_TIME (1000 / FPS)  //duration of each frame in milliseconds

// RENDERING OPTIONS
enum cull_method {
	CULL_NONE,
	CULL_BACKFACE	
} cull_method;

enum render_method {
	RENDER_WIRE,
	RENDER_WIRE_VERTEX,
	RENDER_FILL_TRIANGLE,
	RENDER_FILL_TRIANGLE_WIRE	
} render_method;


//####globals implemented in main.c######
extern int window_width;   
extern int window_height;

extern uint32_t *color_buffer;
extern SDL_Texture *color_buffer_texture;

extern SDL_Window *window;
extern SDL_Renderer *renderer;

extern const int min_grid;
extern const int max_grid;
extern int grid_space;
extern int grid_growth; //0 is expanding grid, 1 is contracting

//##############################

bool initialize_window(void);
void destroy_window(void);

void clear_color_buffer(uint32_t color);
void render_color_buffer(void);
void draw_grid(int space, uint32_t grid_color);
void draw_pixel(int x, int y, uint32_t pix_color);
void draw_rectangle(int x, int y, int width, int height,uint32_t rect_color);
void draw_line(int x0, int y0, int x1, int y1, uint32_t pix_color);
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t pix_color);
#endif