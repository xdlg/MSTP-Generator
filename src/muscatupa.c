/**************************************************************************//**
 * @file
 * 
 * Multi-scale Turing patterns based on Jonathan McCabe's work.
 *****************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>
#include <math.h>
#include "gifsave/gifsave89.c"
#include "utils.h"
#include "colormap.h"
#include "blur.h"

#define N_REPEATS 		0 	/**< 0: Loop the gif animation */
#define T_FRAME 		4 	/**< *10 ms: Timespan of a gif frame */
#define TRANSPARENCY	-1 	/**< -1: No transparency */
#define DISPOSAL 		1 	/**< Don't really know the influence of that */
#define PATH_TO_GIF		"0.gif"

#define N_SCALES 		5 	/**< Number of Turing patterns/scales */
#define W				300 /**< Image width */
#define H				300	/**< Image height */
#define N_STEPS 		100 /**< Number of timesteps to generate */

#define ANIMATE 		1 	/**< 0 to save only the last generated picture */

/**
 * One Turing pattern/one scale.
 */
struct pattern
{
	uint32_t act_r; /**< Activator radius */
	uint32_t inh_r; /**< Inhibitor radius */
	float sa;		/**< Small amount */
	uint32_t wt;	/**< Weight */
};

/** 
* Parameters of the Turing patterns.
*/
const uint32_t act_r_all[N_SCALES] = {100, 20, 10, 5, 1};	 	/**< Activator radii */
const uint32_t inh_r_all[N_SCALES] = {200, 40, 20, 10, 2};	 	/**< Inhibitor radii */
const float sa_all[N_SCALES] = {0.05, 0.04, 0.03, 0.02, 0.01}; 	/**< Small amounts */
const uint32_t wt_all[N_SCALES] = {1, 1, 1, 1, 1}; 				/**< Weights */

/**************************************************************************//**
 * Initializes the image with random values.
 * 
 * The w*h array is filled with values E[-1; 1].
 * 
 * @param[in] w Width of the image
 * @param[in] h Height of the image
 * @param[out] im Image
 *****************************************************************************/
static void init_image(uint32_t w, uint32_t h, float s[][h]);

/**************************************************************************//**
 * One step of the main algorithm.
 * 
 * @param[in] p Array of Turing patterns
 * @param[in] n Number of Turing patterns
 * @param[in] w Width of the image
 * @param[in] h Height of the image
 * @param[inout] im Image
 *****************************************************************************/
static void step(struct pattern *p, uint32_t n, uint32_t w, uint32_t h, 
    float im[][h]);

/**************************************************************************//**
 * Computes the variation arrays for all scales.
 * 
 * The variation is simply |activator[x][y] - inhibitor[x][y]| for each pixel.
 * Normally there should be an averaging over a radius, but it's simpler to 
 * code that for a single-pixel radius and in theory it gives better pictures.
 * 
 * @param[in] n Number of Turing patterns
 * @param[in] w Width of the image
 * @param[in] h Height of the image
 * @param[in] act Activator arrays
 * @param[in] inh Inhibitor arrays
 * @param[out] var Variation arrays
 *****************************************************************************/
static void compute_var(uint32_t n, uint32_t w, uint32_t h, float act[][w][h], 
	float inh[][w][h], float var[][w][h]);

/**************************************************************************//**
 * Finds which scale has the smallest variation.
 * 
 * @param[in] n Number of Turing patterns
 * @param[in] w Width of the image
 * @param[in] h Height of the image
 * @param[in] var Variation arrays 
 * @param[out] best_scale Scale with the smallest variation
 *****************************************************************************/
static void find_best_scale(uint32_t n, uint32_t w, uint32_t h,
    float var[][w][h], uint32_t best_scale[][h]);

/**************************************************************************//**
 * Updates the image.
 * 
 * Each pixel of the image is increased/decreased by a small amount depending
 * on the activator and inhibitor value at those coordinates. This function is
 * normally called with the parameters of the smallest-variation scale.
 * 
 * @param[in] p Array of Turing patterns
 * @param[in] n Number of Turing patterns
 * @param[in] w Width of the image
 * @param[in] h Height of the image
 * @param[in] act Activator arrays
 * @param[in] inh Inhibitor arrays
 * @param[in] best_scale Scale with the smallest variation
 * @param[inout] im Image
 *****************************************************************************/
static void update(struct pattern *p, uint32_t n, uint32_t w, uint32_t h, 
	float act[][w][h], float inh[][w][h], uint32_t best_scale[][h],
    float im[][h]);

/**************************************************************************//**
 * Normalizes the image back to the interval [-1; 1].
 * 
 * @param[in] w Width of the image
 * @param[in] h Height of the image
 * @param[inout] im Image
 *****************************************************************************/
