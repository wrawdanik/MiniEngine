/*
-----------------------------------------------------------------------------

Copyright (c) 2009 Walter Rawdanik	

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "Log.h"
#include "StringUtils.h"
#include <cstdio>
#include <time.h>


using namespace MiniEngine;

Log::Log():mFilePath("MiniEngineLog.txt")
{
    mIsLocked=true;
	init("MiniEngineLog.txt");
}
Log::Log(const char  *filePath)
{
    mIsLocked=true;
	init(filePath);
}

void Log::init(const char *filePath)
{
	mFile=0;
	if(filePath!=0)
		mFile = fopen(filePath,"w"); 
	mIsEnabled=true;
    mBuffer.resize(512);
}

Log::~Log()
{
	if(mFile!=0)
		fclose(mFile);
}
		
void Log::msgTrace(const char* fileName, const char *functionName,int lineNum,const char *format,...)
{
	if(mIsEnabled)
	{
        bool setLock=mIsLocked;
        
        msg( "%s  %s  :%d ",fileName,functionName,lineNum);
        if(setLock)
            mMutex.lock();
		va_list arguments;
		va_start( arguments,format );
        StringUtils::sprintf(mBuffer, format, arguments);
		writeLog(true,&mBuffer[0]);
        writeLog(true,"\n");
        if(setLock)
            mMutex.unlock();
	}    
    
}


void Log::msg(const char *format, ...)
{
	if(mIsEnabled)
	{
        bool setLock=mIsLocked;
        if(setLock)
            mMutex.lock();
		va_list arguments;  
		va_start( arguments,format );
        StringUtils::sprintf(mBuffer, format, arguments);
        writeLog(true,&mBuffer[0]);
        if(setLock)
            mMutex.unlock();
	}

}
 
void Log::msgRaw( const char *format, ...)
{
	if(mIsEnabled)
	{
        bool setLock=mIsLocked;
        if(setLock)
            mMutex.lock();
		va_list arguments;  
		va_start( arguments,format );
        StringUtils::sprintf(mBuffer, format, arguments);
        writeLog(true,&mBuffer[0]);
        if(setLock)
            mMutex.unlock();
	}
}


void Log::writeLog(bool useRaw,const char *finalString)
{
	if(useRaw)
	{
		if(mFile)
        {
			fprintf(mFile,"%s",finalString);
			fflush (mFile );
        }
        printf("%s",finalString);
		fflush(stdout);
	}
	else
	{
		struct tm *pTime;
		time_t ctTime; time(&ctTime);
		pTime = localtime( &ctTime );
        
		if(mFile)
		{
			fprintf(mFile,"%d:%d:%d %s\n",pTime->tm_hour,pTime->tm_min,pTime->tm_sec, finalString);
			fflush (mFile );
		}
        printf("%d:%d:%d %s\n",pTime->tm_hour,pTime->tm_min,pTime->tm_sec,finalString);
		fflush(stdout);
	}
}