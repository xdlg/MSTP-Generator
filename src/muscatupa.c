/**************************************************************************//**
 * @file
 * Multi-scale Turing patterns based on Jonathan McCabe's work.
 *****************************************************************************/

/******************************************************************************
 * #include
 *****************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>
#include <math.h>
#include "gifsave/gifsave89.c"
#include "utils.h"
#include "blur.h"

/******************************************************************************
 * #define
 *****************************************************************************/
#define COLOR_DEPTH 	256 /**< Number of color levels in the final image */
#define N_REPEATS 		0 	/**< 0: loop the gif animation */
#define T_FRAME 		4 	/**< *10 ms: timespan of a gif frame */
#define TRANSPARENCY	-1 	/**< -1: no transparency */
#define DISPOSAL 		1 	/**< Don't really know the influence of that */
#define PATH_TO_GIF		"0.gif"

#define N_SCALES 		4 	/**< Number of Turing patterns/scales */
#define W				300 /**< Image width */
#define H				300	/**< Image height */
#define N_STEPS 		200 /**< Number of timesteps to generate */

#define ANIMATE 		1 	/**< 0 to save only the last generated picture */

/******************************************************************************
 * Types and structures
 *****************************************************************************/
/** 
* One Turing pattern/one scale.
*/
struct pattern
{
	uint32_t act_r; /**< Activator radius */
	uint32_t inh_r; /**< Inhibitor radius */
	float sa;		/**< Small amount */
};

/******************************************************************************
 * Private variables
 *****************************************************************************/
/** 
* Parameters of the Turing patterns.
*/
const uint32_t act_r_all[N_SCALES] = {10, 25, 10, 5};	 /**< Activator radii */
const uint32_t inh_r_all[N_SCALES] = {20, 50, 20, 10};	 /**< Inhibitor radii */
const float sa_all[N_SCALES] = {0.05, 0.04, 0.03, 0.02}; /**< Small amounts */

/******************************************************************************
 * Private functions
 *****************************************************************************/
void init_image(uint32_t w, uint32_t h, float s[][h]);
void build_colormap(uint32_t depth, int32_t *colors);
void step(struct pattern *p, uint32_t n, uint32_t w, uint32_t h, float im[][h]);
void compute_var(uint32_t n, uint32_t w, uint32_t h, float act[][w][h], 
	float inh[][w][h], float var[][w][h], float *lst_var);
void update_image(struct pattern *p, uint32_t w, uint32_t h, float im[][h],
	float act[][h], float inh[][h]);
void normalize_image(uint32_t w, uint32_t h, float s[][h]);
void convert_image(uint32_t w, uint32_t h, float im_float[][h], 
	uint8_t im_bytes[][h]);
uint32_t write_gif(unsigned char *gif_image, uint32_t n_bytes);

/**************************************************************************//**
 * Turing pattern generator.
 * @return 0 when successful
 *****************************************************************************/
int main(void)
{
	// gifsave89 variables
	void *gs_data = NULL; 				// gifsave89 main variable
	int bg_index = 0;					// Background color index
	int32_t colors[3*COLOR_DEPTH + 1];	// Color map (RGB values + term. char)
	uint32_t n_bytes = 0;				// Number of bytes in the gif
	unsigned char *gif_image = NULL;	// Pointer to the buffer
	
	struct pattern p[N_SCALES];	// Array of Turing patterns
	float im_float[W][H];		// Image with values E[-1; 1]
	uint8_t im_bytes[W][H];		// Image with values E[0; 255]
	
	uint32_t i;
	
	init_image(W, H, im_float);
	build_colormap(COLOR_DEPTH, colors);
	
	// Initialize the patterns
	for (i=0; i<N_SCALES; i++)
	{
		p[i].act_r = act_r_all[i];
		p[i].inh_r = inh_r_all[i];
		p[i].sa = sa_all[i];
	}
	
	// Initialize GIF
	gs_data = newgif((void **)(&gif_image), W, H, (int *)colors, bg_index);
	
	if (gs_data != NULL) 
	{ 
		// Initialize animation
		if (ANIMATE) 
		{
			animategif(gs_data, N_REPEATS, T_FRAME, TRANSPARENCY, DISPOSAL);
		}
		
		// Build frames
		for (i=0; i<N_STEPS; i++)
		{
			printf("%d/%d\n", i+1, N_STEPS);
			step(p, N_SCALES, W, H, im_float);
			if (ANIMATE)
			{
				convert_image(W, H, im_float, im_bytes);
				putgif(gs_data, im_bytes);
			}
		}

		if (!ANIMATE)
		{
			convert_image(W, H, im_float, im_bytes);
			putgif(gs_data, im_bytes);
		}
		n_bytes = endgif(gs_data); 
	}
	
	// Write GIF to file
	if (n_bytes > 0) 
	{
		write_gif(gif_image, n_bytes);
		free(gif_image); 
	}

	return 0;
}

/**************************************************************************//**
 * Initialize the image with random values.
 * The w*h array is filled with values E[-1; 1].
 * @param w width of the image
 * @param h height of the image
 * @param s source image in a 2D array (overwritten)
 *****************************************************************************/
void init_image(uint32_t w, uint32_t h, float s[][h])
{
	uint32_t x, y;
	
	srand(time(NULL));
	for (x = 0; x < w; x++)
	{
		for (y = 0; y < h; y++)
		{
			s[x][y] = (float)rand() / (float)(RAND_MAX) * 2.0 - 1.0;
		}
	}
}

/**************************************************************************//**
 * Build the RGB colormap for gifsave89.
 * The colormap associates an RGB value for each pixel value. For instance,
 * with a pixel depth of 1 bit, the colormap could be:
 * {0, 0, 0, 255, 255, 255, -1} -> a pixel value of 0 is black, pixel 1 is
 * white, and the array is terminated with -1.
 * @param depth color depth
 * @param colors colormap array (overwritten)
 *****************************************************************************/
