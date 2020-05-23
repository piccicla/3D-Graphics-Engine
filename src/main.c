#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"

//####globals######


//###################
//const int N_POINTS = 9*9*9; //not compiling, so we use #define N_POINTS 
#define N_POINTS (9 * 9 * 9)
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];
//###################


triangle_t triangles_to_render[N_MESH_FACES];


float fov_factor = 1000;  //increase to increase the objects size

vec3_t camera_position = { .x = 0, .y = 0, .z = -5};

vec3_t cube_rotation = {.x = 0, .y = 0, .z = 0};

bool is_running;

int previous_frame_time = 0;

//#################

void setup(void){
	
	color_buffer =(uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);
	
	
	/////
	/*int point_count=0;
	for (float x=-1; x<=1; x+=0.25){
		for (float y=-1; y<=1; y+=0.25){
			for (float z=-1; z<=1; z+=0.25){
				vec3_t new_point = {.x=x, .y=y, .z=z};
				cube_points[point_count++] = new_point;
			}
		}
	}*/

};

/*perspective projection*/
vec2_t project(vec3_t point){
	
	vec2_t projected_point = {
		.x = (fov_factor * point.x)/point.z,
		.y = (fov_factor * point.y)/point.z
	};
	return projected_point;
}

void process_input(void){
	SDL_Event event;
	SDL_PollEvent(&event);
	switch(event.type){
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				is_running = false;
			break;
			
	}
};

void update(void){
	
	
	//lock until frame_target_time is passed and then store the current time
	//todo: also check the deltatime approach
	
	// the while will consume100% cpu, it's better to use the OS independent SDL_Delay()
    ///while (!SDL_TICKS_PASSED(SDL_GetTicks(), previous_frame_time + FRAME_TARGET_TIME));	
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
	if (time_to_wait>0 && time_to_wait<=FRAME_TARGET_TIME){
		SDL_Delay(time_to_wait);
	}
	previous_frame_time = SDL_GetTicks();
	
	

	// pulsating grid
	if (grid_growth==0){
		grid_space++;
		if (grid_space==max_grid) {grid_growth=1;}
	}
	else{
		grid_space--;
		if (grid_space==min_grid) {grid_growth=0;}		
		
	}
	
	cube_rotation.x += 0.01;
	cube_rotation.y += 0.01;
	cube_rotation.z += 0.01;
	
	/*// project the cube
	for (int i=0; i<N_POINTS; i++){
		vec3_t point = cube_points[i];
		
		vec3_t transformed_point = vec3_rotate_x(point, cube_rotation.x);
		transformed_point = vec3_rotate_y(transformed_point, cube_rotation.y);
		transformed_point = vec3_rotate_z(transformed_point, cube_rotation.z);		
		
		
		//move away points from the camera_position
		////point.z -= camera_position.z;
		transformed_point.z -= camera_position.z;
		
		////vec2_t projected_point = project(point);
		vec2_t projected_point = project(transformed_point);
		projected_points[i]  = projected_point;
		
	}*/
	
	//loop triangle faces
	for (int i=0; i<N_MESH_FACES; i++){
		face_t mesh_face = mesh_faces[i];
		
		vec3_t face_vertices[3];
		face_vertices[0] = mesh_vertices[mesh_face.a - 1];//-1 because array index start at zero
		face_vertices[1] = mesh_vertices[mesh_face.b - 1];
		face_vertices[2] = mesh_vertices[mesh_face.c - 1];
		
		
		triangle_t projected_triangle;
		//loop 3 vertices and applytransformation
		for (int j=0; j<3; j++){
			
			vec3_t transformed_vertex = face_vertices[j];
		
			transformed_vertex = vec3_rotate_x(transformed_vertex, cube_rotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, cube_rotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, cube_rotation.z);
			
			//move away points from the camera_position
			////point.z -= camera_position.z;
			transformed_vertex.z -= camera_position.z;
		
			////vec2_t projected_point = project(point);
			vec2_t projected_point = project(transformed_vertex);
			
			//scale and translate projected points to the middle of the screen
			projected_point.x += (window_width/2);
			projected_point.y += (window_height/2); 
			
			projected_triangle.points[j]  = projected_point;
		}
		// save projected triangle in the array of triangles to render
		triangles_to_render[i] = projected_triangle;
	}
	
	
};

void render(void){
	//SDL_SetRenderDrawColor(renderer, 0,0,0,255);
	//SDL_RenderClear(renderer);
	
	clear_color_buffer(0xFF000000);
	
	//draw_grid(grid_space,0xFFFFFFFF);	
	//draw_rectangle(100,100,200,200, 0xFFFF0000);	
	//draw_pixel(5500,5500,0xFFFF0000);
	
	/*for (int i=0; i<N_POINTS; i++){
	
		vec2_t projected_point  = projected_points[i];
		draw_rectangle(
			projected_point.x + (window_width/2) ,
			projected_point.y + (window_height/2) ,
			4,
			4,
			0xFFFF0000);
		
	}*/ 
	
	for (int i=0; i<N_MESH_FACES; i++){
		triangle_t triangle = triangles_to_render[i];
		draw_rectangle(triangle.points[0].x,triangle.points[0].y, 8, 8, 0xFFFF0000);
		draw_rectangle(triangle.points[1].x,triangle.points[1].y, 8, 8, 0xFFFF0000);
		draw_rectangle(triangle.points[2].x,triangle.points[2].y, 8, 8, 0xFFFF0000);
		
		draw_triangle(	triangle.points[0].x,triangle.points[0].y,
						triangle.points[1].x,triangle.points[1].y,
						triangle.points[2].x,triangle.points[2].y, 
						0xFFFF0000);

	}

		
	render_color_buffer();

	SDL_RenderPresent(renderer);
};

int main( int argc, char* args[] )
{	
	is_running = initialize_window();
	
	setup();
	
	while(is_running){
		process_input();
		update();
		render();
	}
	destroy_window();
    return 0;    
}

