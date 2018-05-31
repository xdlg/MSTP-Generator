/**************************************************************************//**
 * @file
 * 
 * Turing pattern generation.
 *****************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "muscatupa.h"
#include "gifsave_wrapper.h"

#define N_SCALES 		5 	/**< Number of Turing patterns/scales */
#define W				300 /**< Image width */
#define H				300	/**< Image height */
#define N_STEPS 		100 /**< Number of timesteps to generate */

/** 
* Parameters of the Turing patterns.
*/
/** Activator radii */
const uint32_t act_r_all[N_SCALES] = {100, 20, 10, 5, 1};
/** Inhibitor radii */
const uint32_t inh_r_all[N_SCALES] = {200, 40, 20, 10, 2};
/** Small amounts */
const float sa_all[N_SCALES] = {0.05, 0.04, 0.03, 0.02, 0.01};
/** Weights */
const uint32_t wt_all[N_SCALES] = {1, 1, 1, 1, 1};

int main(void)
{	
	struct pattern p[N_SCALES];	// Array of Turing patterns
	float im_float[W][H];		// Image with values E[-1; 1]
	uint8_t im_bytes[W][H];		// Image with values E[0; 255]
	uint32_t i;
	
	init_image(W, H, im_float);

	// Initialize the patterns
	for (i = 0; i < N_SCALES; i++)
	{
		p[i].act_r = act_r_all[i];
		p[i].inh_r = inh_r_all[i];
		p[i].sa = sa_all[i];
		p[i].wt = wt_all[i];
	}
	
	gifsave_wrapper_init(W, H);
		
    // Build frames
    for (i = 0; i < N_STEPS; i++)
    {
        printf("%d/%d\n", i + 1, N_STEPS);
        step(p, N_SCALES, W, H, im_float);
        convert_image(W, H, im_float, im_bytes);
        gifsave_wrapper_put((uint8_t *)im_bytes);
    }

    gifsave_wrapper_close();
 
	return 0;
}
