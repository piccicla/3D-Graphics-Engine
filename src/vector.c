#include <math.h>
#include "vector.h"


// vector magnitude
float vec2_length(vec2_t v){
	return sqrt(v.x * v.x + v.y * v.y);
}
float vec3_length(vec3_t v){
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

//vector addition/subtraction/multiplication/division
vec2_t vec2_add(vec2_t a, vec2_t b){
	vec2_t result =	{ 	.x = a.x + b.x ,
						.y = a.y + b.y 
					};
	return result;
}
vec3_t vec3_add(vec3_t a, vec3_t b){
	vec3_t result = { 	.x = a.x + b.x,
						.y = a.y + b.y, 
						.z = a.z + b.z  
					};
	return result;	
}

vec2_t vec2_sub(vec2_t a, vec2_t b){
	vec2_t result = { 	.x = a.x - b.x,
						.y = a.y - b.y 
					};
	return result;	
}
vec3_t vec3_sub(vec3_t a, vec3_t b){
	vec3_t result = { 	.x = a.x - b.x,
						.y = a.y - b.y,
						.z = a.z - b.z
					};
	return result;	
}

vec2_t vec2_mul(vec2_t a, float scale){
	vec2_t result =	{	.x = a.x * scale,
						.y = a.y * scale
					};
	return result;
}
vec3_t vec3_mul(vec3_t a, float scale){
	vec3_t result =	{	.x = a.x * scale,
						.y = a.y * scale,
						.z = a.z * scale
					};
	return result;	
}
vec2_t vec2_div(vec2_t a, float scale){
	vec2_t result =	{	.x = a.x / scale,
						.y = a.y / scale
					};
	return result;
}
vec3_t vec3_div(vec3_t a, float scale){
	vec3_t result =	{	.x = a.x / scale,
						.y = a.y / scale,
						.z = a.z / scale
					};
	return result;	
}

// vector crossproduct
vec3_t vec3_cross(vec3_t a, vec3_t b){
	vec3_t result = { 	.x = (a.y * b.z) - (a.z * b.y),
						.y = (a.z * b.x) - (a.x * b.z), 
						.z = (a.x * b.y) - (a.y * b.x)  
					};
	return result;	
}
// float dot product
float vec2_dot(vec2_t a, vec2_t b){
	return (a.x * b.x) + (a.y * b.y);
}
float vec3_dot(vec3_t a, vec3_t b){
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

// rotate vectors
vec3_t vec3_rotate_x(vec3_t v, float angle){
	vec3_t rotated_vector = {
		.x = v.x,
		.y = v.y * cos(angle) - v.z * sin(angle),
		.z = v.y * sin(angle) + v.z * cos(angle)
	};
	return rotated_vector;
}
vec3_t vec3_rotate_y(vec3_t v, float angle){
	vec3_t rotated_vector = {
		.x = v.x * cos(angle) + v.z * sin(angle),
		.y = v.y,
		.z = -v.x * sin(angle) + v.z * cos(angle)		
	};
	return rotated_vector;	
}
vec3_t vec3_rotate_z(vec3_t v, float angle){
	vec3_t rotated_vector = {
		.x = v.x * cos(angle) - v.y * sin(angle),
		.y = v.x * sin(angle) + v.y * cos(angle),
		.z = v.z
	};
	return rotated_vector;
}


// normalization
void vec2_normalize(vec2_t *v){
	float magn = vec2_length(*v);
	v->x = v->x/magn;
	v->y = v->y/magn;
}
void vec3_normalize(vec3_t *v){
	float magn = vec3_length(*v);
	v->x = v->x/magn;
	v->y = v->y/magn;
	v->z = v->z/magn;	
}

// vector conversion functions
vec4_t vec4_from_vec3(vec3_t v){
	vec4_t result = {v.x, v.y, v.z, 1.0};
	return result;	
}
vec3_t vec3_from_vec4(vec4_t v){
	vec3_t result = {v.x, v.y, v.z};
	return result;	
}
