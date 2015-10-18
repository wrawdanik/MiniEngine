#include "StringUtils.h"
#include <cstdio>
#include <stdarg.h>

using namespace MiniEngine;


void StringUtils::sprintf(rde::string &target,const char *  format, ...)
{
    va_list arguments;
    va_start( arguments,format );
    sprintf(target,format,arguments);
}

void StringUtils::sprintf(rde::string &target, const char * format, va_list &arguments)
{
    rde::vector<char> buffer(512);
    
    
    sprintf(buffer,format,arguments);
    target.clear();
    target.append(&buffer[0],buffer.size());
}

void StringUtils::sprintf(rde::vector<char> &target, const char * format, va_list &arguments)
{
    int res=vsnprintf(&target[0],(size_t)target.size(),format,arguments);
    if(res>target.size())
    {
        target.resize(res+1);
        vsnprintf(&target[0], target.size(),format,arguments);
    }
}
