#include "Random.h"
#include <limits>
using namespace MiniEngine;

unsigned int Random::rand(uint32_t min, uint32_t max)
{
    uint32_t r;
    const uint32_t range = max - min;
    const uint32_t buckets = (std::numeric_limits<uint32_t>::max()) / range;
    const uint32_t limit = buckets * range;
    
    do
    {
        r = tinymt32_generate_uint32(&tinymt);
    } while (r >= limit);
    
    return min + (r / buckets);
}