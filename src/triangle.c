#include "triangle.h"


/*///////////////////////////////////////////////////////////////////////////////
// Draw a filled a triangle with a flat bottom
///////////////////////////////////////////////////////////////////////////////
//
//        (x0,y0)
//          / \
//         /   \
//        /     \
//       /       \
//      /         \
//  (x1,y1)------(x2,y2)
//
///////////////////////////////////////////////////////////////////////////////*/
void fill_flat_bottom_triangle(int x0, int y0,int  x1, int y1, int x2, int y2, uint32_t color){
	
	// find 2 slopes (how x changes when y changes)
	float inv_slope_1 = (float)(x1 - x0) / (y1 - y0);
	float inv_slope_2 = (float)(x2 - x0) / (y2 - y0);
	
	// loop all the scanlines from top to bottom and calculate x based on y using the slope
	float x_start = x0;
	float x_end = x0;
	for ( int y = y0; y <= y2; y++){
		
		draw_line(x_start, y, x_end, y, color);
		x_start += inv_slope_1;
		x_end += inv_slope_2;

	}
	
}
/*///////////////////////////////////////////////////////////////////////////////
// Draw a filled a triangle with a flat top
///////////////////////////////////////////////////////////////////////////////
//
//  (x0,y0)------(x1,y1)
//      \         /
//       \       /
//        \     /
//         \   /
//          \ /
//        (x2,y2)
//
///////////////////////////////////////////////////////////////////////////////*/
void fill_flat_top_triangle(int x0, int y0,int  x1, int y1, int x2, int y2, uint32_t color){
	// find 2 slopes (how x changes when y changes)
	float inv_slope_1 = (float)(x0 - x2) / (y2 - y0);
	float inv_slope_2 = (float)(x1 - x2) / (y2 - y0);
	
	// loop all the scanlines from bottom to top and calculate x based on y using the slope
	float x_start = x2;
	float x_end = x2;
	for ( int y = y2; y >= y0; y--){
		
		draw_line(x_start, y, x_end, y, color);
		x_start += inv_slope_1;
		x_end += inv_slope_2;

	}		
}
/*
///////////////////////////////////////////////////////////////////////////////
// Draw a filled triangle with the flat-top/flat-bottom method
// We split the original triangle in two, half flat-bottom and half flat-top
///////////////////////////////////////////////////////////////////////////////
//
//          (x0,y0)
//            / \
//           /   \
//          /     \
//         /       \
//        /         \
//   (x1,y1)------(Mx,My)
//       \_           \
//          \_         \
//             \_       \
//                \_     \
//                   \    \
//                     \_  \
//                        \_\
//                           \
//                         (x2,y2)
//
///////////////////////////////////////////////////////////////////////////////*/
void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color){
	
	// 1) sort vertices by y-coordinate ascending y0<y1<y2
	if (y0 > y1){
		int_swap(&y0,&y1);
		int_swap(&x0,&x1);
	}
	if (y1 > y2){
		int_swap(&y1,&y2);
		int_swap(&x1,&x2);
	}
	if (y0 > y1){
		int_swap(&y0,&y1);
		int_swap(&x0,&x1);
	}


    if (y1 == y2) {
        // Draw flat-bottom triangle
        fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
    } else if (y0 == y1) {
        // Draw flat-top triangle
        fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
    } else {
        // Calculate the new vertex (Mx,My) using triangle similarity
        int My = y1;
        int Mx = (((x2 - x0) * (y1 - y0)) / (y2 - y0)) + x0;

        // Draw flat-bottom triangle
        fill_flat_bottom_triangle(x0, y0, x1, y1, Mx, My, color);

        // Draw flat-top triangle
        fill_flat_top_triangle(x1, y1, Mx, My, x2, y2, color);
    }
	
}

