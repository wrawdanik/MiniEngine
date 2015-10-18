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


#ifndef VECTOR2_H
#define VECTOR2_H

#include "MathUtils.h"
#include <math.h>
#include "Random.h"

namespace MiniEngine
{

	class Vector2
	{
	public:

		float x;
		float y;
        
        
		inline Vector2() {}

		inline Vector2(const float vx, const float vy)
		{
			x=vx; y=vy;
		}

		inline explicit Vector2(const float v)
		{
			x=v; y=v;
		}
		inline explicit Vector2(const float v[])
		{
			x=v[0];y=v[1];
		}
		inline explicit Vector2(const int v[])
		{
			x=(float)v[0];y=(float)v[1];
		}

		inline Vector2( const Vector2& rhs )
		{
			x=rhs.x;y=rhs.y;
		}

        inline Vector2& operator = ( const Vector2& rhs )
        {
            x=rhs.x;y=rhs.y;
            return *this;
        }

        inline Vector2& operator = ( const float v )
        {
			x=v; y=v;
            return *this;
        }
		inline float* ptr()  
		{
			return &x;
		}

		inline const float* ptr() const
		{
			return &x;
		}

		inline float operator [] ( const size_t i ) const
        {
            return *(&x+i);
        }

		inline float& operator [] ( const size_t i )
        {
            return *(&x+i);
        }

		inline bool operator== (const float val) const
		{
			return MathUtils::AlmostEqualComplement(x, val) && MathUtils::AlmostEqualComplement(y, val);
		}


        inline bool operator == ( const Vector2& rhs ) const
        {
            return MathUtils::AlmostEqualComplement(x, rhs.x) && MathUtils::AlmostEqualComplement(y, rhs.y);
        }

        inline bool operator != ( const Vector2& rhs ) const
        {
            return !MathUtils::AlmostEqualComplement(x, rhs.x) || !MathUtils::AlmostEqualComplement(y, rhs.y);
        }

        inline Vector2 operator + ( const Vector2& rhs ) const
        {
            return Vector2(x + rhs.x, y + rhs.y);
        }

        inline Vector2 operator - ( const Vector2& rhs ) const
        {
            return Vector2(x - rhs.x,y - rhs.y);
        }

        inline Vector2 operator * ( const float val ) const
        {
            return Vector2(x * val,y * val);
        }

        inline Vector2 operator * ( const Vector2& rhs) const
        {
            return Vector2(x * rhs.x,y * rhs.y);
        }

        inline Vector2 operator / ( const float val ) const
        {
            return Vector2(x/val, y/val);
        }

        inline Vector2 operator / ( const Vector2& rhs) const
        {
            return Vector2(x / rhs.x, y / rhs.y);
        }

        inline const Vector2& operator + () const
        {
            return *this;
        }

        inline Vector2 operator - () const
        {
            return Vector2(-x, -y);
        }

        inline bool operator < ( const Vector2& rhs ) const
        {
            if( x < rhs.x && y < rhs.y )
                return true;
            return false;
        }

        inline bool operator > ( const Vector2& rhs ) const
        {
            if( x > rhs.x && y > rhs.y  )
                return true;
            return false;
        }


        
		
		// overloaded friends

        inline friend Vector2 operator * ( const float val, const Vector2& rhs )
        {
            return Vector2(val * rhs.x,val * rhs.y);
        }

        inline friend Vector2 operator / ( const float val, const Vector2& rhs )
        {
            return Vector2(val / rhs.x,val / rhs.y);
        }

        inline friend Vector2 operator + (const Vector2& lhs, const float rhs)
        {
            return Vector2(lhs.x + rhs,lhs.y + rhs);
        }

        inline friend Vector2 operator + (const float lhs, const Vector2& rhs)
        {
            return Vector2(lhs + rhs.x,lhs + rhs.y);
        }

        inline friend Vector2 operator - (const Vector2& lhs, const float rhs)
        {
            return Vector2(lhs.x - rhs,lhs.y - rhs);
        }

        inline friend Vector2 operator - (const float lhs, const Vector2& rhs)
        {
            return Vector2(lhs - rhs.x,lhs - rhs.y);
        }

        
        

	// updates

    inline Vector2& operator += ( const Vector2& rhs )
        {
            x += rhs.x; y += rhs.y;
            return *this;
        }

        inline Vector2& operator += ( const float val )
        {
            x += val;y += val;
            return *this;
        }

        inline Vector2& operator -= ( const Vector2& rhs )
        {
            x -= rhs.x;y -= rhs.y;

            return *this;
        }

        inline Vector2& operator -= ( const float val )
        {
            x -= val;y -= val;
            return *this;
        }

        inline Vector2& operator *= ( const float val )
        {
            x *= val;y *= val;
            return *this;
        }

        inline Vector2& operator *= ( const Vector2& rhs )
        {
            x *= rhs.x;y *= rhs.y;

            return *this;
        }

        inline Vector2& operator /= ( const float val )
        {

            x /= val; y /= val;

            return *this;
        }

        inline Vector2& operator /= ( const Vector2& rhs )
        {
            x /= rhs.x; y /= rhs.y;

            return *this;
        }
		
