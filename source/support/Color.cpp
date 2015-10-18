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

#include "Color.h"

using namespace MiniEngine;


Color	Color::White;
Color	Color::Black(0.0f,0.0f,0.0f,1.0f);
Color	Color::Red(1.0f,0.0f,0.0f,1.0f);
Color	Color::Green(0.0f,1.0f,0.0f,1.0f);
Color	Color::Blue(0.0f,0.0f,1.0f,1.0f);
Color	Color::Pink(1.0f,0,1.0f,1.0f); 
Color	Color::Gray(0.5f,0.5f,0.5f,1.0f); 

ColorQuad ColorQuad::White(Color(1.0f));
ColorQuad ColorQuad::Black(Color(0.0f));

void Color::getHSB(float  &hue, float  &saturation, float &brightness) const
{
    
    float vMin = rde::min(r, rde::min(g, b));
    float vMax = rde::max(r, rde::max(g, b));
    float delta = vMax - vMin;
    
    brightness = vMax;
    
    if (MathUtils::AlmostEqualComplement(delta, 0.0f))
    {
        // grey
        hue = 0;
        saturation = 0;
    }
    else                                    
    {
        // a colour
        saturation = delta / vMax;
        
        float deltaR = (((vMax - r) / 6.0f) + (delta / 2.0f)) / delta;
        float deltaG = (((vMax - g) / 6.0f) + (delta / 2.0f)) / delta;
        float deltaB = (((vMax - b) / 6.0f) + (delta / 2.0f)) / delta;
        
        if (MathUtils::AlmostEqualComplement(r, vMax))
            hue = deltaB - deltaG;
        else if (MathUtils::AlmostEqualComplement(g, vMax))
            hue = 0.3333333f + deltaR - deltaB;
        else if (MathUtils::AlmostEqualComplement(b, vMax)) 
            hue = 0.6666667f + deltaG - deltaR;
        
        if (hue < 0.0f) 
            hue += 1.0f;
        if (hue > 1.0f)
            hue -= 1.0f;
    }
    
}
void Color::setHSB(float hue, float saturation, float brightness)
{

	// wrap hue
	if (hue > 1.0f)
	{
		hue -= (int)hue;
	}
	else if (hue < 0)
	{
		hue += (int)hue + 1;
	}
	// clamp saturation / brightness

	saturation = rde::min<float>(saturation, 1.0f);
	saturation = rde::max<float>(saturation, 0);
	brightness = rde::min<float>(brightness, 1.0f);
	brightness = rde::max<float>(brightness, 0);

	if (brightness == 0)
	{   
		// early exit, this has to be black
		r = g = b = 0;
		return;
	}

	if (saturation == 0)
	{   
		// early exit, this has to be grey

		r = g = b = brightness;
		return;
	}


	float hueDomain  = hue *6.0f;
	if (hueDomain >= 6.0f)
	{
		// wrap around, and allow mathematical errors
		hueDomain = 0.0f;
	}
	unsigned short domain = (unsigned short)hueDomain;
	float f1 = (brightness * (1.0f- saturation));
	float f2 = (brightness * (1.0f- (saturation * (hueDomain - domain))));
	float f3 = (brightness * (1.0f - (saturation * (1.0f - (hueDomain - domain)))));

	switch (domain)
	{
	case 0:
		// red domain; green ascends
		r = brightness;
		g = f3;
		b = f1;
		break;
	case 1:
		// yellow domain; red descends
		r = f2;
		g = brightness;
		b = f1;
		break;
	case 2:
		// green domain; blue ascends
		r = f1;
		g = brightness;
		b = f3;
		break;
	case 3:
		// cyan domain; green descends
		r = f1;
		g = f2;
		b = brightness;
		break;
	case 4:
		// blue domain; red ascends
		r = f3;
		g = f1;
		b = brightness;
		break;
	case 5:
		// magenta domain; blue descends
		r = brightness;
		g = f1;
		b = f2;
		break;
	}


}

Color Color::scaledB(float scale)
{
    float h,s,b;
    
    Color tmp=*this;
    
    tmp.getHSB(h, s, b);
    b*=scale;
    tmp.setHSB(h, s, b);
    
    return tmp;
}



Color ColorRange::interpolatedColor(float t) const 
{
	if(t>1)
		t=1;
	else if(t<0)
		t=0;

	float fSeg = t * (mColorRange.size() - 1);

	uint32_t segIdx = (uint32_t)fSeg;
	t = fSeg - segIdx;

	if ((segIdx + 1) == mColorRange.size())
	{
		return mColorRange[segIdx].color;
	}
	else if (t <= 0.0f)
	{
		return mColorRange[segIdx].color;
	}
	else if(t >= 1.0f)
	{
		return mColorRange[segIdx].color;
	}
	else
	{
		return Color::interpolate(mColorRange[segIdx].color,mColorRange[segIdx+1].color,t);
	}

}

void ColorRange::interpolatedColor(Color &interColor,float t) const 
{
	if(t>1)
		t=1;
	else if(t<0)
		t=0;

	float fSeg = t * (mColorRange.size() - 1);
	unsigned int segIdx = (unsigned int)fSeg;
	t = fSeg - segIdx;

	if ((segIdx + 1) == mColorRange.size())
	{
		interColor=mColorRange[segIdx].color;
	}
	else if (t <= 0.0f)
	{
		interColor=mColorRange[segIdx].color;
	}
	else if(t >= 1.0f)
	{
		interColor=mColorRange[segIdx].color;
	}
	else
	{
		interColor=Color::interpolate(mColorRange[segIdx].color,mColorRange[segIdx+1].color,t);
	}

	return;

}
