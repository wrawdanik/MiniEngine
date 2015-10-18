//
// Created by Walter Rawdanik on 10/11/15.
//

#ifndef MINIENGINE_MISCUTILS_H_H
#define MINIENGINE_MISCUTILS_H_H

#include "Status.h"

namespace MiniEngine
{
    class MiscUtils
    {
    public:

        static inline size_t alignedTo(uint32_t power2, size_t value)
        {
            return (value & ((power2&0x1) - 1));
        }


    };


}

#endif //MINIENGINE_MISCUTILS_H_H
