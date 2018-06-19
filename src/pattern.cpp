/**************************************************************************//**
 * @file
 * 
 * Container for the parameters of one pattern.
 *****************************************************************************/

#include "pattern.h"

Pattern::Pattern(uint32_t act_r, uint32_t inh_r, uint32_t wt, uint32_t sym,
    float_t sa)
{    
    this->act_r = act_r;
    this->inh_r = inh_r;
    this->wt = wt;
    this->sym = sym;
    this->sa = sa;
}

uint32_t Pattern::get_act_r(void) const
{
    return act_r;
}

uint32_t Pattern::get_inh_r(void) const
{
    return inh_r;
}

uint32_t Pattern::get_wt(void) const
{
    return wt;
}

uint32_t Pattern::get_sym(void) const
{
    return sym;
}

float_t Pattern::get_sa(void) const
{
    return sa;
}

