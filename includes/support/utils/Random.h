/***************************** RANDOMC.H *********************** 2001-10-24 AF *
*
*
* class TRanrotBGenerator:
* Random number generator of type RANROT-B.
* Source file ranrotb.cpp
*
*
*******************************************************************************/

#ifndef RANDOMC_H
#define RANDOMC_H

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include "tinymt32.h"


namespace MiniEngine
{

    class Random 
    {
    public:
        
        Random();
        
        Random(uint32_t seed)
        {
            tinymt32_init(&tinymt, seed);
        }
        
        
        void seed(uint32_t seed)
        {
            tinymt32_init(&tinymt, seed);
        }
        unsigned int rand(uint32_t min, uint32_t max);
        unsigned int rand()
        {
            return tinymt32_generate_uint32(&tinymt);
        }
        
        float randf()
        {
            return tinymt32_generate_float(&tinymt);
        }
        float randf(float min, float max)
        {
            return (min + tinymt32_generate_float(&tinymt) * (max - min));
        }
        
        float symmetricRand()
        {
            return 2.0f * tinymt32_generate_float(&tinymt) - 1.0f;
        }
        
    private:        
        
         tinymt32_t tinymt;
        
    };

}
#endif  