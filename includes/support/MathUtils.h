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


#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <math.h>
#include <limits>
#include <float.h>
#include <stddef.h>
#include <cassert>


namespace MiniEngine
{



    class MathUtils
    {
	public:
		static const float PI;
        static const float TwoPI;
        static const float HalfPI;
		static const float PiOver180;
		static const float PiUnder180;


		static inline float Abs (const float val) 
		{ 
			union 
			{
				int i;
				float f;
			} fi;

			fi.f = val;
			fi.i = fi.i & 0x7FFFFFFF;
			return fi.f;
		}

		static inline float fmod_pi(float x)
		{
			float x1 = x * (1.0f / PI);
			return PI * (x1 - (int)x1);
		}

		static inline int IntAbs (const int val) { return ( val >= 0 ? val : -val ); }
		static inline int IntCeil (const float val) { return int(ceil(val)); }
		static inline int IntFloor (const float val) { return int(floor(val)); }

		static float ACos (float val);
		static float ASin (float val);

		static inline  float Clamp0(const float val)
		{
			union 
			{
				int i;
				float f;
			} fi;

			fi.f=val;
			int s=fi.i >>31 ;
			s=~s;
			fi.i&=s;
			return fi.f;
		}

		static inline  float Clamp1(const float val)
		{
			union 
			{
				int i;
				float f;
			} fi;

			fi.f=val-1.0f;
			int s=fi.i >>31 ;
			fi.i&=s;
			return fi.f+1.0f;
		}
			
		static inline  float DegreeToRadian(const float val) { return (val*PiOver180); }
		static inline  float RadianToDegree(const float val) { return (val*PiUnder180); }

		static	inline float FRound(float n, unsigned int d)
		{
			return floorf(n * powf(10.0f, (float)d) + .5f) / powf(10.0f, (float)d);
		}


		static inline bool AlmostEqual(float a, float b, float tolerance = FLT_EPSILON)
		{
			if (fabs(b-a) <= tolerance)
					return true;
			else
				return false;
		}
        
        
        static inline bool AlmostEqualInteger(float A, float B, int maxUlps=4)
        {
            assert(sizeof(float) == sizeof(int));
            if (A == B)
                return true;
            int intDiff = abs(*(int*)&A - *(int*)&B);
            if (intDiff <= maxUlps)
                return true;
            return false;
        }
        
        static bool AlmostEqualComplement(float A, float B, int maxUlps=4)
        {
            assert(maxUlps > 0 && maxUlps < 4 * 1024 * 1024);
            int aInt = *(int*)&A;
            if (aInt < 0)
                aInt = 0x80000000 - aInt;
            int bInt = *(int*)&B;
            if (bInt < 0)
                bInt = 0x80000000 - bInt;
            int intDiff = abs(aInt - bInt);
            if (intDiff <= maxUlps)
                return true;
            return false;
        }
        

        // default implementations
        
		inline static void SinCos(float theta, float& s, float& c)
		{

			s=sinf(theta);
			c=cosf(theta);
			return;
		}

        inline static void Sin(float theta, float& s, float& c)
        {
            
            s=sin(theta);
            c=cos(theta);
            return;
        }
        
        static inline float Sin(float val)
        {
            
            return sinf(val);
        }
        
        static inline float Cos(float val)
        {
            
            return cosf(val);
        }


	
		
	};
}

#endif