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

 THE SOFTWARE IS PROVIDED "tranAS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------
*/


#ifndef GEOMETRY_UTILS_H
#define GEOMETRY_UTILS_H

#include "Matrix4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Global.h"
#include <float.h>
#include "rdestl.h"

namespace MiniEngine
{



	class BoundingSphere;
	class AABoundingBox;
	class Plane;

	class Ray
	{
	public:
		Vector3	direction;
		Vector3	position;

		Ray()	{}
		Ray(const Vector3&	pos, const Vector3&	dir)
		{
			direction=dir;
			position=pos;
		}

		bool intersect(const BoundingSphere &sphere, float  &distance) const;		// tested
		bool intersect(const AABoundingBox &box, float  &distance) const;			// tested
		bool intersect(const Plane &plane, float &distance) const;					// tested


		bool intersect(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3,const Vector3 &normal,float &distance,bool posSide, bool negSide) const;
	
		bool intersectRect(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3,const Vector3 &p4,Vector3 &result);
		bool intersectGrid(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3,const Vector3 &p4,float cellWidthX, float cellHeightY,Vector2 &result);


	};


	class Plane
	{
	public:
		float d;
		Vector3	normal;

		Plane() {}
		Plane(const Vector3& normal, float d);
		Plane(const Vector3& p0,const Vector3& p1,const Vector3& p2);
		Plane(const Vector3& normal, const Vector3 &p);
		IntersectionType intersect(const BoundingSphere &sphere) const;		// tested
		IntersectionType intersect(const AABoundingBox &box) const;			// tested

		float distance (const Vector3& point) const { return normal.dot(point) + d;}


	};

	class BoundingFrustum
	{	
	public:
		Plane	planes[6];
		Vector3 corners[8];
		Matrix4	viewProjection;

		BoundingFrustum() {};
		BoundingFrustum(const Matrix4& matrix);
		void setMatrix(const Matrix4& matrix);

		
		bool	isVisible(const AABoundingBox& box) const;		// tested
		bool	isVisible(const BoundingSphere& box) const;		// tested
		bool	isVisible(const Vector3& point) const;		// tested


	protected:
		void computeIntersection(const Plane& plane, const Ray& ray,Vector3& result);
		void computeIntersectionLine(const Plane& p1, const Plane& p2, Ray& result);
	};	

	class BoundingSphere
	{
	public:
		Vector3	center;
		float	radius;
		BoundingSphere() {;}
		BoundingSphere(const Vector3& center, float radius);

		bool intersects(const BoundingSphere& sphere);				// tested
		bool intersects(const AABoundingBox& box);	// tested

		void merge(const BoundingSphere& other);		// tested

		static void createMerged(const BoundingSphere& first,const  BoundingSphere& second, BoundingSphere& result); //tested
 
		void transform(const Matrix4 &mat);	// tested

		void createFromPoints(unsigned char	*points, size_t numPoints, size_t stride, size_t offset); // tested ?

	};


	class AABoundingBox
	{
	public:
		Vector3	min;
		Vector3	max;

		AABoundingBox();

		AABoundingBox(const Vector3&	min, const Vector3&	max);

		AABoundingBox(const BoundingSphere& sphere);
        
        AABoundingBox(unsigned char *points, size_t numPoints,size_t stride, size_t offset=0);

		inline void merge(const AABoundingBox& other)
		{
			if(!isNull())
			{
				Vector3::createMin(this->min,other.min,this->min);
				Vector3::createMax(this->max,other.max,this->max);
			}
			else
			{
				this->min=other.min;
				this->max=other.max;
			}
		}

		static void createMerged(const AABoundingBox& first,const  AABoundingBox& second, AABoundingBox& result); //tested


		void corners(Vector3	*corners) const;

		bool intersects(const BoundingSphere& sphere) const;					// tested
 		bool intersects(const AABoundingBox& box) const;						// tested
 
		
		void transform(const Matrix4 &mat);									// tested
		void createFromPoints(unsigned char *points, size_t numPoints,size_t stride, size_t offset=0);

		inline void setNull() { min=Vector3::One; max=Vector3::NegOne;}

		inline bool isNull() const { return min>max;}

		inline Vector3 center() const { return  Vector3((max.x + min.x) * 0.5f,(max.y + min.y) * 0.5f,
										(max.z + min.z) * 0.5f); }

