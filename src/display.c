#include "display.h"
#include <math.h>

//display.h variables
int window_width = 800;
int window_height = 600;

uint32_t *color_buffer = NULL;
SDL_Texture *color_buffer_texture = NULL;

SDL_Window *window;
SDL_Renderer *renderer;

const int min_grid = 50;
const int max_grid = 100;
int grid_space = min_grid;
int grid_growth = 0; //0 is expanding grid, 1 is contracting

bool initialize_window(void){
	
	//Start SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) !=0){
		fprintf(stderr, "Error initializing SDL\n");
		return false;
	}
	
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	window_height = display_mode.h;
	window_width = display_mode.w;	
	
	// create window
	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_BORDERLESS
	);
    if (!window){
		fprintf(stderr, "Error creating SDL window\n");
		return false;
	}
		
	// create renderer
	renderer = SDL_CreateRenderer(window, -1, 0);
	    if (!renderer){
		fprintf(stderr, "Error creating SDL renderer\n");
		return false;
	}
	
	return true;
	
}

void destroy_window(void){
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
    SDL_Quit();
}

void clear_color_buffer(uint32_t color){
	
	for(int y=0; y<window_height; y++){
		for(int x=0; x<window_width; x++)
			color_buffer[(window_width * y) + x] = color;
	}
}

void render_color_buffer(void){
	
	SDL_UpdateTexture(
		color_buffer_texture,
		NULL, //SDL_Rect structure
		color_buffer,
		(int)(window_width * sizeof(uint32_t)) //number of bytes per row
	);
	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
	
}

void draw_grid(int space, uint32_t grid_color){
	
	//fill in horizontal lines
	/*for(int y=0; y<window_height; y++){
		if (y%space==0){
			for(int x=0; x<window_width; x++)
			{color_buffer[(window_width * y) + x] = grid_color;}
		}
	}
	//fill in the vertical lines
	for(int x=0; x<window_width; x++){
		if (x%space==0){
			for(int y=0; y<window_height; y++)
			{color_buffer[(window_width * y) + x] = grid_color;}
		}
	}*/
	//fill in horizontal lines
	for(int y=0; y<window_height; y+=space){
		for(int x=0; x<window_width; x++)
			{color_buffer[(window_width * y) + x] = grid_color;}	
	}
	//fill in the vertical lines
	for(int x=0; x<window_width; x+=space){
		for(int y=0; y<window_height; y++)
			{color_buffer[(window_width * y) + x] = grid_color;}	
	}
}

void draw_pixel(int x, int y, uint32_t pix_color){
	if (x>=0 && x < window_width && y>=0 && y < window_height) {
		color_buffer[(window_width * y) + x] = pix_color;
	}
	
}

void draw_rectangle(int x, int y, int width, int height,uint32_t rect_color){
	
	/*if (y< window_height){
	
		for(int y1=y; y1< (y+height); y1++){			
			if (y1>=window_height) break;
			for(int x1=x; x1<(x+width); x1++)
				{
					if (x1>=window_width) break;
					color_buffer[(window_width * y1) + x1] = rect_color;
				}	
		}
	}*/
	for(int y1=y; y1< (y+height); y1++){			
		for(int x1=x; x1<(x+width); x1++)
		{
			draw_pixel(x1, y1, rect_color);
		}	
	}
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t pix_color){
	
	int delta_x = (x1 - x0);
	int delta_y = (y1 - y0);
	
	int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);
	
	float x_inc = delta_x / (float)side_length;
	float y_inc = delta_y / (float)side_length;
	
	float current_x  = x0;
	float current_y  = y0;
	
	for(int i = 0; i <= side_length; i++){
		draw_pixel(round(current_x),round(current_y), pix_color);
		current_x  += x_inc;
		current_y  += y_inc;
		
	}
	
}


void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t pix_color){	
	draw_line(x0, y0, x1, y1, pix_color);
	draw_line(x1, y1, x2, y2, pix_color);
	draw_line(x2, y2, x0, y0, pix_color);	
}