void build_colormap(uint32_t depth, int32_t *colors)
{
	uint32_t i;
	
	// Grayscale map
	for (i=0; i<depth; i++)
	{
		colors[3*i] = i;
		colors[3*i+1] = i;
		colors[3*i+2] = i;
	}
	colors[3*depth] = -1;
}

/**************************************************************************//**
 * One step of the main algorithm.
 * @param p array of Turing patterns
 * @param n number of Turing patterns
 * @param w width of the image
 * @param h height of the image
 * @param im image (overwritten)
 *****************************************************************************/
void step(struct pattern *p, uint32_t n, uint32_t w, uint32_t h, float im[][h])
{
	float act[n][w][h];	// Activator arrays
	float inh[n][w][w];	// Inhibitor arrays
	float var[n][w][h];	// Variation arrays
	float lst_var[n];	// Least variation for each scale
	float dummy;
	uint32_t i, min;
	
	// Generate the activator and inhibitor arrays (try with multiple passes)
	for (i=0; i<n; i++)
	{
		blur(w, h, im, act[i], (p+i)->act_r);
		blur(w, h, im, inh[i], (p+i)->inh_r);
	}
	
	// Find the scale with smallest variation
	compute_var(n, w, h, act, inh, var, lst_var);
	min = min_array(n, lst_var, &dummy);
	
	// Update the image with the scale that has just been found
	update_image((p+min), w, h, im, act[min], inh[min]);
	normalize_image(w, h, im);
}

/**************************************************************************//**
 * Compute the variation arrays for all scales.
 * The variation is simply |activator[x][y] - inhibitor[x][y]| for each pixel.
 * Normally there should be an averaging over a radius, but it's simpler to 
 * code that for a single-pixel radius and in theory it gives better pictures.
 * Although the smallest variation for each scale could be computed in another
 * function, it's more elegant to do it here since we're going through the
 * whole array anyway.
 * @param n number of Turing patterns
 * @param w width of the image
 * @param h height of the image
 * @param act activator arrays
 * @param inh inhibitor arrays
 * @param var variation arrays (overwritten)
 * @param lst_var least variations for all scales (overwritten)
 *****************************************************************************/
void compute_var(uint32_t n, uint32_t w, uint32_t h, float act[][w][h], 
	float inh[][w][h], float var[][w][h], float *lst_var)
{
	float min;
	uint32_t x, y;
	uint32_t i;
	
	for (i=0; i<n; i++)
	{
		min = 10; // Arbitrary value
		
		for (x=0; x<w; x++)
		{
			for (y=0; y<h; y++)
			{
				var[i][x][y] = fabs(act[i][x][y] - inh[i][x][y]);
				if (var[i][x][y] < min) min = var[i][x][y];
			}
		}
		
		lst_var[i] = min;
	}
}

/**************************************************************************//**
 * Update the image on one single scale.
 * Each pixel of the image is increased/decreased by a small amount depending
 * on the activator and inhibitor value at those coordinates. This function is
 * normally called with the parameters of the smallest-variation scale.
 * @param p a Turing pattern
 * @param w width of the image
 * @param h height of the image
 * @param im image (overwritten)
 * @param act activator array
 * @param inh inhibitor array
 *****************************************************************************/
void update_image(struct pattern *p, uint32_t w, uint32_t h, float im[][h],
	float act[][h], float inh[][h])
{
	uint32_t x, y;
	
	for (x = 0; x < w; x++)
	{
		for (y = 0; y < h; y++)
		{
			im[x][y] += act[x][y] > inh[x][y] ? p->sa : -(p->sa);
		}
	}
}

/**************************************************************************//**
 * Normalize the image back to the interval [-1; 1].
 * @param w width of the image
 * @param h height of the image
 * @param im image (overwritten)
 *****************************************************************************/
void normalize_image(uint32_t w, uint32_t h, float im[][h])
{
	float max, min, range;
	uint32_t x, y;
	
	max_array(w*h, (float *)im, &max);
	min_array(w*h, (float *)im, &min);
	range = max - min;
	
	for (x=0; x<w; x++)
	{
		for (y=0; y<h; y++)
		{
			im[x][y] = 2*(im[x][y] - min) / range - 1;
		}
	}
}

/**************************************************************************//**
 * Convert the image from floats to bytes.
 * The input image has values E[-1; 1] and the output E[0; 255] to write those
 * values in a grayscale picture.
 * @param w width of the image
 * @param h height of the image
 * @param im_float input image 
 * @param im_bytes output image (overwritten)
 *****************************************************************************/
void convert_image(uint32_t w, uint32_t h, float im_float[][h], 
	uint8_t im_bytes[][h])
{
	uint32_t x, y;
	
	for (x = 0; x < w; x++)
	{
		for (y = 0; y < h; y++)
		{
			im_bytes[x][y] = (uint8_t)((im_float[x][y] + 1.0) * 0xFF / 2.0);
		}
	}
}

/**************************************************************************//**
 * Write image to a gif file.
 * @param gif_image the gifsave89 buffer
 * @param n_bytes number of bytes to write 
 * @return number of written bytes
 *****************************************************************************/
uint32_t write_gif(unsigned char *gif_image, uint32_t n_bytes) 
{
	FILE *f = fopen(PATH_TO_GIF, "wb");
	uint32_t n_written = 0;
	
	if (f != NULL) 
	{
		n_written = fwrite(gif_image, sizeof(unsigned char), n_bytes, f);
		fclose(f); 
	}
	
	return n_written;
}