static void normalize(uint32_t w, uint32_t h, float s[][h]);

/**************************************************************************//**
 * Convert the image from floats to bytes.
 * 
 * The input image has values E[-1; 1] and the output E[0; 255] to write those
 * values in a grayscale picture.
 * 
 * @param[in] w Width of the image
 * @param[in] h Height of the image
 * @param[in] im_float Input image 
 * @param[out] im_bytes Output image
 *****************************************************************************/
static void convert_image(uint32_t w, uint32_t h, float im_float[][h], 
	uint8_t im_bytes[][h]);

/**************************************************************************//**
 * Writes image to a gif file.
 * 
 * @param[in] gif_image The gifsave89 buffer
 * @param[in] n_bytes Number of bytes to write 
 * 
 * @return Number of written bytes
 *****************************************************************************/
static uint32_t write_gif(unsigned char *gif_image, uint32_t n_bytes);

/**************************************************************************//**
 * Turing pattern generator.
 * 
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
	build_colormap(colors);

	// Initialize the patterns
	for (i = 0; i < N_SCALES; i++)
	{
		p[i].act_r = act_r_all[i];
		p[i].inh_r = inh_r_all[i];
		p[i].sa = sa_all[i];
		p[i].wt = wt_all[i];
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
		for (i = 0; i < N_STEPS; i++)
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

static void init_image(uint32_t w, uint32_t h, float im[][h])
{
	uint32_t x, y;
	
	srand(time(NULL));
	for (x = 0; x < w; x++)
	{
		for (y = 0; y < h; y++)
		{
			im[x][y] = (float)rand() / (float)(RAND_MAX) * 2.0 - 1.0;
		}
	}
}

static void step(struct pattern *p, uint32_t n, uint32_t w, uint32_t h,
    float im[][h])
{
	float act[n][w][h];	// Activator arrays
	float inh[n][w][w];	// Inhibitor arrays
	float var[n][w][h];	// Variation arrays
	uint32_t best_scale[w][h];
	uint32_t i;
	
	// Generate the activator and inhibitor arrays
	for (i = 0; i < n; i++)
	{
		blur(w, h, (p+i)->act_r, (p+i)->wt, im, act[i]);
		blur(w, h, (p+i)->inh_r, (p+i)->wt, im, inh[i]);
	}
	
	compute_var(n, w, h, act, inh, var);
	find_best_scale(n, w, h, var, best_scale);
	update(p, n, w, h, act, inh, best_scale, im);
	normalize(w, h, im);
}

static void compute_var(uint32_t n, uint32_t w, uint32_t h, float act[][w][h], 
	float inh[][w][h], float var[][w][h])
{
	uint32_t x, y;
	uint32_t i;
	
	for (i = 0; i < n; i++)
	{		
		for (x = 0; x < w; x++)
		{
			for (y = 0; y < h; y++)
			{
				var[i][x][y] = fabs(act[i][x][y] - inh[i][x][y]);
			}
		}
	}
}

static void find_best_scale(uint32_t n, uint32_t w, uint32_t h,
    float var[][w][h], uint32_t best_scale[][h])
{
	float lst_var[w][h];
	uint32_t i, x, y;
	
	for (x = 0; x < w; x++)
	{
		for (y = 0; y < h; y++)
		{
			// Start by assuming that scale 0 is the best
			lst_var[x][y] = var[0][x][y];
			best_scale[x][y] = 0;
			for (i = 1; i < n; i++)
			{
				if (var[i][x][y] < lst_var[x][y])
				{
					lst_var[x][y] = var[i][x][y];
					best_scale[x][y] = i;
				}
			}
		}
	}
}

static void update(struct pattern *p, uint32_t n, uint32_t w, uint32_t h, 
	float act[][w][h], float inh[][w][h], uint32_t best_scale[][h],
    float im[][h])
{
	uint32_t i, x, y;
	
	for (x = 0; x < w; x++)
	{
		for (y = 0; y < h; y++)
		{
			i = best_scale[x][y];
			im[x][y] += act[i][x][y] > inh[i][x][y] ? (p+i)->sa : -((p+i)->sa);
		}
	}
}

static void normalize(uint32_t w, uint32_t h, float im[][h])
{
	float max, min, range;
	uint32_t x, y;
	
	max_array(w*h, (float *)im, &max);
	min_array(w*h, (float *)im, &min);
	range = max - min;
	
	for (x = 0; x < w; x++)
	{
		for (y = 0; y < h; y++)
		{
			im[x][y] = (2 * (im[x][y] - min) / range) - 1;
		}
	}
}

static void convert_image(uint32_t w, uint32_t h, float im_float[][h], 
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

static uint32_t write_gif(unsigned char *gif_image, uint32_t n_bytes) 
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
