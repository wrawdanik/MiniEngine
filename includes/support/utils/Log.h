

#ifndef LOG_H
#define LOG_H

#include <stdio.h> 
#include "rdestl.h"
#include <stdarg.h>
#include <mutex>

#ifdef SYSTEM_WIN64
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

namespace MiniEngine
{



class  Log
    {
    public:

		Log();
		Log(const char *filePath);
		~Log();
		
        void msgTrace(const char* fileName, const char *functionName, int lineNum,const char *format,...);
		void msg( const char *format, ...);
		void msgRaw( const char *format, ...);

		inline void setLogEnabled(bool val) { mIsEnabled=val;}
		
		inline bool isLogEnabled() const { return mIsEnabled;}
		
        inline bool isLocked() const { return mIsLocked;}
        inline void setLocked(bool flag) { mIsLocked=flag;};

		static Log& defaultLog()
		{
			static Log log;

			return log;
		}


    private:

		void init(const char *filePath);
		void writeLog(bool useRaw,const char *finalString);

        bool    mIsLocked;
		bool	mIsEnabled;
		rde::string mFilePath;
		FILE		*mFile;
        rde::vector<char> mBuffer;
		std::mutex         mMutex;
	};
	 

#define _Log(d,...) Log::defaultLog().msg(d, ##__VA_ARGS__)	
#define _LogRaw(d,...) Log::defaultLog().msgRaw(d, ##__VA_ARGS__)	    
#define _LogEnable   (Log::defaultLog().setLogEnabled(true)	)
#define _LogDisable   (Log::defaultLog().setLogEnabled(false)	)
#ifdef DEBUG
    #define _LogDebugAssert(l,d,...) if(l)\
										 Log::defaultLog().msgTrace(__FILE__, __PRETTY_FUNCTION__, __LINE__,d,##__VA_ARGS__ )

    #define _LogDebugError(d) Log::defaultLog().msg("Error:",d->errorString.c_str());    
	#define _LogDebug(d,...) Log::defaultLog().msgTrace(__FILE__, __PRETTY_FUNCTION__, __LINE__,d,##__VA_ARGS__ )
	#define _LogDebugWithLog(l,d,...) (l.msgTrace(__FILE__, __PRETTY_FUNCTION__, __LINE__,d,##__VA_ARGS__ )	)
#else
	#define _LogDebugAssert(l,d,...)
	#define _LogDebugError(d)
	#define _LogDebug(d,...)
	#define _LogDebugWithLog(l,d,...) 
#endif
}




#endif