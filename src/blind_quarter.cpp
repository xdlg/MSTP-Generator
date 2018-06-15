/**************************************************************************//**
 * @file
 * 
 * Multi-scale Turing patterns based on Jonathan McCabe's work.
 *****************************************************************************/

#include "blind_quarter.h"
#include <ctime>
#include <cfloat>
#include "blur.h"

/**************************************************************************//**
 * Normalizes the image to the interval [0; 1].
 * 
 * @param[in] w Image width
 * @param[in] h Image height
 * @param[inout] im Image
 *****************************************************************************/
static void normalize(const size_t w, const size_t h, float_t* im);

void blind_quarter_init_image(const size_t w, const size_t h, float_t* im)
{
	srand(time(NULL));
	for (size_t i = 0; i < w*h; i++)
	{
        im[i] = (float_t)rand()/(float_t)(RAND_MAX);
	}
}

void blind_quarter_step(pattern_vector& patterns, const size_t w, const size_t h,
    float_t* im)
{   
    if (!patterns.empty())
    {
        float_t* var = new float_t[w*h];
        const Pattern** best_patterns = new const Pattern*[w*h];
        
        // For each pattern...
        for (pattern_const_iter p = patterns.begin(); p != patterns.end(); p++)
        {
            // Compute activator and inhibitor arrays
            float_t* act = new float_t[w*h];
            float_t* inh = new float_t[w*h];
            blur(w, h, p->get_act_r(), p->get_wt(), im, act);
            blur(w, h, p->get_inh_r(), p->get_wt(), im, inh);
            
            // For each pixel...
            for (size_t j = 0; j < w*h; j++)
            {
                // Update the variation array if the variation for this pixel
                // is smaller than the one already stored. This way,
                // the variation array always stores the smallest variation.
                // When processing the first pattern, the variation array is
                // always updated, so we don't need to initialize it beforehand.
                float_t var_new = act[j] - inh[j];
                if ((fabs(var_new) < fabs(var[j])) || (p == patterns.begin()))
                {
                    var[j] = var_new;
                    best_patterns[j] = &(*p);
                }
            }
            
            delete [] act;
            delete [] inh;
        }
        
        // For each pixel, add the small amount if the activator was larger
        // than the inhibitor, subtract otherwise
        for (size_t j = 0; j < w*h; j++)
        {
            if (var[j] > 0)
            {
                im[j] += best_patterns[j]->get_sa();
            }
            else
            {
                im[j] -= best_patterns[j]->get_sa();
            }
        }

        delete [] var;
        delete [] best_patterns;
        
        normalize(w, h, im);
    }
}

static void normalize(const size_t w, const size_t h, float_t *im)
{
	float_t max = FLT_MIN;
    float_t min = FLT_MAX;
    
    for (size_t i = 0; i < w*h; i++)
	{
		if (im[i] > max)
		{
			max = im[i];
		}
        if (im[i] < min)
		{
			min = im[i];
		}
	}
	float_t range = max - min;
	
	for (size_t i = 0; i < w*h; i++)
	{
		im[i] = (im[i] - min)/range;
	}
}
