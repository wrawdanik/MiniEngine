//
// Created by Walter Rawdanik on 10/14/15.
//

#ifndef MINIENGINE_RANGE_H
#define MINIENGINE_RANGE_H

#include <type_traits>

namespace MiniEngine
{

    template <typename T,
            class=typename std::enable_if< std::is_arithmetic<T>::value, T>::type>
    class Range
    {
        public:

        Range()
        {
            offset=0;size=0;
        }

        Range(T offset,T size)
        {
            this->offset=offset;
            this->size=size;
        }

        Range(const Range<T> &after,T size)
        {
            this->offset=after.offset+after.size;
            this->size=size;
        }

        inline T endOffset() const
        {
            return offset+size-1;
        }

        inline bool isEmpty() const
        {
            return (size==0);
        }

        inline bool operator < (const Range& rhs) const
        {
            return offset<rhs.offset;
        }

        inline bool operator > (const Range& rhs) const
        {
            return offset>rhs.offset;
        }

        inline bool operator == (const Range& rhs) const
        {
            return offset==rhs.offset && size==rhs.size;
        }

        T offset;
        T size;
    };
};


#endif //MINIENGINE_RANGE_H
