/**************************************************************************//**
 * @file
 * 
 * Container for the parameters of one pattern.
 *****************************************************************************/

#include "Pattern.h"

Pattern::Pattern(uint32_t act_r, uint32_t inh_r, uint32_t wt, float_t sa)
{    
    this->act_r = act_r;
    this->inh_r = inh_r;
    this->wt = wt;
    this->sa = sa;
}

uint32_t Pattern::get_act_r(void)
{
    return act_r;
}

uint32_t Pattern::get_inh_r(void)
{
    return inh_r;
}

float_t Pattern::get_sa(void)
{
    return sa;
}

uint32_t Pattern::get_wt(void)
{
    return wt;
}
