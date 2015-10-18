#include "Status.h"
#include <cstdio>
#include "StringUtils.h"

using namespace MiniEngine;

Status::Status(Code code)
{
    this->statusCode=code;
}

Status::Status( Code code,const char *format,...)
{
    va_list arguments;
    va_start( arguments,format );
    StringUtils::sprintf(statusString,format, arguments);
}

Status::Status(const char *text, Code code):statusString(text)
{
    this->statusCode=code;
}
