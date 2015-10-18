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

#ifndef COLOR_H
#define COLOR_H

#include "Random.h"
#include "MathUtils.h"
#include "rdestl.h"

namespace MiniEngine
{

	class Color
	{
	public:


		Color():r(1.0f),g(1.0f),b(1.0f),a(1.0f)
		{

		}

		Color(float red, float green, float blue, float alpha=1.0f):
			r(red),g(green),b(blue),a(alpha)
			{

			}

        Color(float color,float alpha=1.0f):r(color),g(color),b(color),a(alpha)
        {
            
        }

        inline explicit Color( float* const v )
        {
			r=v[0];g=v[1];b=v[2];a=v[3];
        }        
        
		inline explicit Color(const float v[])
		{
			r=v[0];g=v[1];b=v[2];a=v[3];
		}       
              
        
        Color saturated() const
        {
            Color tmp(*this); 
            tmp.saturate();
            return tmp;
        }
        
        
		 void saturate(void)
        {
            if (r < 0)
                r = 0;
            else if (r > 1.0f)
                r = 1.0f;

            if (g < 0)
                g = 0;
            else if (g > 1.0f)
                g = 1.0f;

            if (b < 0)
                b = 0;
            else if (b > 1.0f)
                b = 1.0f;

            if (a < 0)
                a = 0;
            else if (a > 1.0f)
                a = 1.0f;
        }

		inline float operator [] ( const size_t i ) const
		{
			return *(&r+i);
		}
		inline float& operator [] ( const size_t i )
		{
			return *(&r+i);
		}

		inline float* ptr()
		{
			return &r;
		}
		inline const float* ptr() const
		{
			return &r;
		}

        inline Color operator + ( const Color& rkVector ) const
        {
            Color kSum;

            kSum.r = r + rkVector.r;
            kSum.g = g + rkVector.g;
            kSum.b = b + rkVector.b;
            kSum.a = a + rkVector.a;

            return kSum;
        }

        inline Color operator - ( const Color& rkVector ) const
        {
            Color kDiff;

            kDiff.r = r - rkVector.r;
            kDiff.g = g - rkVector.g;
            kDiff.b = b - rkVector.b;
            kDiff.a = a - rkVector.a;

            return kDiff;
        }

        inline Color operator * (const float fScalar ) const
        {
            Color kProd;

            kProd.r = (fScalar*r);
            kProd.g = (fScalar*g);
            kProd.b = (fScalar*b);
            kProd.a = (fScalar*a);

            return kProd;
        }

        inline Color operator * ( const Color& rhs) const
        {
            Color kProd;

            kProd.r = (rhs.r * r);
            kProd.g = (rhs.g * g);
            kProd.b = (rhs.b * b);
            kProd.a = (rhs.a * a);

            return kProd;
        }

        inline Color operator / ( const Color& rhs) const
        {
            Color kProd;

            kProd.r = (rhs.r / r);
            kProd.g = (rhs.g /g);
            kProd.b = (rhs.b / b);
            kProd.a = (rhs.a / a);

            return kProd;
        }

        inline Color operator / (const float fScalar ) const
        {
            assert( fScalar != 0.0 );

            Color kDiv;

            float fInv = (1.0f / fScalar);
            kDiv.r = (r * fInv);
            kDiv.g = (g * fInv);
            kDiv.b = (b * fInv);
            kDiv.a = (a * fInv);

            return kDiv;
        }

        inline friend Color operator * (const float fScalar, const Color& rkVector )
        {
            Color kProd;

            kProd.r = (fScalar * rkVector.r);
            kProd.g = (fScalar * rkVector.g);
            kProd.b = (fScalar * rkVector.b);
            kProd.a = (fScalar * rkVector.a);

            return kProd;
        }

        inline Color& operator += ( const Color& rkVector )
        {
            r += rkVector.r;
            g += rkVector.g;
            b += rkVector.b;
            a += rkVector.a;

            return *this;
        }

        inline Color& operator -= ( const Color& rkVector )
        {
            r -= rkVector.r;
            g -= rkVector.g;
            b -= rkVector.b;
            a -= rkVector.a;

            return *this;
        }

        inline Color& operator *= ( const Color& rkVector )
        {
            r *= rkVector.r;
            g *= rkVector.g;
            b *= rkVector.b;
            a *= rkVector.a;

            return *this;
        }

        inline Color& operator /= ( const Color& rkVector )
        {
            r /= rkVector.r;
            g /= rkVector.g;
            b /= rkVector.b;
            a /= rkVector.a;

            return *this;
        }

        inline Color& operator *= (const float fScalar )
        {
            r *= fScalar;
            g *= fScalar;
            b *= fScalar;
            a *= fScalar;
            return *this;
        }

        inline Color& operator /= (const float fScalar )
        {           

            float fInv = (1.0f / fScalar);

            r *= fInv;
            g *= fInv;
            b *= fInv;
            a *= fInv;

            return *this;
        }

       inline bool operator == ( const Color& rhs ) const
        {
		return (rgba() == rhs.rgba());
	   }
       inline bool operator != ( const Color& rhs ) const
        {
		return (rgba() != rhs.rgba());
	   }

		static inline uint32_t createRgba(float r, float g, float b, float a)
		{
			unsigned char val8;
            uint32_t val32 = 0;


			val8 = static_cast<unsigned char>(a* 255);
			val32 = val8 << 24;

			val8 = static_cast<unsigned char>(b * 255);
			val32 += val8 << 16;

			val8 = static_cast<unsigned char>(g * 255);
			val32 += val8 << 8;

			val8 = static_cast<unsigned char>(r * 255);
			val32 += val8;

			return val32;
		}
      
        
        

