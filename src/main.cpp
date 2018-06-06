/**************************************************************************//**
 * @file
 * 
 * Turing pattern generation.
 *****************************************************************************/

#include <iostream>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <SDL.h>
#include <chrono>
#include "muscatupa.h"
#include "colormap.h"

#define WIDTH       400 /**< Image width */
#define HEIGHT      225 /**< Image height */
#define N_SCALES    5   /**< Number of Turing patterns/scales */

/** 
* Parameters of the Turing patterns.
*/
/** Activator radii */
const uint32_t act_r_all[N_SCALES] = {50, 25, 10, 5, 1};
/** Inhibitor radii */
const uint32_t inh_r_all[N_SCALES] = {100, 50, 20, 10, 2};
/** Small amounts */
const float_t sa_all[N_SCALES] = {0.05, 0.04, 0.03, 0.02, 0.01};
/** Weights */
const uint32_t wt_all[N_SCALES] = {1, 1, 1, 1, 1};

int main(int argc, char ** argv)
{
    struct pattern p[N_SCALES];
    float_t image[WIDTH*HEIGHT];
    uint32_t image_colormapped[WIDTH*HEIGHT];
    uint32_t i;
    
    SDL_Event event;
    bool quit = false;
    
    // Initialize the patterns
	for (i = 0; i < N_SCALES; i++)
	{
		p[i].act_r = act_r_all[i];
		p[i].inh_r = inh_r_all[i];
		p[i].sa = sa_all[i];
		p[i].wt = wt_all[i];
    }
    
    colormap_init(COLORMAP_LAVA);
    muscatupa_init_image(WIDTH, HEIGHT, image);
    
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("Muscatupa",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture * texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, WIDTH, HEIGHT);
 
    while (!quit)
    {    
        // Check if the user wants to quit
        while (SDL_PollEvent(&event))
        {
            quit = event.type == SDL_QUIT;
        }

        // Update image
        muscatupa_step(p, N_SCALES, WIDTH, HEIGHT, image);
        colormap_ARGB8888(WIDTH, HEIGHT, image, image_colormapped);
    
        // Show updated image
        SDL_UpdateTexture(texture, NULL, image_colormapped,
            WIDTH*sizeof(uint32_t));
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    
    // Clean up SDL
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
 
    return 0;
}
