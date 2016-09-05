/**************************************************************************//**
 * @file
 * Point symmetry around the origin of an image.
 *****************************************************************************/

#ifndef SYMMETRY_H
#define SYMMETRY_H

/******************************************************************************
 * #include
 *****************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/******************************************************************************
 * #define
 *****************************************************************************/
#define LEQ 0 /**< Lower or equal */
#define GEQ 1 /**< Greater or equal */

/** a <= b or a >= b, depending on the leq parameter */
#define compare(a, b, leq) ((leq == LEQ) ? (a <= b) : (a >= b))

/******************************************************************************
 * Types and structures
 *****************************************************************************/
/** 
 * Symmetry object: points to the first element of a list of symmetrical
 * coordinates
 */
struct sym
{
	struct xy *head_xy; 	/**< Head (x,y) couple */
	struct sym *next;		/**< Next list element */
};

/**
 * (x, y) couple object
 */
struct xy
{
    uint32_t x;			/**< x coordinate */
    uint32_t y;			/**< y coordinate */
    struct xy *next;	/**< Next list element */
};

/******************************************************************************
 * Global and public variables
 *****************************************************************************/

/******************************************************************************
 * Public functions
 *****************************************************************************/ 
void build_sym_list(struct sym **head_sym, struct sym **tail_sym, uint32_t dim, 
	uint32_t s);
void free_sym(struct sym *head);
void check_sym_list(struct sym *head_sym, uint32_t dim);
void symmetrize(struct sym *head_sym, uint32_t dim, float s[][dim]);

#endif
