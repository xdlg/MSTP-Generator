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
#include <SDL_keycode.h>
#include "blind_quarter.h"
#include "colormap.h"

#define N_SCALES    5   /**< Number of Turing patterns/scales */
#define N_ARGUMENTS 4   /**< Required number of command line arguments */
#define WIDTH_MIN   100 /**< Minimum image width */
#define HEIGHT_MIN  100 /**< Minimum image height */

/** 
* Parameters of the Turing patterns.
*/
/** Activator radii */
static const uint32_t act_r_all[N_SCALES] = {50, 25, 10, 5, 1};
/** Inhibitor radii */
static const uint32_t inh_r_all[N_SCALES] = {100, 50, 20, 10, 2};
/** Small amounts */
static const float_t sa_all[N_SCALES] = {0.05, 0.04, 0.03, 0.02, 0.01};
/** Weights */
static const uint32_t wt_all[N_SCALES] = {1, 1, 1, 1, 1};

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
static bool parse_args(int argc, char** argv, size_t* width, size_t* height,
    std::string* colors);

int main(int argc, char** argv)
{       
    size_t width;
    size_t height;
    std::string colors;
    bool parsing_error = parse_args(argc, argv, &width, &height, &colors);
    
    if (parsing_error)
    {
        std::cerr << "Usage: " << argv[0]
            << " image_width image_height color_map" << std::endl;
        std::cerr << "image_width >= " << std::to_string(WIDTH_MIN)
            << std::endl;
        std::cerr << "image_height >= " << std::to_string(HEIGHT_MIN)
            << std::endl;
        std::cerr << "color_map = [bw|rainbow|holiday|neon|lava|ice|dawn|toxic]"
            << std::endl;
        return 1;
    }
    
    // Initialize the patterns
    uint32_t n_active_scales = 1;
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
    colormap_init(colors);
    blind_quarter_init_image(width, height, image);
    
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
        // Handle events
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYUP:
                    SDL_Keycode key = event.key.keysym.sym;
                    if ((key == SDLK_KP_PLUS) && (n_active_scales < N_SCALES))
                    {
                        n_active_scales++;
                    }
                    if ((key == SDLK_KP_MINUS) && (n_active_scales > 1))
                    {
                        n_active_scales--;
                    }
                    break;
            }
        }

        // Update image
        blind_quarter_step(p, n_active_scales, width, height, image);
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

static bool parse_args(int argc, char** argv, size_t* width, size_t* height,
    std::string* colors)
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
        *colors = argv[3];
    
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
