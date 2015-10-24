//
// Created by Walter Rawdanik on 10/18/15.
//

#include "rdestl.h"
#include "Platform.h"
#include <wrl/client.h>
#include <Windows.h>



rde::string convertWideToString(void *buffer);


using namespace MiniEngine;

Platform::Platform()
{

	//uint64_t mermory =  DeviceStatus.DeviceTotalMemory();




}

constexpr size_t Platform::vertexAlignment()
{
    return 4;
}


constexpr size_t Platform::backendBufferInstances()
{
    return 3;
}


rde::string Platform::computerName()
{
	rde::string result;

	/*

	#define INFO_BUFFER_SIZE MAX_COMPUTERNAME_LENGTH + 1

	TCHAR  infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;

	// Get and display the name of the computer. 
	bufCharCount = INFO_BUFFER_SIZE;
	if (GetComputerName(infoBuf, &bufCharCount))
	{
		return convertWideToString(infoBuf);
	}
	*/
    return result;
}


rde::string convertWideToString(void *inBuffer)
{
	rde::string result;
/*
#ifdef UNICODE
		const size_t size = wcslen(inBuffer) * 2 + 2;
		if (size > 0)
		{
			char *buffer = new char[size];
			wcstombs(buffer, inBuffer, size);
			result = rde::string(buffer);
			delete buffer;
		}
#else
		result = rde::string(infoBuf);
#endif
	*/
	return result;
}
