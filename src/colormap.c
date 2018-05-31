/**************************************************************************//**
 * @file
 * 
 * Colormap generation for gifsave89.
 * 
 * The colormap associates an RGB value for each pixel value. For instance,
 * with a pixel depth of 1 bit, the colormap could be:
 * {0, 0, 0, 255, 255, 255, -1} -> a pixel value of 0 is black, pixel 1 is
 * white, and the array is terminated with -1.
 *****************************************************************************/

#include "colormap.h"
#include <stdio.h>

/**************************************************************************//**
 * Builds a 2-color gradient.
 * 
 * @param[in] depth Number of color levels
 * @param[in] rgb_min First color of the gradient
 * @param[in] rgb_max Last color of the gradient
 * @param[out] colors Colormap array, must have size 3*COLOR_DEPTH + 1
 *****************************************************************************/
static void gradient2(uint32_t depth, uint32_t *rgb_min, uint32_t *rgb_max, 
	int32_t *map);
    
/**************************************************************************//**
 * Builds a 3-color gradient.
 * 
 * @param[in] depth Number of color levels
 * @param[in] rgb_min First color of the gradient
 * @param[in] rgb_mid Middle color of the gradient
 * @param[in] rgb_max Last color of the gradient
 * @param[out] colors Colormap array, must have size 3*COLOR_DEPTH + 1
 *****************************************************************************/
static void gradient3(uint32_t depth, uint32_t *rgb_min, uint32_t *rgb_mid, 
	uint32_t *rgb_max, int32_t *map);
 
void colormap_build(int32_t *colors)
{
	uint32_t black[3] = {0, 0, 0};
	uint32_t white[3] = {255, 255, 255};
	//uint32_t red[3] = {150, 0, 0};
	uint32_t purple[3] = {80, 0, 80};
	//uint32_t green[3] = {0, 100, 0};
	//uint32_t blue[3] = {0, 0, 150};
	
	//gradient2(COLOR_DEPTH, black, white, colors);
	gradient3(COLOR_DEPTH, black, purple, white, colors);
	colors[3*COLOR_DEPTH] = -1; // Always terminate the map with -1
	//colormap_check(colors);
}

static void gradient2(uint32_t depth, uint32_t *rgb_min, uint32_t *rgb_max, 
	int32_t *colors)
{
	float grad[3];
	uint32_t i, j;
	
	for (j = 0; j < 3; j++)
	{
		grad[j] = ((float)rgb_max[j] - (float)rgb_min[j]) / (float)depth;
	}
	
	for (i = 0; i < depth; i++)
	{
		for (j = 0; j < 3; j++)
		{
			colors[3 * i + j] = (int32_t)(i * grad[j] + rgb_min[j]);
		}
	}
}

static void gradient3(uint32_t depth, uint32_t *rgb_min, uint32_t *rgb_mid, 
	uint32_t *rgb_max, int32_t *colors)
{
	gradient2(depth / 2, rgb_min, rgb_mid, colors);
	gradient2(depth / 2, rgb_mid, rgb_max, colors + 3 * depth / 2);
}

void colormap_check(int32_t *colors)
{
	uint32_t i;
	
	for (i = 0; i < COLOR_DEPTH; i++)
	{
		printf("%d - [%d][%d][%d]\n", 
			i, colors[3 * i], colors[3 * i + 1], colors[3 * i + 2]);
	}
}
