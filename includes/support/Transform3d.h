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


#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vector4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Quat.h"
#include "Matrix4.h"
#include "Matrix3.h"
#include "GeometryUtils.h"

namespace MiniEngine
{
	struct Transform3d
	{	
        Transform3d() {}
        
        Transform3d(const Vector3 &pos):position(pos),scale(Vector3::One),orientation(Quat::Identity)
        {
            
        }
        
        
        Transform3d(const Vector3 &pos, const Vector3 &sc, const Quat &rot):position(pos),scale(sc),orientation(rot)
        {
        }
        
        
		void reset()
		{
			position=Vector3::Zero;
			scale=Vector3::One;
			orientation=Quat::Identity;
		}

		inline void rotate(float radians,const Vector3 &axis)
		{
			orientation=orientation*Quat(radians,axis);				
		}

		inline void yaw(float radians)
		{
			orientation=orientation*Quat(radians,Vector3::UnitY);
		}

		inline void pitch(float radians)
		{
			orientation=orientation*Quat(radians,Vector3::UnitX);
		}

		inline void roll(float radians)
		{
			orientation=orientation*Quat(radians,Vector3::UnitZ);
		}

		inline void parentTransform(const Transform3d &parent,Transform3d &out)
		{
			out.orientation=parent.orientation*orientation;
			out.scale=parent.scale*scale;
			out.position=parent.orientation*(parent.scale*position);
			out.position+=parent.position;
		}

		inline void parentTransform(const Transform3d &parent)
		{
			Quat nOrient;
			Vector3 nPos;
			Vector3 nScale;
			nOrient=parent.orientation*orientation;
			nScale=parent.scale*scale;
			nPos=parent.orientation*(parent.scale*position);
			nPos+=parent.position;
			position=nPos;
			orientation=nOrient;
			scale=nScale;
		}

		inline void lookAt(const Vector3 &target,const Vector3 &localDirection=Vector3::NegUnitZ, bool useFixedYaw=false,const Vector3 &fixedYawAxis=Vector3::UnitY)
		{
			setDirection(target-position,localDirection,useFixedYaw,fixedYawAxis);
		}

		void setDirection(const Vector3 &direction, const Vector3 &localDirection,bool useFixedYaw=false,const Vector3 &fixedYawAxis=Vector3::UnitY);

		Vector3 position;
		Vector3 scale;
		Quat	orientation;
        
        static Transform3d Identity;

	};


}




#endif