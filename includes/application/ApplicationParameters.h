//
// Created by Walter Rawdanik on 10/21/15.
//

#ifndef MINIENGINE_APPLICATIONPARAMETERS_H
#define MINIENGINE_APPLICATIONPARAMETERS_H

#include "rdestl.h"

namespace MiniEngine
{

    class ApplicationParameters
    {
    public:
        using KeyValue=std::tuple<rde::string,rde::string>;
        using KeyList=rde::vector<KeyValue>;


        KeyList keys;
    };
}


#endif //MINIENGINE_APPLICATIONPARAMETERS_H
