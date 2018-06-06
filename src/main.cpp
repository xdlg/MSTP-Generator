/**************************************************************************//**
 * @file
 * 
 * Turing pattern generation.
 *****************************************************************************/

#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <SDL.h>
#include "blind_quarter.h"
#include "colormap.h"

#define N_SCALES    5   /**< Number of Turing patterns/scales */
#define N_ARGUMENTS 3   /**< Required number of command line arguments */
#define WIDTH_MIN   100 /**< Minimum image width */
#define HEIGHT_MIN  100 /**< Minimum image height */

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

/**************************************************************************//**
 * Parses the command line arguments.
 * 
 * @param[in] argc Argument count
 * @param[in] argv Argument vector
 * @param[out] width Image width (argument 1)
 * @param[out] height Image height (argument 2)
 * 
 * @return false if no error
 *****************************************************************************/
static bool parse_args(int argc, char** argv, size_t* width, size_t* height);

int main(int argc, char** argv)
{   
    size_t width;
    size_t height;
    bool parsing_error = parse_args(argc, argv, &width, &height);
    
    if (parsing_error)
    {
        std::cerr << "Usage: " << argv[0] << " IMAGE_WIDTH IMAGE_HEIGHT"
            << std::endl;
        std::cerr << "IMAGE_WIDTH >= " << std::to_string(WIDTH_MIN)
            << std::endl;
        std::cerr << "IMAGE_HEIGHT >= " << std::to_string(HEIGHT_MIN)
            << std::endl;
        return 1;
    }
    
    // Initialize the patterns
    struct pattern p[N_SCALES];
	for (size_t i = 0; i < N_SCALES; i++)
	{
		p[i].act_r = act_r_all[i];
		p[i].inh_r = inh_r_all[i];
		p[i].sa = sa_all[i];
		p[i].wt = wt_all[i];
    }
    
    // Initialize the image generation
    float_t *image = new float_t[width*height];
    uint32_t *image_colormapped = new uint32_t[width*height];
    colormap_init(COLORMAP_LAVA);
    muscatupa_init_image(width, height, image);
    
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("Muscatupa",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture * texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);
    SDL_Event event;
    bool quit = false;
 
    while (!quit)
    {    
        // Check if the user wants to quit
        while (SDL_PollEvent(&event))
        {
            quit = event.type == SDL_QUIT;
        }

        // Update image
        muscatupa_step(p, N_SCALES, width, height, image);
        colormap_ARGB8888(width, height, image, image_colormapped);
    
        // Show updated image
        SDL_UpdateTexture(texture, NULL, image_colormapped,
            width*sizeof(uint32_t));
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    
    // Clean up SDL
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    delete [] image;
    delete [] image_colormapped;
 
    return 0;
}

static bool parse_args(int argc, char** argv, size_t* width, size_t* height)
{
    bool error = false;
    
    // Check the number of arguments
    if (argc != N_ARGUMENTS)
    {
        error = true;
    }
    else
    {
        char* end_width;
        char* end_height;
        *width = strtol(argv[1], &end_width, 0);
        *height = strtol(argv[2], &end_height, 0);
    
        // Check the contents of the arguments
        if ((end_width == NULL) || (end_height == NULL)
            || (*width < WIDTH_MIN) || (*height < HEIGHT_MIN)
            || (*width == UINT32_MAX) || (*height == UINT32_MAX))
        {
            error = true;
        }
    }
    
    return error;
}
