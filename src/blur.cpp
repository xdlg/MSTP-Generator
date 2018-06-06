/**************************************************************************//**
 * @file
 * 
 * Fast box blur.
 * 
 * Box blurring of an image with separate horizontal and vertical passes,
 * re-using previous calulations to go faster. The algorithm is based on 
 * http://www.blackpawn.com/texts/blur/default.html, with additional boundary 
 * checks.
 *****************************************************************************/
 
#include "blur.h"

/**************************************************************************//**
 * Horizontal box blur (moving average of the picture, row by row).
 * 
 * @param[in] w Width of the image
 * @param[in] h Height of the image
 * @param[in] r Blurring radius in pixels
 * @param[in] wt Weight
 * @param[in] s Source image
 * @param[out] d Destination image
 *****************************************************************************/
static void blur_x(const size_t w, const size_t h, const uint32_t r,
    const uint32_t wt, const float_t* s, float_t* d);
    
/**************************************************************************//**
 * Vertical box blur (moving average of the picture, column by column).
 * 
 * @param[in] w Width of the image
 * @param[in] h Height of the image
 * @param[in] r Blurring radius in pixels
 * @param[in] wt Weight
 * @param[in] s Source image
 * @param[out] d Destination image
 *****************************************************************************/
static void blur_y(const size_t w, const size_t h, const uint32_t r,
    const uint32_t wt, const float_t* s, float_t* d);

void blur(const size_t w, const size_t h, const uint32_t r, const uint32_t wt,
    const float_t* s, float_t* d)
{
	float_t* temp = new float_t[w*h];
    
	blur_x(w, h, r, wt, s, temp);
	blur_y(w, h, r, wt, temp, d);
    
    delete [] temp;
}

static void blur_x(const size_t w, const size_t h, const uint32_t r,
    const uint32_t wt, const float_t* s, float_t* d)
{	   
	for (size_t y = 0; y < h; y++)
	{
		float_t sum = 0.0;
		
		// In the blurred picture, the first pixel of each row is the average
		// of the source pixels between x = 0 and x = r.
		for (size_t x = 0; x <= r; x++)
		{
			sum += s[x + y*w];
		}
		d[y*w] = (float_t)wt * sum / (float_t)(r + 1);
		
		// The other pixels are computed with a moving average. Pixel values 
		// are subtracted or added from the sum *only* if they are part of the
		// picture. This wasn't really explained on blackpawn's page. And the
		// span of the moving average isn't always equal to the radius, so it
		// has to be computed as well.
		// The casts to int32 are necessary, because subtracting two uint32
		// always gives a positive result when doing the comparison. I think
		// casting from unsigned to signed is compiler-dependent, though, so
		// this might not be so safe.
		for (size_t x = 1; x < w; x++)
		{
			sum += (x + r) < w ? s[x + r + y*w] : 0;
			sum -= (int32_t)(x - r - 1) >= 0 ? s[x - r - 1 + y*w] : 0;
            uint32_t span = ((x + r) < w ? (x + r) : (w - 1))
				 - ((int32_t)(x - r) >= 0 ? (x - r) : 0) 
                 + 1;
            d[x + y*w] = (float_t)wt * sum / (float_t)span;
		}
		
	}
}

static void blur_y(const size_t w, const size_t h, const uint32_t r,
    const uint32_t wt, const float_t* s, float_t* d)
{	
	// See the comments in blur_x, it's exactly the same but working on the
	// columns instead of the rows.
	for (size_t x = 0; x < w; x++)
	{
		float_t sum = 0.0;
		
		for (size_t y = 0; y <= r; y++)
		{
			sum += s[x + y*w];
		}
		d[x] = (float_t)wt * sum / (float_t)(r + 1);
		
		for (size_t y = 1; y < h; y++)
		{
			sum += (y + r) < h ? s[x + (y + r)*w] : 0;
			sum -= (int32_t)(y - r - 1) >= 0 ? s[x + (y - r - 1)*w] : 0;
            uint32_t span = ((y + r) < h ? (y + r) : (h - 1))
				 - ((int32_t)(y - r) >= 0 ? (y - r) : 0)
                 + 1;
            d[x + y*w] = (float_t)wt * sum / (float_t)span;
		}
	}
}
