/**************************************************************************//**
 * @file
 * 
 * Multi-scale Turing patterns based on Jonathan McCabe's work.
 *****************************************************************************/

#include "muscatupa.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include "blur.h"

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
 * Maximum value in an array.
 * 
 * The number of dimensions of the array doesn't matter, but to avoid warning, 
 * don't forget to (float *)-cast the array parameter when calling the function
 * on a multidimensional array.
 * 
 * @param[in] length Total array length (all dimensions)
 * @param[in] array The array
 * @param[out] max Maximum value
 * 
 * @return Index of the maximum
 *****************************************************************************/
static uint32_t max_array(uint32_t length, float *array, float *max);

/**************************************************************************//**
 * Minimum value in an array.
 * 
 * The number of dimensions of the array doesn't matter, but to avoid warning, 
 * don't forget to (float *)-cast the array parameter when calling the function
 * on a multidimensional array.
 * 
 * @param[in] length Total array length (all dimensions)
 * @param[in] array The array
 * @param[out] min Minimum value
 * 
 * @return Index of the minimum
 *****************************************************************************/
static uint32_t min_array(uint32_t length, float *array, float *min);

void muscatupa_init_image(uint32_t w, uint32_t h, float im[][h])
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

void muscatupa_step(struct pattern *p, uint32_t n, uint32_t w, uint32_t h,
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

void muscatupa_convert_image(uint32_t w, uint32_t h, float im_float[][h],
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

static uint32_t max_array(uint32_t length, float *array, float *max)
{
	uint32_t i;
    uint32_t i_max = 0;
    
	*max = FLT_MIN;
	
	for (i = 0; i < length; i++)
	{
		if (*(array + i) > *max)
		{
			*max = *(array + i);
			i_max = i;
		}
	}
	
	return i_max;
}

static uint32_t min_array(uint32_t length, float *array, float *min)
{
	uint32_t i;
    uint32_t i_min = 0;
    
	*min = FLT_MAX;
	
	for (i = 0; i < length; i++)
	{
		if (*(array + i) < *min)
		{
			*min = *(array + i);
			i_min = i;
		}
	}
	
	return i_min;
}
