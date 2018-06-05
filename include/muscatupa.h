/**************************************************************************//**
 * @file
 * 
 * Multi-scale Turing patterns based on Jonathan McCabe's work.
 *****************************************************************************/

#ifndef MUSCATUPA_H
#define MUSCATUPA_H

#include <stdint.h>
#include <math.h>

/**
 * One Turing pattern/one scale.
 */
struct pattern
{
	uint32_t act_r; /**< Activator radius */
	uint32_t inh_r; /**< Inhibitor radius */
	float_t sa;		/**< Small amount */
	uint32_t wt;	/**< Weight */
};

/**************************************************************************//**
 * Initializes the image with random values.
 * 
 * @param[in] w Image width
 * @param[in] h Image height
 * @param[out] im Image (values in the interval [0; 1])
 *****************************************************************************/
void muscatupa_init_image(uint32_t w, uint32_t h, float_t *im);

/**************************************************************************//**
 * One step of the main algorithm.
 * 
 * @param[in] p Array of Turing patterns
 * @param[in] n Number of Turing patterns
 * @param[in] w Image width
 * @param[in] h Image height
 * @param[inout] im Image (values in the interval [0; 1])
 *****************************************************************************/
void muscatupa_step(struct pattern *p, uint32_t n, uint32_t w, uint32_t h, 
    float_t *im);

#endif