/*
///////////////////////////////////////////////////////////////////////////////
// Draw a textured triangle with the flat-top/flat-bottom method
// We split the original triangle in two, half flat-bottom and half flat-top
///////////////////////////////////////////////////////////////////////////////
//
//          (x0,y0, u0,v0)
//            / \
//           /   \
//          /     \
//         /       \
//        /         \
//   (x1,y1,u1,v1)--(Mx,My)
//       \_           \
//          \_         \
//             \_       \
//                \_     \
//                   \    \
//                     \_  \
//                        \_\
//                           \
//                         (x2,y2,u2,v2)
//
///////////////////////////////////////////////////////////////////////////////*/
void draw_textured_triangle(int x0, int y0, float u0, float v0,
							int  x1, int y1, float u1, float v1,
							int x2, int y2, float u2, float v2,
							uint32_t* texture){
								
	// We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
    if (y0 > y1) {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
        float_swap(&u0, &u1);
        float_swap(&v0, &v1);
    }
    if (y1 > y2) {
        int_swap(&y1, &y2);
        int_swap(&x1, &x2);
        float_swap(&u1, &u2);
        float_swap(&v1, &v2);
    }
    if (y0 > y1) {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
        float_swap(&u0, &u1);
        float_swap(&v0, &v1);
    }	
	
	//create vector point
	vec2_t point_a = { x0, y0};
    vec2_t point_b = { x1, y1};
	vec2_t point_c = { x2, y2};

    ///////////////////////////////////////////////////////
    // Render the upper part of the triangle (flat-bottom)
    ///////////////////////////////////////////////////////
    float inv_slope_1 = 0;
    float inv_slope_2 = 0;

    if (y1 - y0 != 0) inv_slope_1 = (float)(x1 - x0) / abs(y1 - y0);
    if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

    if (y1 - y0 != 0) {
        for (int y = y0; y <= y1; y++) {
            int x_start = x1 + (y - y1) * inv_slope_1;
            int x_end = x0 + (y - y0) * inv_slope_2;

            if (x_end < x_start) {
                int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
            }

            for (int x = x_start; x < x_end; x++) {
                // Draw our pixel with a custom color
                //draw_pixel(x, y, (x % 2 == 0 && y % 2 == 0) ? 0xFFFF00FF : 0x00000000);
				draw_texel( x, y, texture, point_a, point_b, point_c, u0, v0, u1, v1, u2, v2);

            }
        }
    }

	///////////////////////////////////////////////////////
    // Render the bottom part of the triangle (flat-top)
    ///////////////////////////////////////////////////////
    inv_slope_1 = 0;
    inv_slope_2 = 0;

    if (y2 - y1 != 0) inv_slope_1 = (float)(x2 - x1) / abs(y2 - y1);
    if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

    if (y2 - y1 != 0) {
        for (int y = y1; y <= y2; y++) {
            int x_start = x1 + (y - y1) * inv_slope_1;
            int x_end = x0 + (y - y0) * inv_slope_2;

            if (x_end < x_start) {
                int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
            }

            for (int x = x_start; x < x_end; x++) {
                /// Draw our pixel with a custom color
                //draw_pixel(x, y, (x % 2 == 0 && y % 2 == 0) ? 0xFFFF00FF : 0x00000000);
				draw_texel( x, y, texture, point_a, point_b, point_c, u0, v0, u1, v1, u2, v2);
            }
        }
    }

	
}

/*
///////////////////////////////////////////////////////////////////////////////
//return barycentric weights for point P inside triangle ABC
///////////////////////////////////////////////////////////////////////////////
//
//             A
//            /|\
//           / | \
//          /  |  \
//         /   |   \
//        /gamm|    \
//      B -----P     \
//       \alpha \_beta\
//          \_    \_   \
//             \_   \_  \
//                \_  \_ \
//                   \  \_\
//                     \_ \\
//                        \_\
//                           \
//                            C
//
///////////////////////////////////////////////////////////////////////////////*/
vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p){
	
	vec2_t ab = vec2_sub(b, a);
	vec2_t bc = vec2_sub(c, b);
	vec2_t ac = vec2_sub(c, a);
	vec2_t ap = vec2_sub(p, a);
	vec2_t bp = vec2_sub(p, b);	
	
	//area parallelogram using cross product
	float area_parallelogram_abc = ( ab.x * ac.y - ab.y * ac.x );
	
	float alpha = ( bc.x * bp.y - bp.x * bc.y ) / area_parallelogram_abc;
	float beta = ( ap.x * ac.y - ac.x * ap.y ) / area_parallelogram_abc;
	float gamma = 1- alpha - beta;
	
	vec3_t weights = { alpha, beta, gamma};
	
	return weights;
}

/*
/////////////////////////////////////////////////////////
// Function to draw texture pixel at location x,y using interpolation
*/
void draw_texel( int x, int y, uint32_t* texture, vec2_t point_a, vec2_t point_b,  vec2_t point_c, float u0, float v0, float u1, float v1, float u2, float v2){
	
	vec2_t point_p = { x, y};
	
	vec3_t weights = barycentric_weights(point_a, point_b, point_c, point_p);
	
	float alpha = weights.x;
	float beta = weights.y;
	float gamma = weights.z;	
	
	float interpolated_u = (u0) * alpha + (u1) * beta + (u2) * gamma;
	float interpolated_v = (v0) * alpha + (v1) * beta + (v2) * gamma;
	
	int tex_x = abs((int)(interpolated_u * texture_width));
	int tex_y = abs((int)(interpolated_v * texture_height));
	
	draw_pixel(x, y, texture[(texture_width * tex_y) + tex_x]);
}






