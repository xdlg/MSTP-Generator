/**************************************************************************//**
 * @file
 * 
 * Container for the parameters of one pattern.
 *****************************************************************************/

#ifndef PATTERN_H
#define PATTERN_H

#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <vector>

class Pattern
{
    public:
        Pattern(uint32_t act_r, uint32_t inh_r, uint32_t wt, float_t sa);
        uint32_t get_act_r(void) const;
        uint32_t get_inh_r(void)const;
        uint32_t get_wt(void) const;
        float_t get_sa(void) const;
        
    private:
        /** Activator radius */
        uint32_t act_r;
        /** Inhibitor radius */
        uint32_t inh_r;
        /** Weight */
        uint32_t wt;
        /** Small amount */
        float_t sa;
};

typedef std::vector<Pattern> pattern_vector;
typedef std::vector<Pattern>::const_iterator pattern_const_iter;

#endif
