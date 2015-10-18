#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdarg.h>
#include "rdestl.h"


namespace MiniEngine
{
    class StringUtils
    {
    public:
        
        static void sprintf(rde::string &target,const char * format, ...);
        static void sprintf(rde::string &target, const char * format, va_list &arguments);
        static void sprintf(rde::vector<char> &targer, const char * format, va_list &arguments);
    };
    
    
}



#endif