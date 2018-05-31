/**************************************************************************//**
 * @file
 * 
 * Multi-scale Turing patterns based on Jonathan McCabe's work.
 *****************************************************************************/

#ifndef MUSCATUPA_H
#define MUSCATUPA_H

#include <stdint.h>

/**
 * One Turing pattern/one scale.
 */
struct pattern
{
	uint32_t act_r; /**< Activator radius */
	uint32_t inh_r; /**< Inhibitor radius */
	float sa;		/**< Small amount */
	uint32_t wt;	/**< Weight */
};

/**************************************************************************//**
 * Initializes the image with random values.
 * 
 * The w*h array is filled with values E[-1; 1].
 * 
 * @param[in] w Width of the image
 * @param[in] h Height of the image
 * @param[out] im Image
 *****************************************************************************/
void init_image(uint32_t w, uint32_t h, float s[][h]);

/**************************************************************************//**
 * One step of the main algorithm.
 * 
 * @param[in] p Array of Turing patterns
 * @param[in] n Number of Turing patterns
 * @param[in] w Width of the image
 * @param[in] h Height of the image
 * @param[inout] im Image
 *****************************************************************************/
void step(struct pattern *p, uint32_t n, uint32_t w, uint32_t h, 
    float im[][h]);

/**************************************************************************//**
 * Convert the image from floats to bytes.
 * 
 * The input image has values E[-1; 1] and the output E[0; 255] to write those
 * values in a grayscale picture.
 * 
 * @param[in] w Width of the image
 * @param[in] h Height of the image
 * @param[in] im_float Input image 
 * @param[out] im_bytes Output image
 *****************************************************************************/
void convert_image(uint32_t w, uint32_t h, float im_float[][h], 
	uint8_t im_bytes[][h]);

#endif
