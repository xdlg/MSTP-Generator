/**************************************************************************//**
 * @file
 * 
 * Rotational symmetry functions for the activator/inhibitor arrays.
 *****************************************************************************/
 
#include "symmetry.h"

/** Minimum symmetry order */
#define SYM_ORDER_MIN   2
/** Maximum symmetry order */
#define SYM_ORDER_MAX   4
/** Pi */
#define PI              3.14159265

/**
 * A symmetry lookup table is a list of symmetrical pixel coordinates.
 * Each row is a group of symmetrical pixel coordinates. The coordinates are
 * linear (i.e. i = x + y*w, w being the width of the image).
 * In each row there are N elements (N = symmetry order).
 * 
 * For example, let's say we have a 2*2 image and a symmetry order 2.
 * The linear pixels coordinates are shown below (in a 2D array for clarity).
 *  -------
 * | 0 | 1 |
 *  -------
 * | 2 | 3 |
 *  -------
 * 
 * By rotational symmetry around the center of the image, pixel 0 is
 * symmetrical with pixel 3 and 1 with 2. After calculation, the resulting
 * lookup table should look like this:
 *  -------
 * | 0 | 3 |
 *  -------
 * | 1 | 2 |
 *  -------
 */
typedef std::vector<std::vector<size_t>> symmetry_lookup;

/** Symmetry lookup tables for all orders up to SYMMETRY_ORDER_MAX */
static std::vector<symmetry_lookup> symmetry_lookup_all;

/**************************************************************************//**
 * Get the lookup table for symmetry order 2.
 * 
 * This is a rotational symmetry, because for order 2 it's very simple.
 * 
 * @param[in] w Image width
 * @param[in] h Image height
 * 
 * @return Lookup table
 *****************************************************************************/
static symmetry_lookup symmetry_init_order_2(const size_t w, const size_t h);

/**************************************************************************//**
 * Get the lookup table for symmetry order 3.
 * 
 * Not implemented at the moment.
 * 
 * @param[in] w Image width
 * @param[in] h Image height
 * 
 * @return Lookup table
 *****************************************************************************/
static symmetry_lookup symmetry_init_order_3(const size_t w, const size_t h);

/**************************************************************************//**
 * Get the lookup table for symmetry order 4.
 * 
 * For the moment it's simply a horizontal + vertical mirroring, because for a
 * rotational symmetry we would have to deal with unequal width and height,
 * which isn't a priority right now.
 * 
 * @param[in] w Image width
 * @param[in] h Image height
 * 
 * @return Lookup table
 *****************************************************************************/
static symmetry_lookup symmetry_init_order_4(const size_t w, const size_t h);

void symmetry_init(const size_t w, const size_t h)
{    
    for (size_t sym_order = SYM_ORDER_MIN; sym_order <= SYM_ORDER_MAX; sym_order++)
    {
        symmetry_lookup sym;
        
        if (sym_order == 2)
        {
            sym = symmetry_init_order_2(w, h);
        }
        else if (sym_order == 3)
        {
            sym = symmetry_init_order_3(w, h);
        }
        else if (sym_order == 4)
        {
            sym = symmetry_init_order_4(w, h);
        }
                
        symmetry_lookup_all.push_back(sym);
    }
}

static symmetry_lookup symmetry_init_order_2(const size_t w, const size_t h)
{
    symmetry_lookup sym;
    
    for (size_t i = 0; i < w*h/2; i++)
    {
        std::vector<size_t> mirrors(2);
        mirrors[0] = i;
        mirrors[1] = w*h - 1 - i;
        sym.push_back(mirrors);
    }
    
    return sym;
}

static symmetry_lookup symmetry_init_order_3(const size_t w, const size_t h)
{
    symmetry_lookup sym;
    
    return sym;
}

static symmetry_lookup symmetry_init_order_4(const size_t w, const size_t h)
{
    symmetry_lookup sym;
    
    for (size_t x = 0; x < w/2; x++)
    {
        for (size_t y = 0; y < h/2; y++)
        {
            std::vector<size_t> mirrors(4);
            mirrors[0] = x + y*w;
            mirrors[1] = (w - 1 - x) + y*w;
            mirrors[2] = x + (h - 1 - y)*w;
            mirrors[3] = (w - 1 - x) + (h - 1 - y)*w;
            sym.push_back(mirrors);
        }
    }
    
    return sym;
}

void symmetry_apply(const uint32_t order, const size_t w, const size_t h,
    float_t* im)
{
    if ((order >= SYM_ORDER_MIN) && (order <= SYM_ORDER_MAX))
    {
        symmetry_lookup* sym = &symmetry_lookup_all[order - SYM_ORDER_MIN];
        
        for (size_t i = 0; i < (*sym).size(); i++)
        {
            float_t sum = 0.0;
            for (size_t j = 0; j < order; j++)
            {
                sum += im[(*sym)[i][j]];
            }
            
            float_t average = sum / (float_t)order;
            for (size_t j = 0; j < order; j++)
            {
                im[(*sym)[i][j]] = average;
            }
        }
    }
}
