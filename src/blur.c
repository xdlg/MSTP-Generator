/**************************************************************************//**
 * @file
 * Fast box blur.
 * Box blurring of an image with separate horizontal and vertical passes,
 * re-using previous calulations to go faster. The algorithm is based on 
 * http://www.blackpawn.com/texts/blur/default.html, with additional boundary 
 * checks.
 *****************************************************************************/

/******************************************************************************
 * #include
 *****************************************************************************/
#include "blur.h"
#include <stdio.h>

/******************************************************************************
 * Private variables
 *****************************************************************************/
 
/******************************************************************************
 * Private functions
 *****************************************************************************/
static void blur_x(uint32_t w, uint32_t h, float s[][h], float d[][h], 
	uint32_t r, uint32_t wt);
static void blur_y(uint32_t w, uint32_t h, float s[][h], float d[][h], 
	uint32_t r, uint32_t wt);

/**************************************************************************//**
 * Box blur main function.
 * Calls the horizontal and vertical blurring functions.
 * @param w width of the image
 * @param h height of the image
 * @param s source image
 * @param d destination image (overwritten)
 * @param r blurring radius in pixels
 * @param wt weight
 *****************************************************************************/
void blur(uint32_t w, uint32_t h, float s[][h], float d[][h], uint32_t r,
	uint32_t wt)
{
	float temp[w][h];
	blur_x(w, h, s, temp, r, wt);
	blur_y(w, h, temp, d, r, wt);
}

/**************************************************************************//**
 * Horizontal box blur.
 * This is just a moving average of the picture, row by row.
 * @param w width of the image
 * @param h height of the image
 * @param s source image
 * @param d destination image (overwritten)
 * @param r blurring radius in pixels
 * @param wt weight
 *****************************************************************************/
void blur_x(uint32_t w, uint32_t h, float s[][h], float d[][h], uint32_t r,
	uint32_t wt)
{	
	float sum;
	uint32_t x, y, span;
	
	for (y=0; y<h; y++)
	{
		sum = 0;
		
		// In the blurred picture, the first pixel of each row is the average
		// of the source pixels between x=0 and x=r-1.
		for (x=0; x<r; x++)
		{
			sum += s[x][y];
		}
		d[0][y] = (float)wt * sum / (r + 1);
		
		// The other pixels are computed with a moving average. Pixel values 
		// are subtracted or added from the sum *only* if they are part of the
		// picture. This wasn't really explained on blackpawn's page. And the
		// span of the moving average isn't always equal to the radius, so it
		// has to be computed as well.
		// The casts to int32 are necessary, because subtracting two uint32
		// always gives a positive result when doing the comparison. I think
		// casting from unsigned to signed is compiler-dependent, though, so
		// this might not be so safe.
		for (x=1; x<w; x++)
		{
			sum -= (int32_t)(x-r-1) >= 0 ? s[x-r-1][y] : 0;
			sum += (x+r) < w ? s[x+r][y] : 0;
            span = ((x+r) < w ? (x+r) : (w-1))
				 - ((int32_t)(x-r) >= 0 ? (x-r) : 0) + 1;
            d[x][y] = (float)wt * sum / span;
		}
		
	}
}

/**************************************************************************//**
 * Vertical box blur.
 * This is just a moving average of the picture, column by column.
 * @param w width of the image
 * @param h height of the image
 * @param s source image
 * @param d destination image (overwritten)
 * @param r blurring radius in pixels
 * @param wt weight
 *****************************************************************************/
void blur_y(uint32_t w, uint32_t h, float s[][h], float d[][h], uint32_t r,
	uint32_t wt)
{	
	// See the comments in blur_x, it's exactly the same but working on the
	// columns instead of the rows.
	float sum;
	uint32_t x, y, span;
	
	for (x=0; x<w; x++)
	{
		sum = 0;
		
		for (y=0; y<r; y++)
		{
			sum += s[x][y];
		}
		d[x][0] = (float)wt * sum / (r + 1);
		
		for (y=1; y<h; y++)
		{
			sum -= (int32_t)(y-r-1) >= 0 ? s[x][y-r-1] : 0;
			sum += (y+r) < h ? s[x][y+r] : 0;
            span = ((y+r) < h ? (y+r) : (h-1))
				 - ((int32_t)(y-r) >= 0 ? (y-r) : 0) + 1;
            d[x][y] = (float)wt * sum / span;
		}
	}
}
