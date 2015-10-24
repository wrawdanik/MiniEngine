//
// Created by Walter Rawdanik on 10/18/15.
//



#include "Platform.h"
#import <Foundation/NSAutoreleasePool.h>
#import <Foundation/NSProcessInfo.h>
#import <Foundation/NSString.h>
#include <SystemConfiguration/SystemConfiguration.h>
#include <Foundation/Foundation.h>


using namespace MiniEngine;

Platform::Platform()
{
    VersionMinor=VersionMajor=VersionPatch=CpuCount=0;

    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];


    NSProcessInfo *processInfo=[NSProcessInfo processInfo];

    if ([processInfo respondsToSelector:@selector(operatingSystemVersionString)])
    {
        NSString *native =[processInfo operatingSystemVersionString];
        if (native != NULL)
            OsVersion = rde::string( [native UTF8String]);
    }
    if ([processInfo respondsToSelector:@selector(operatingSystemVersion)])
    {
        NSOperatingSystemVersion native =[processInfo operatingSystemVersion];
        VersionMinor=native.minorVersion;
        VersionMajor=native.majorVersion;
        VersionPatch=native.patchVersion;
    }

    if ([processInfo respondsToSelector:@selector(processorCount)])
    {
        CpuCount =(size_t)([processInfo processorCount]);
    }

    if ([processInfo respondsToSelector:@selector(physicalMemory)])
    {
        TotalMemoryInBytes =(uint64_t)([processInfo physicalMemory]);
    }

    [pool drain];

}



rde::string Platform::computerName()
{
    rde::string result;

    CFStringRef temp = SCDynamicStoreCopyComputerName (NULL, NULL);
    if (temp != NULL)
    {
        CFIndex length = CFStringGetLength(temp);
        CFIndex maxSize = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
        char *buffer = (char *) malloc(maxSize);
        if (CFStringGetCString(temp, buffer, maxSize, kCFStringEncodingUTF8))
        {
            result = rde::string(buffer);
            free(buffer);
        }
        CFRelease(temp);
    }
    return result;
}
rde::string Platform::hostName()
{
    rde::string result;

    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    NSProcessInfo *processInfo=[NSProcessInfo processInfo];

    if ([processInfo respondsToSelector:@selector(hostName)])
    {
        NSString *native =[processInfo hostName];
        if (native != NULL)
            result = rde::string( [native UTF8String]);
    }

    [pool drain];

    return result;
}

