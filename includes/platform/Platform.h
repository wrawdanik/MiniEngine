//
// Created by Walter Rawdanik on 10/11/15.
//

#ifndef MINIENGINE_PLATFORM_H
#define MINIENGINE_PLATFORM_H

#include <cstddef>
#include "stdint.h"
#include "rdestl.h"

namespace MiniEngine
{
    class Platform
    {
    public:
        Platform();

        static constexpr size_t vertexAlignment();
        static constexpr size_t backendBufferInstances();


        rde::string computerName();
        rde::string hostName();

        rde::string OsVersion;
        int VersionMinor;
        int VersionMajor;
        int VersionPatch;

        size_t CpuCount;
        uint64_t TotalMemoryInBytes;

    };
}

#endif //MINIENGINE_PLATFORM_H
