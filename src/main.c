#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "array.h"
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"
#include "matrix.h"
#include "light.h"

//####globals######
//global variables for execution status and game loop
vec3_t camera_position = { .x = 0, .y = 0, .z = 0};
//vec3_t cube_rotation = {.x = 0, .y = 0, .z = 0};
bool is_running;
int previous_frame_time = 0;
bool animate_grid = false;
//float fov_factor = 1000;  //increase to increase the objects size
mat4_t proj_matrix;

//###################
//const int N_POINTS = 9*9*9; //not compiling, so we use #define N_POINTS 
//#define N_POINTS (9 * 9 * 9)
//vec3_t cube_points[N_POINTS];
//vec2_t projected_points[N_POINTS];
//###################

//array of tringles that should be rendered frame by frame
/////triangle_t triangles_to_render[N_MESH_FACES];
triangle_t* triangles_to_render = NULL;

//#################

void setup(void){
	
	// initialize render mode
	render_method = RENDER_WIRE;
	cull_method = CULL_BACKFACE;
	
	color_buffer =(uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);
	
	
	//initiakize perspective projection matrix
	float fov = M_PI/3.0;  //60 degrees in radians
	float aspect = (float)window_height/(float)window_width;
	float znear = 0.1;
	float zfar = 100.0;
	proj_matrix = mat4_make_perspective(fov, aspect, znear, zfar);
	
	
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

	//load_cube_mesh_data();  //setup cube data
	load_obj_file_data("./assets/f22.obj");
	//load_obj_file_data("./assets/cube.obj");

};

/*perspective projection*/
/*vec2_t project(vec3_t point){
	
	vec2_t projected_point = {
		.x = (fov_factor * point.x)/point.z,
		.y = (fov_factor * point.y)/point.z
	};
	return projected_point;
}*/

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
			if (event.key.keysym.sym == SDLK_1)
				render_method = RENDER_WIRE_VERTEX;
			if (event.key.keysym.sym == SDLK_2)
				render_method = RENDER_WIRE;
			if (event.key.keysym.sym == SDLK_3)
				render_method = RENDER_FILL_TRIANGLE;
			if (event.key.keysym.sym == SDLK_4)
				render_method = RENDER_FILL_TRIANGLE_WIRE;
			if (event.key.keysym.sym == SDLK_5)
				render_method = RENDER_TEXTURED_WIRE;
			if (event.key.keysym.sym == SDLK_6)
				render_method = RENDER_TEXTURED_WIRE;			
			if (event.key.keysym.sym == SDLK_c)
				cull_method = CULL_BACKFACE;			
			if (event.key.keysym.sym == SDLK_d)
				cull_method = CULL_NONE;				
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
	
	
	//initialize the array of tringles to render
	triangles_to_render = NULL;
	
	

	// pulsating grid
	if(animate_grid){ 
		if (grid_growth==0){
			grid_space++;
			if (grid_space==max_grid) {grid_growth=1;}
		}
		else{
			grid_space--;
			if (grid_space==min_grid) {grid_growth=0;}			
		}
	}
	
	mesh.rotation.x += 0.01;
	mesh.rotation.y += 0.01;
	mesh.rotation.z += 0.01;
	
	//mesh.scale.x += 0.001;
	//mesh.scale.y += 0.001;
	
	//mesh.translation.x += 0.005;
	mesh.translation.z = 5;
	
	// use matrixes to scale, translate, and rotate a mesh
	mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z); 
	mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x); 
	mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y); 
    mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z); 	
	
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
	int num_faces = array_length(mesh.faces);
	///for (int i=0; i<N_MESH_FACES; i++){
	for (int i=0; i<num_faces; i++){	
		////face_t mesh_face = mesh_faces[i];
		face_t mesh_face = mesh.faces[i];
		
		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];//-1 because array index start at zero
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];
		
		vec4_t transformed_vertices[3]; // will be used to store the 3 transformed vertices before back culling
		
		
		//loop 3 vertices and applytransformation
		for (int j=0; j<3; j++){
			
			vec4_t transformed_vertex =  vec4_from_vec3(face_vertices[j]);
		
			//use a matrix to scale,rotate, and translate (order is important!)
			///transformed_vertex = mat4_mul_vec(scale_matrix, transformed_vertex);
			///transformed_vertex = mat4_mul_vec(rotation_matrix_x, transformed_vertex);
			///transformed_vertex = mat4_mul_vec(rotation_matrix_y, transformed_vertex);
			///transformed_vertex = mat4_mul_vec(rotation_matrix_z, transformed_vertex);
			///transformed_vertex = mat4_mul_vec(translation_matrix, transformed_vertex);
			///transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
			////transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
			///transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);
			
			
			//create a world matrix to scale,rotate, and translate vertex (order matters)
			mat4_t world_matrix = mat4_identity();
			world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
			world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);
			transformed_vertex = mat4_mul_vec(world_matrix, transformed_vertex);
			
			//move away points from the camera_position (which is at 0,0,0)
			////point.z -= camera_position.z;
			//transformed_vertex.z += 5;
			
			// we save the vertex
			transformed_vertices[j] = transformed_vertex;
		}
		
		
		vec3_t  vector_a = vec3_from_vec4(transformed_vertices[0]);	/*	 A  clockwise triangle */
		vec3_t  vector_b = vec3_from_vec4(transformed_vertices[1]);	/*  / \	 */
		vec3_t  vector_c = vec3_from_vec4(transformed_vertices[2]);	/* C---B */
		
		// get subtraction b-a and c-a
		vec3_t vector_ab = vec3_sub(vector_b, vector_a);
		vec3_t vector_ac = vec3_sub(vector_c, vector_a);
		vec3_normalize(&vector_ab);
		vec3_normalize(&vector_ac);
		
		// take the cross product to get the trangle normal, this engine is left-handed
		vec3_t normal = vec3_cross(vector_ab, vector_ac);
		vec3_normalize(&normal);


		// find ray between point A and camera 
		vec3_t camera_ray =vec3_sub(camera_position, vector_a);
			
		//do Backculling
		if (cull_method == CULL_BACKFACE){	
			//render the face only if angle between camera-ray and normal is < 90degrees
			if (vec3_dot(normal,camera_ray)<0){
				continue;  // go back to for(int i=0; i<num_faces; i++)
			}
		}
		
		///triangle_t projected_triangle;
		vec4_t projected_points[3];
		
		// finally project projection from 3d to 2d screen
		for (int j=0; j<3; j++){
		
			////vec2_t projected_point = project(point);
			///vec2_t projected_point = project(transformed_vertices[j]);
			///projected_points[j] = project(vec3_from_vec4(transformed_vertices[j])); 
			projected_points[j] = mat4_mul_vec4_project(proj_matrix, transformed_vertices[j]);
			
			
			// scale into the view
			projected_points[j].x *= (window_width/2.0);
			projected_points[j].y *= (window_height/2.0);
			
			// translate projected points to the middle of the screen
			//projected_point.x += (window_width/2);
			//projected_point.y += (window_height/2);
			projected_points[j].x += (window_width/2.0);
			projected_points[j].y += (window_height/2.0);
			
			}
			
			// calculate average depth on each face based on vertices after transformation
			float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z)/3.0;
			
			//calculate shade intensity based on angle between face normal and light direction
			float light_intensity_factor = -vec3_dot(normal, light.direction); //negative to inverse the light direction
			
			//calculate the triangle color based on the light angle
			uint32_t triangle_color = light_apply_intensity(mesh_face.color, light_intensity_factor);
			
			////projected_triangle.points[j]  = projected_point;
			triangle_t projected_triangle = {
				.points = {
					{projected_points[0].x, projected_points[0].y},
					{projected_points[1].x, projected_points[1].y},
					{projected_points[2].x, projected_points[2].y}
				},
				.color = triangle_color,
				.avg_depth= avg_depth
			};
			
		///}
	
		// save projected triangle in the array of triangles to render
		////triangles_to_render[i] = projected_triangle;
		array_push(triangles_to_render, projected_triangle);
			
	}
	
	//todo sort triangles to render by their avg_depth()
	// using simple bubble sort
	
	int num_triangles = array_length(triangles_to_render);
	for(int i=0; i<num_triangles; i++){
		for(int j=i; j<num_triangles; j++){
			if(triangles_to_render[i].avg_depth < triangles_to_render[j].avg_depth){
				triangle_t temp = triangles_to_render[i];
				triangles_to_render[i] = triangles_to_render[j];
				triangles_to_render[j] = temp;
				
			}
		}	
	}
	
	
	
};