		inline Vector3 extends() const { return Vector3(fabs(max.x -min.x),fabs(max.y -min.y),fabs(max.z -min.z));}

		inline float	boundingRadius() const { return (max-min).length()*0.5f;}
	};

	class  RectangleF
	{
	public:

		RectangleF()	{x1=y1=0.0f;width=height=0.0f;}

		RectangleF( float left, float top, float width, float height )
		{
			x1=left;
			y1=top;
			this->width=width;			
			this->height=height;
		}

        static RectangleF createWithCoordinates(float left, float top, float bottom, float right )
        {
            return RectangleF(left,top,bottom-top,right-left);
        }
        
		inline float	   left()	const { return x1; }
		inline float	   right()	const { return x1+width; }
		inline float	   top()	const { return y1; }
		inline float	   bottom()	const { return y1+height; }


		inline void   setLeft( float v )   { float diff = v - x1; x1 += diff; width -= diff; }
		inline void   setRight( float v ) {   width = v - x1;  }
		inline void   setTop( float v ) { float diff = v - y1; y1 += diff; height -= diff; }
		inline void   setBottom( float v ) { height = v - y1; }


		void   moveBy( float dx, float dy )
		{
			x1+=dx;y1+=dy;
		}	

		void   moveByX(float dx)
		{
			x1+=dx;
		}

		void   moveByY(float dy)
		{
			y1+=dy;
		}
		inline void   setRect( float x, float y, float w, float h )
		{
			x1=x;
			y1=y;
			width=w;
			height=h;
		}

		inline void setPosition(float x, float y)
		{
			x1=x;
			y1=y;
		}
		
		Vector2  pointAt(Origin alignment);

	

		inline void		setCenter(float x, float y ) { x1=x-(width*0.5f); y1=y-(height*0.5f);}

		inline  bool   isValid()	const { return width > 0.0f && height > 0.0f; }	

		inline void   setCoords( float x1, float y1, float x2, float y2 ) 
		{     
			this->x1 = x1;
			this->y1 = y1;
			width = x2 - x1;
			height = y2 - y1;
		}

		void  normalize();
		bool   contains( float x, float y ) const ;
		bool	contains(const RectangleF &r) const;
		bool   intersects( const RectangleF &r ) const;
		RectangleF  intersect( const RectangleF &r ) const;
		RectangleF  unite( const RectangleF &r ) const;
        
        inline Vector2 center() const { return Vector2(x1+width/2.0f,y1+height/2.0f);}

		inline void united(const RectangleF &r )
		{
			*this=unite(r);
		}

		inline friend  bool operator==( const RectangleF &lhs, const RectangleF &rhs )
		{
			return MathUtils::AlmostEqualComplement(lhs.x1, rhs.x1) && MathUtils::AlmostEqualComplement(lhs.y1, rhs.y1)
           &&  MathUtils::AlmostEqualComplement(lhs.width, rhs.width) && MathUtils::AlmostEqualComplement(lhs.height, rhs.height);
		}
		inline friend  bool operator!=( const RectangleF &lhs, const RectangleF &rhs )
		{
			return !MathUtils::AlmostEqualComplement(lhs.x1, rhs.x1) || !MathUtils::AlmostEqualComplement(lhs.y1, rhs.y1)
			|| !MathUtils::AlmostEqualComplement(lhs.width, rhs.width) || !MathUtils::AlmostEqualComplement(lhs.height, rhs.height);
		}


		float x1;
		float y1;
		float width;
		float height;


		static const RectangleF Invalid;

	};


	class  RectangleI
	{

	public:
		RectangleI()	{x1=y1=1;x2=y2=0;}

		RectangleI( int left, int top, int width, int height )
		{
			x1=left;
			x2=(left+width-1);
			y1=top;
			y2=(top+height-1);
		}


        static RectangleI createWithCoordinates(int left, int top, int bottom, int right )
        {
            return RectangleI(left,top,bottom-top,right-left);
        }

        
		inline int	   left()	const { return x1; }
		inline int	   right()	const { return x2; }
		inline int	   top()	const { return y1; }
		inline int	   bottom()	const { return y2; }


