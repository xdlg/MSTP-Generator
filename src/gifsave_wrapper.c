/**************************************************************************//**
 * @file
 * 
 * Wrapper for gifsave89.
 *****************************************************************************/

#include "gifsave_wrapper.h"
#include "gifsave/gifsave89.c"
#include "colormap.h"

#define N_REPEATS 		0 	/**< 0: Loop the gif animation */
#define T_FRAME 		4 	/**< *10 ms: Timespan of a gif frame */
#define TRANSPARENCY	-1 	/**< -1: No transparency */
#define DISPOSAL 		1 	/**< Don't really know the influence of that */
#define PATH_TO_GIF		"0.gif"

/** gifsave89 main variable */
static void *gs_data = NULL;
/** gifsave89 buffer */
static unsigned char *gif_image = NULL;

/**************************************************************************//**
 * Writes image to a gif file.
 * 
 * @param[in] gif_image The gifsave89 buffer
 * @param[in] n_bytes Number of bytes to write 
 * 
 * @return Number of written bytes
 *****************************************************************************/
static uint32_t write_gif(unsigned char *gif_image, uint32_t n_bytes);

void gifsave_wrapper_init(uint32_t w, uint32_t h)
{
	int bg_index = 0;					// Background color index
	int32_t colors[3*COLOR_DEPTH + 1];	// Color map (RGB values + term. char)
    
	build_colormap(colors);
	
	// Initialize GIF
	gs_data = newgif((void **)(&gif_image), w, h, (int *)colors, bg_index);
	
	if (gs_data != NULL) 
	{ 
		animategif(gs_data, N_REPEATS, T_FRAME, TRANSPARENCY, DISPOSAL);
	}
}

void gifsave_wrapper_put(uint8_t *frame)
{
    putgif(gs_data, frame);
}

void gifsave_wrapper_close(void)
{
    uint32_t n_bytes = endgif(gs_data);
    
    // Write GIF to file
	if (n_bytes > 0) 
	{
		write_gif(gif_image, n_bytes);
		free(gif_image); 
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