void render(void){
	//SDL_SetRenderDrawColor(renderer, 0,0,0,255);
	//SDL_RenderClear(renderer);
	
	clear_color_buffer(0xFF000000);
	
	draw_grid(grid_space,0xFF909090);	
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
	
	
	// draw  triangles
	int num_triangles = array_length(triangles_to_render);
	//for (int i=0; i<N_MESH_FACES; i++){
	for (int i=0; i<num_triangles; i++){	
		triangle_t triangle = triangles_to_render[i];
		
		//draw texture triangle
		if (render_method==RENDER_TEXTURED || render_method==RENDER_TEXTURED_WIRE){
			//TODO: draw_textured_triangle{}	
		}
			
		if (render_method==RENDER_WIRE_VERTEX){
			draw_rectangle(triangle.points[0].x-4,triangle.points[0].y-4, 8, 8, 0xFFFF0000);
			draw_rectangle(triangle.points[1].x-4,triangle.points[1].y-4, 8, 8, 0xFFFF0000);
			draw_rectangle(triangle.points[2].x-4,triangle.points[2].y-4, 8, 8, 0xFFFF0000);
		}
		if (render_method==RENDER_FILL_TRIANGLE || render_method==RENDER_FILL_TRIANGLE_WIRE){
			draw_filled_triangle(	triangle.points[0].x,triangle.points[0].y,
							triangle.points[1].x,triangle.points[1].y,
							triangle.points[2].x,triangle.points[2].y, 
							///0xFFFFFF00
							triangle.color
							);
		}
		
		if (render_method==RENDER_WIRE || render_method==RENDER_WIRE_VERTEX || render_method==RENDER_FILL_TRIANGLE_WIRE || render_method==RENDER_TEXTURED_WIRE){
			draw_triangle(	triangle.points[0].x,triangle.points[0].y,
							triangle.points[1].x,triangle.points[1].y,
							triangle.points[2].x,triangle.points[2].y, 
							0xFFFF0000);
		}
	}
	
	//draw_filled_triangle(	300,100,50,400,500,700, 0xFFFF0000);
	

	//clear array of triangles to render after every frame loop
	array_free(triangles_to_render);
			
	render_color_buffer();

	SDL_RenderPresent(renderer);
};

////clean dynamic memory
void free_resources(void){
	free(color_buffer);
	array_free(mesh.vertices);
	array_free(mesh.faces);
}

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
	free_resources();
    return 0;    
}

