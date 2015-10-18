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
#include "Transform3d.h"


using namespace MiniEngine;


Transform3d Transform3d::Identity=Transform3d(Vector3(0,0,0),Vector3(1.0f,1.0f,1.0f),Quat(0,0,0,1.0f));

void Transform3d::setDirection(const Vector3 &direction, const Vector3 &localDirection,bool useFixedYaw,const Vector3 &fixedYawAxis)
{
	Vector3 targetDir = direction.normalized();

	Quat targetOrientation;
	if(useFixedYaw)
	{
		Vector3 xVec = fixedYawAxis.cross(targetDir);
		xVec.normalize();
		Vector3 yVec = targetDir.cross(xVec);
		yVec.normalize();
		Quat unitZToTarget(xVec,yVec,targetDir);

		if (localDirection == Vector3::NegUnitZ)
		{
			targetOrientation.w=-unitZToTarget.y;
			targetOrientation.x=-unitZToTarget.z;
			targetOrientation.y=unitZToTarget.w;
			targetOrientation.z=unitZToTarget.x;
		}
		else
		{
			Quat localToUnitZ=localDirection.rotationTo(Vector3::UnitZ);
			targetOrientation=unitZToTarget*localToUnitZ;
		}
	}
	else
	{

		Vector3 currentDir=orientation*localDirection;
		if ((currentDir+targetDir).lengthSqr() < 0.00005f)
		{
			targetOrientation.y=-orientation.y;
			targetOrientation.z=-orientation.z;
			targetOrientation.w=orientation.w;
			targetOrientation.x=-orientation.x;		
		}
		else
		{			
			Quat rotQuat=currentDir.rotationTo(targetDir);
			targetOrientation=rotQuat*orientation;
		}
	}
	orientation=targetOrientation;
}