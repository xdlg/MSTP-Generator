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

class Pattern
{
    public:
        Pattern(uint32_t act_r, uint32_t inh_r, uint32_t wt, float_t sa);
        uint32_t get_act_r(void);
        uint32_t get_inh_r(void);
        uint32_t get_wt(void);
        float_t get_sa(void);
        
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

#endif