		inline void   setLeft( int v ) {  x1=v; }
		inline void   setRight( int v ) {  x2=v; }
		inline void   setTop( int v ) {  y1=v; }
		inline void   setBottom( int v ) {  y2=v; }


		void expand( int dx, int dy)
		{
			x1-=dx;y1-=dy;x2+=dx;y2+=dy;
		}

		RectangleI expanded(int dx, int dy) const
		{
			return RectangleI(x1-dx,y1-dy,width()+(dx*2),height()+(dy*2));
		}

		void   moveBy( int dx, int dy )
		{
			x1+=dx;y1+=dy;x2+=dx;y2+=dy;
		}	

		void   moveByX(int dx)
		{
			x1+=dx;x2+=dx;
		}

		void   moveByY(int dy)
		{
			y1+=dy;y2+=dy;
		}
		inline void   setRect( int x, int y, int w, int h )
		{
			x1=x;
			x2=(x+w-1);
			y1=y;
			y2=(y+h-1);
		}

		inline  bool   isValid()	const { return (x1 <= x2 && y1 <= y2); }	

		inline void   setCoords( int x1, int y1, int x2, int y2 ) 
		{ 
			this->x1=x1;this->y1=y1;this->x2=x2;this->y2=y2;
		}

		inline int	   width()	const {return x2 -x1 + 1; }
		inline int	   height()	const {return y2- y1 + 1; }

		


		inline void   setWidth( int w )  { x2= x1+w-1;} 

		inline void   setHeight( int h ) { y2= y1+h-1;}

		void  normalize();

		inline bool   contains( int x, int y ) const 
		{
			return (x >= x1 && x <= x2 && y >= y1 && y <= y2);
		}

		inline bool	contains(const RectangleI &r) const
		{
			return (r.x1 >= x1 && r.x2 <= x2 && r.y1 >= y1 && r.y2 <= y2);
		}

		bool   intersects( const RectangleI &r ) const;
		RectangleI  intersect( const RectangleI &r ) const;
		RectangleI  unite( const RectangleI &r ) const;

		RectangleI aligned(const RectangleI &r , Origin alignment);
		void align(const RectangleI &r , Origin alignment);

		RectangleI	centered(const RectangleI &r ) const;
		void	center(const RectangleI &r);

        inline void scale(const Vector2 &factor)
        {
            int offsetX=(int)((factor.x*(float)width())*0.5f);
            int offsetY=(int)((factor.y*(float)height())*0.5f);            
            x1-=offsetX;
            x2+=offsetX;            
            y1-=offsetY;
            y2+=offsetY;            
        }
        
		inline void united(const RectangleI &r )
		{
			using rde::min;
			using rde::max;
			x1=rde::min<int>( x1, r.x1 );
			x2=rde::max<int>( x2, r.x2 );
			y1=rde::min<int>( y1, r.y1 );
			y2=rde::max<int>( y2, r.y2 );
		}

		inline friend  bool operator==( const RectangleI &lhs, const RectangleI &rhs )
		{
			return (lhs.x1==rhs.x1 && lhs.x2==rhs.x2 && lhs.y1==rhs.y1 && lhs.y2==rhs.y2);
		}
		inline friend  bool operator!=( const RectangleI &lhs, const RectangleI &rhs )
		{
			return (lhs.x1!=rhs.x1 || lhs.x2!=rhs.x2 || lhs.y1!=rhs.y1 ||lhs.y2!=rhs.y2);
		}


		int x1;
		int y1;
		int x2;
		int y2;


		static const RectangleI Invalid;
	};


	class RectangleUv
	{
	public:
		

		RectangleUv() {}
		RectangleUv(float u1,float v1, float u2, float v2):u1(u1),v1(v1),u2(u2),v2(v2){}
		

		inline void clip(const int twidth, const int theight,const RectangleI &orig, const RectangleI &clipped) 
		{
			float dX=((float)(clipped.x1-orig.x1)/(float)twidth);
			float dY=((float)(clipped.y1-orig.y1)/(float)theight);
			float dX2=((float)(clipped.x2-orig.x2)/(float)twidth);
			float dY2=((float)(clipped.y2-orig.y2)/(float)theight);
			u1=u1+dX;
			v1=v1-dY2;
			u2=u2+dX2;
			v2=v2-dY;

		}
        
        
		float u1;
		float v1;
		float u2;
		float v2;

	};





}

#endif