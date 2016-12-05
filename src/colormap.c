/**************************************************************************//**
 * @file
 * Colormap generation for gifsave89.
 * The colormap associates an RGB value for each pixel value. For instance,
 * with a pixel depth of 1 bit, the colormap could be:
 * {0, 0, 0, 255, 255, 255, -1} -> a pixel value of 0 is black, pixel 1 is
 * white, and the array is terminated with -1.
 *****************************************************************************/

/******************************************************************************
 * #include
 *****************************************************************************/
#include "colormap.h"

/******************************************************************************
 * Private variables
 *****************************************************************************/
 
/******************************************************************************
 * Private functions
 *****************************************************************************/
static void gradient2(uint32_t depth, uint32_t *rgb_min, uint32_t *rgb_max, 
	int32_t *map);
static void gradient3(uint32_t depth, uint32_t *rgb_min, uint32_t *rgb_mid, 
	uint32_t *rgb_max, int32_t *map);
 
/**************************************************************************//**
 * Build the RGB colormap for gifsave89.
 * Modify the called function to select another color scheme, transparently for 
 * the main program.
 * @param colors colormap array (overwritten), must have size 3*COLOR_DEPTH + 1
 *****************************************************************************/
void build_colormap(int32_t *colors)
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
	//check_map(colors);
}

/**************************************************************************//**
 * Build a 2-color gradient.
 * @param depth number of color levels
 * @param rgb_min first color of the gradient
 * @param rgb_max last color of the gradient
 * @param colors colormap array (overwritten), must have size 3*COLOR_DEPTH + 1
 *****************************************************************************/
void gradient2(uint32_t depth, uint32_t *rgb_min, uint32_t *rgb_max, 
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

/**************************************************************************//**
 * Build a 3-color gradient.
 * @param depth number of color levels
 * @param rgb_min first color of the gradient
 * @param rgb_mid middle color of the gradient
 * @param rgb_max last color of the gradient
 * @param colors colormap array (overwritten), must have size 3*COLOR_DEPTH + 1
 *****************************************************************************/
void gradient3(uint32_t depth, uint32_t *rgb_min, uint32_t *rgb_mid, 
	uint32_t *rgb_max, int32_t *colors)
{
	gradient2(depth / 2, rgb_min, rgb_mid, colors);
	gradient2(depth / 2, rgb_mid, rgb_max, colors + 3 * depth / 2);
}

/**************************************************************************//**
 * Write the color map to console (debug).
 * @param depth number of color levels
 * @param rgb_min first color of the gradient
 * @param rgb_mid middle color of the gradient
 * @param rgb_max last color of the gradient
 * @param colors colormap array (overwritten), must have size 3*COLOR_DEPTH + 1
 *****************************************************************************/
void check_map(int32_t *colors)
{
	uint32_t i;
	
	for (i = 0; i < COLOR_DEPTH; i++)
	{
		printf("%d - [%d][%d][%d]\n", 
			i, colors[3 * i], colors[3 * i + 1], colors[3 * i + 2]);
	}
}
