/**************************************************************************//**
 * @file
 * 
 * Rotational symmetry functions for the activator/inhibitor arrays.
 *****************************************************************************/
 
#include "symmetry.h"

/** Minimum symmetry order */
#define SYM_ORDER_MIN   2
/** Maximum symmetry order */
#define SYM_ORDER_MAX   5
/** Pi */
#define PI              3.14159265

/**
 * A symmetry lookup table is a list of symmetrical pixel coordinates.
 * Assuming that the symmetry order M is applied to N pixels, the lookup table
 * should have the dimensions [N][M]. Each row corresponds to the linear
 * coordinates of the original pixel, and each column to the linear coordinates
 * of all mirrored pixels.
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
 * symmetrical with pixel 3, 1 with 2, 2 with 1 and 3 with 0. (On a small
 * example it's obvious, but for the general case the mirrored coordinates must
 * be calculated with a rotation matrix.) After calculation, the resulting
 * lookup table should look like this:
 *  -------
 * | 0 | 3 |
 *  -------
 * | 1 | 2 |
 *  -------
 * | 2 | 1 |
 *  -------
 * | 3 | 0 |
 *  -------
 * 
 * If we had a symmetry order 4:
 *  ---------------
 * | 0 | 1 | 3 | 2 |
 *  ----------------
 * | 1 | 3 | 2 | 0 |
 *  ---------------
 * | 2 | 0 | 1 | 3 |
 *  ---------------
 * | 3 | 2 | 0 | 1 |
 *  ---------------
 * 
 * And so on. Of course there are duplicates, which could be a problem with the
 * resulting symmetries. But at the moment it's much simpler to caculate the
 * mirrors for all pixels than to determine which pixels can be skipped because
 * their mirrors have already been found.
 */
typedef std::vector<std::vector<size_t>> symmetry_lookup;

/** Symmetry lookup tables for all orders up to SYMMETRY_ORDER_MAX */
static std::vector<symmetry_lookup> symmetry_lookup_all;

void symmetry_init(const size_t w, const size_t h)
{    
    for (size_t sym_order = SYM_ORDER_MIN; sym_order < SYM_ORDER_MAX; sym_order++)
    {
        // Symmetry lookup table for this order 
        symmetry_lookup sym(w*h, std::vector<size_t>(sym_order));
        
        // For each pixel coordinates...
        for (size_t x = 0; x < w; x++)
        {
            for (size_t y = 0; y < h; y++)
            {
                // The first element is always the original coordinates
                sym[x + y*w][0] = x + y*w;
                
                // The rest is for the mirrored elements...
                for (size_t i = 1; i < sym_order; i++)
                {
                    // Since we want a rotational symmetry around the center,
                    // we first need the coordinates of the center
                    double_t x_c = ((double_t)w - 1.0)/2.0;
                    double_t y_c = ((double_t)h - 1.0)/2.0;
                    
                    // The coordinates are translated relatively to the center
                    double_t x_t = (double_t)x - x_c;
                    double_t y_t = (double_t)y - y_c;
                    
                    // Calculate the rotated coordinates
                    double_t cos_i = std::cos(i*M_PI/sym_order);
                    double_t sin_i = std::sin(i*M_PI/sym_order);
                    double_t x_r = x_t*cos_i - y_t*sin_i;
                    double_t y_r = x_t*sin_i + y_t*cos_i;
                    
                    // Bring the coordinates back to the original referential
                    x_r += x_c;
                    y_r += y_c;
                    
                    // If the new (linear) coordinates are out of bounds, just
                    // take the original coordinates. The symmetry operations
                    // for that specific point will have no effect, which
                    // shouldn't be too visible amid the general chaos.
                    double_t xy_r = std::round(x_r + y_r*(double_t)w);
                    if ((xy_r < 0.0) || (xy_r >= (double_t)(w*h)))
                    {
                        xy_r = x + y*w;
                    }
                    
                    sym[x + y*w][i] = xy_r;
                }
            }
        }
        
        symmetry_lookup_all.push_back(sym);
    }
}

void symmetry_apply(const uint32_t order, const size_t w, const size_t h,
    float_t* im)
{
    if ((order >= SYM_ORDER_MIN) && (order <= SYM_ORDER_MAX))
    {
        symmetry_lookup* sym = &symmetry_lookup_all[order - SYM_ORDER_MIN];
        
        for (size_t i = 0; i < w*h; i++)
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