		inline uint32_t rgba() const
		{
			unsigned char val8;
            uint32_t val32 = 0;

	
			val8 = static_cast<unsigned char>(a* 255);
			val32 = val8 << 24;

			val8 = static_cast<unsigned char>(b * 255);
			val32 += val8 << 16;

			val8 = static_cast<unsigned char>(g * 255);
			val32 += val8 << 8;

			val8 = static_cast<unsigned char>(r * 255);
			val32 += val8;

			return val32;
		}

		static inline uint32_t createBgra(float r, float g, float b, float a)
		{
			unsigned char val8;
            uint32_t val32 = 0;


			val8 = static_cast<unsigned char>(b * 255);
			val32 = val8 << 24;

			val8 = static_cast<unsigned char>(g * 255);
			val32 += val8 << 16;

			val8 = static_cast<unsigned char>(r * 255);
			val32 += val8 << 8;

			val8 = static_cast<unsigned char>(a * 255);
			val32 += val8;


			return val32;
		}

		inline uint32_t bgra() const
		{
			unsigned char val8;
            uint32_t val32 = 0;


			val8 = static_cast<unsigned char>(b * 255);
			val32 = val8 << 24;

			val8 = static_cast<unsigned char>(g * 255);
			val32 += val8 << 16;

			val8 = static_cast<unsigned char>(r * 255);
			val32 += val8 << 8;

			val8 = static_cast<unsigned char>(a * 255);
			val32 += val8;


			return val32;
		}

        void getHSB(float  &hue, float  &saturation, float &brightness) const;

		void setHSB(float hue, float saturation, float brightness);


		static inline Color create(uint32_t rgba)
		{
			return Color(((float)(rgba&0x000000FF))/255.0f,
						 ((float)((rgba >> 8) &0x000000FF))/255.0f,
						 ((float)((rgba >> 16) &0x000000FF))/255.0f,
						 ((float)(rgba >> 24))/255.0f);
		}

		static inline Color create(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		{
			return Color((float)r/255.0f,(float)g/255.0f,(float)b/255.0f,(float)a/255.0f);
		}

		static inline Color interpolate(const Color &color1, const Color &color2, float t)
		{
			return Color((color2.r * t) + (color1.r * (1.0f - t)),(color2.g * t) + (color1.g * (1.0f - t)),(color2.b * t) + (color1.b * (1.0f - t)),(color2.a * t) + (color1.a * (1.0f - t)));
		}

		float calculateBrightness()
		{
			return sqrtf(r * r * .241f + g * g * .691f + b * b * .068f);		
		}

        Color scaledB(float scale);
        
        /*
		static inline Color createRandom(float min=0.0f, float max=1.0f)
		{
			return Color(Random::randf(min,max),Random::randf(min,max),Random::randf(min,max),1.0f);
		}
         */

	float r,g,b,a;

		static Color White;
		static Color Black;
		static Color Red;
		static Color Green;
		static Color Blue;
		static Color Pink;
		static Color Gray;

	};

	typedef rde::vector<Color> ColorArray;


	class ColorQuad
	{
	public:
		
		inline ColorQuad()
		{
			mColors[0]=0xFFFFFFFF;
			mColors[1]=0xFFFFFFFF;
			mColors[2]=0xFFFFFFFF;
			mColors[3]=0xFFFFFFFF;
		}

		inline ColorQuad(const Color &color1,const Color &color2,const Color &color3,const Color &color4)
		{
			mColors[0]=color1.rgba();
			mColors[1]=color2.rgba();
			mColors[2]=color3.rgba();
			mColors[3]=color4.rgba();
		}

		inline ColorQuad(const Color &color1)
		{
			mColors[0]=color1.rgba();
			mColors[1]=mColors[0];
			mColors[2]=mColors[0];
			mColors[3]=mColors[0];
		}
		
        inline void setColor(const Color &color)
        {
			mColors[0]=color.rgba();
			mColors[1]=mColors[0];
			mColors[2]=mColors[0];
			mColors[3]=mColors[0];            
        }
    
        
		inline void setColors(const Color &color1,const Color &color2,const Color &color3,const Color &color4)
		{
			mColors[0]=color1.rgba();
			mColors[1]=color2.rgba();
			mColors[2]=color3.rgba();
			mColors[3]=color4.rgba();
		}

        uint32_t mColors[4];

		static ColorQuad White;
        static ColorQuad Black;
	};

	typedef rde::vector<ColorQuad> ColorQuadArray;    
    
    class ColorValue
    {
    public:
        inline ColorValue():color(Color::White),value(1.0f) {}
        inline ColorValue(const Color &ccolor, float cvalue):color(ccolor),value(cvalue) {}
        
        Color color;
        float value;
    };
    
    
    

	class ColorRange
	{
	public:

		explicit ColorRange(const Color &color)
		{
			mColorRange.push_back(ColorValue(color,0));
		}
		ColorRange(const Color &color1, const Color &color2)
		{
			mColorRange.push_back(ColorValue(color1,0));
			mColorRange.push_back(ColorValue(color2,1.0f));
		}

		ColorRange()
		{

		}


		inline void addColorValue(const ColorValue &value)	{ mColorRange.push_back(value); }
		inline const ColorValue&	colorValue(int index) const { return mColorRange[index];}
		inline void clear() { mColorRange.clear();}
		inline size_t size() const { return mColorRange.size();}

		Color interpolatedColor(float t) const;
		void interpolatedColor(Color &color,float t) const;

	private:
		rde::vector<ColorValue>	mColorRange;

	};

}

#endif