		// methods

        inline Vector2 randomDeviant(
            float radian, float normalizedRandom) const
        {

            radian *=  normalizedRandom * MathUtils::TwoPI;
            float cosa;
            float sina;
			MathUtils::SinCos(radian,sina,cosa);
            return  Vector2(cosa * x - sina * y,
                            sina * x + cosa * y);
        }

		inline Vector2 perpendicular() const
		{
			return Vector2 (-y, x);
		}

        inline float length () const
        {
			return sqrtf( x * x + y * y );
        }
        inline float lengthSqr () const
        {
            return x * x + y * y;
        }
        inline float distance(const Vector2& rhs) const
        {
            return (*this - rhs).length();
        }
        inline float distanceSqr(const Vector2& rhs) const
        {
            return (*this - rhs).lengthSqr();
        }
        inline float dot(const Vector2& rhs) const
        {
            return x * rhs.x + y * rhs.y;
        }
        inline float perpDot(const Vector2& rhs) const
        {
            return -y * rhs.x + x * rhs.y;
        }
        
        inline float rotationTo(const Vector2 &rhs) const
        {
            return atan2f(perpDot(rhs), dot(rhs));
        }
        
        inline Vector2 rotatedWithComplexNumber(const Vector2 &number) const
        {
            return Vector2(x*number.x - y*number.y, x*number.y + y*number.x);
        }
        
        inline Vector2 unRotatedWithComplexNumber(const Vector2 &number) const
        {
            return Vector2(x*number.x + y*number.y, y*number.x - x*number.y);
        }        
        
        inline Vector2 normalized(void) const
        {
            Vector2 ret(x,y);
            ret.normalize();
            return ret;
        }
        
        inline Vector2 rounded(void) const
        {
            return Vector2(roundf(x),roundf(y));
        }        

        inline float normalize()
        {
			float len = sqrtf( x * x + y * y  );

            if ( len > 1e-08 )
            {
                float invLen = 1.0f / len;
                x *= invLen;
                y *= invLen;
            }
            return len;
        }
        
        inline void setRotation(float degree)
        {
            MathUtils::SinCos(MathUtils::DegreeToRadian(degree),y,x);
        }

        inline Vector2 midPoint( const Vector2& rhs ) const
        {
            return Vector2(
                ( x + rhs.x ) * 0.5f,
                ( y + rhs.y ) * 0.5f);
        }

        inline void floor( const Vector2& rhs )
        {
            if( rhs.x < x ) x = rhs.x;
            if( rhs.y < y ) y = rhs.y;
        }

        inline void ceil( const Vector2& rhs )
        {
            if( rhs.x > x ) x = rhs.x;
            if( rhs.y > y ) y = rhs.y;
        }

		inline void zero()	{ x=y=0;}

        inline bool isZeroLength(void) const
        {
            float sqlen = (x * x) + (y * y) ;
            return (sqlen < (1e-06 * 1e-06));

        }

        inline Vector2 reflect(const Vector2& vec) const
        {
            return Vector2( *this - ( 2 * this->dot(vec) * vec ) );
        }
        
        inline void clamp(const Vector2 &min, const Vector2 &max) 
        {
			x = (x > max.x) ? max.x : x;
			x = (x < min.x) ? min.x : x;  
			y = (y > max.y) ? max.y : y;
			y = (y < min.y) ? min.y : y;             
            
        }

		// static stuff
        
        
        static inline Vector2 fromRotation(float degree)
        {
            float x,y;
            MathUtils::SinCos(MathUtils::DegreeToRadian(degree),y,x);
            return Vector2(x,y);
        }
        

		static inline void createMin(const Vector2& value1, const Vector2& value2, Vector2& result)
		{
			result.x = (value1.x < value2.x) ? value1.x : value2.x;
			result.y = (value1.y < value2.y) ? value1.y : value2.y;
		}

 		static inline void createMax(const Vector2& value1, const Vector2& value2, Vector2& result)
		{
			result.x = (value1.x > value2.x) ? value1.x : value2.x;
			result.y = (value1.y > value2.y) ? value1.y : value2.y;
		}

		static void clamp(const Vector2& value1,const Vector2& min, const Vector2& max,Vector2& result)
		{
			result.x = value1.x;
			result.x = (result.x > max.x) ? max.x : result.x;
			result.x = (result.x < min.x) ? min.x : result.x;
			result.y = value1.y;
			result.y = (result.y > max.y) ? max.y : result.y;
			result.y = (result.y < min.y) ? min.y : result.y;
		}

		static inline Vector2 lerp(const Vector2& value1, const Vector2& value2, float amount)
		{
			return Vector2(value1.x + ((value2.x - value1.x)*amount),
						   value1.y + ((value2.y - value1.y)*amount));	
		}


        


		static const Vector2 Zero;
        static const Vector2 UnitX;
        static const Vector2 UnitY;
        static const Vector2 NegUnitX;
        static const Vector2 NegUnitY;
        static const Vector2 One;
        static const Vector2 NegOne;
	};
}

#endif