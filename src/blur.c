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

/******************************************************************************
 * Private variables
 *****************************************************************************/
 
/******************************************************************************
 * Private functions
 *****************************************************************************/
static void blur_x(uint16_t w, uint16_t h, float s[][h], float d[][h], 
	uint16_t r);
static void blur_y(uint16_t w, uint16_t h, float s[][h], float d[][h], 
	uint16_t r);

/**************************************************************************//**
 * Box blur main function.
 * Calls the horizontal and vertical blurring functions.
 * @param w width of the image
 * @param h height of the image
 * @param s source image
 * @param d destination image (overwritten)
 * @param r blurring radius in pixels
 *****************************************************************************/
void blur(uint16_t w, uint16_t h, float s[][h], float d[][h], uint16_t r)
{
	float temp[w][h];
	blur_x(w, h, s, temp, r);
	blur_y(w, h, temp, d, r);
}

/**************************************************************************//**
 * Horizontal box blur.
 * This is just a moving average of the picture, row by row.
 * @param w width of the image
 * @param h height of the image
 * @param s source image
 * @param d destination image (overwritten)
 * @param r blurring radius in pixels
 *****************************************************************************/
void blur_x(uint16_t w, uint16_t h, float s[][h], float d[][h], uint16_t r)
{	
	float sum;
	uint16_t x, y, span;
	
	for (y=0; y<h; y++)
	{
		sum = 0;
		
		// In the blurred picture, the first pixel of each row is the average
		// of the source pixels between x=0 and x=r-1.
		for (x=0; x<r; x++)
		{
			sum += s[x][y];
		}
		d[0][y] = sum / (r + 1);
		
		// The other pixels are computed with a moving average. Pixel values 
		// are subtracted or added from the sum *only* if they are part of the
		// picture. This wasn't really explained on blackpawn's page. And the
		// span of the moving average isn't always equal to the radius, so it
		// has to be computed as well.
		for (x=1; x<w; x++)
		{
			sum -= (x - r - 1) >= 0 ? s[x-r-1][y] : 0;
			sum += (x + r) < w ? s[x+r][y] : 0;
            span = ((x + r) < w ? (x + r) : (w - 1))
				 - ((x - r) >= 0 ? (x - r) : 0) + 1;
            d[x][y] = sum / span;
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
 *****************************************************************************/
void blur_y(uint16_t w, uint16_t h, float s[][h], float d[][h], uint16_t r)
{	
	float sum;
	uint16_t x, y, span;
	
	for (x=0; x<w; x++)
	{
		sum = 0;
		
		// In the blurred picture, the first pixel of each column is the 
		// average of the source pixels between y=0 and y=r-1.
		for (y=0; y<r; y++)
		{
			sum += s[x][y];
		}
		d[x][0] = sum / (r + 1);
		
		// The other pixels are computed with a moving average. Pixel values 
		// are subtracted or added from the sum *only* if they are part of the
		// picture. This wasn't really explained on blackpawn's page. And the
		// span of the moving average isn't always equal to the radius, so it
		// has to be computed as well.
		for (y = 1; y < h; y++)
		{
			sum -= (y - r - 1) >= 0 ? s[x][y-r-1] : 0;
			sum += (y + r) < h ? s[x][y+r] : 0;
            span = ((y + r) < h ? (y + r) : (h - 1))
				 - ((y - r) >= 0 ? (y - r) : 0) + 1;
            d[x][y] = sum / span;
		}
	}
}
