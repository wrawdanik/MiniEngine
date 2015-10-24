#ifndef ERROR_H
#define ERROR_H


#include "IntrusiveTracked.h"
#include "rdestl.h"

namespace MiniEngine
{


    class Status : public IntrusiveTracked
    {
    public:

        enum class  Code: size_t
        {
            Success,
            InProgress,
            InvalidParameter,
            OutOfMemory,
            NativeOperationFailed,
            NotImplemented,
            InvalidState,
            InvalidResourceId,
            CommunicationError,
            Aborted,
            Failure
        };
        Status()
        {
            statusCode=Code::Success;
        }
        Status(Code code);
        Status(const char *text, Code code) ;
        Status( Code code,const char *format,...) ;
        rde::string statusString;
        Code statusCode;

        bool hasError() const
        {
            return static_cast<size_t>(statusCode)< static_cast<size_t>(Code::InvalidParameter);
        }


    };

    typedef   intrusive_ptr<Status> StatusPtr;
    
}



#endif