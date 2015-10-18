

#include "GeometryUtils.h"
#include "MathUtils.h"

using namespace MiniEngine;

const RectangleI RectangleI::Invalid;

bool Ray::intersectGrid(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3,const Vector3 &p4,float cellWidthX, float cellHeightY,Vector2 &result)
{
	Vector3 point;

	if(intersectRect(p1,p2,p3,p4,point))
	{ 
		float tX;
		float tY;
		point.projectOnLine(p2,p1,tX);
		point.projectOnLine(p4,p1,tY);

		result.x=((p2-p1).length()/cellWidthX)*(1-tX);
		result.y=((p4-p1).length()/cellHeightY)*(1-tY);

		return true;
	}

	return false;
}


bool Ray::intersectRect(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3,const Vector3 &p4,Vector3 &result)
{

	Vector3 p=position;
	Vector3 q=p+1000*direction;

	Vector3 pq = q - p;
	Vector3 pa = p1 - p;
	Vector3 pb = p2 - p;
	Vector3 pc = p3 - p;

	Vector3 m = pc.cross(pq);
	float v = pa.dot(m); 
	if (v >= 0.0f) 
	{
		float u = -pb.dot( m); 
		
		if (u < 0.0f) 
			return false;


		float w=(pq.cross(pb)).dot(pa);
		if (w < 0.0f) 
			return false;
		float denom = 1.0f / (u + v + w);
		u *= denom;
		v *= denom;
		w *= denom; 
		result = u*p1 + v*p2 + w*p3;
	} else {
		Vector3 pd = p4 - p;
		float u = pd.dot( m); 
		if (u < 0.0f) 
			return false;
		float w=(pq.cross(pa)).dot(pd);
		if (w < 0.0f) 
			return false;
		v = -v;
		float denom = 1.0f / (u + v + w);
		u *= denom;
		v *= denom;
		w *= denom; 
		result = u*p1 + v*p4 + w*p3;
	}
	return true;
}

bool Ray::intersect(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3,const Vector3 &normal,float &distance,bool posSide, bool negSide) const
{
	distance=0;
	float t;
	{
		float denom = normal.dot(direction);

		if (denom > +  FLT_EPSILON)
		{
			if (!negSide)
				return false;
		}
		else if (denom < -  FLT_EPSILON)
		{
			if (!posSide)
				return false;
		}
		else
		{
			return false;
		}

		t = normal.dot(p1 - position) / denom;

		if (t < 0)
		{
			return false;
		}
	}
	size_t i0, i1;
	{
		float n0 = MathUtils::Abs(normal.x);
		float n1 = MathUtils::Abs(normal.y);
		float n2 = MathUtils::Abs(normal.z);

		i0 = 1; i1 = 2;
		if (n1 > n2)
		{
			if (n1 > n0) i0 = 0;
		}
		else
		{
			if (n2 > n0) i1 = 0;
		}
	}

	{
		float u1 = p2[i0] - p1[i0];
		float v1 = p2[i1] - p1[i1];
		float u2 = p3[i0] - p1[i0];
		float v2 = p3[i1] - p1[i1];
		float u0 = t * this->direction[i0] + this->position[i0] - p1[i0];
		float v0 = t * this->direction[i1] + this->position[i1] - p1[i1];

		float alpha = u0 * v2 - u2 * v0;
		float beta  = u1 * v0 - u0 * v1;
		float area  = u1 * v2 - u2 * v1;

		const float EPSILON = 1e-3f;

		float tolerance = - EPSILON * area;

		if (area > 0)
		{
			if (alpha < tolerance || beta < tolerance || alpha+beta > area-tolerance)
				return false;
		}
		else
		{
			if (alpha > tolerance || beta > tolerance || alpha+beta < area-tolerance)
				return false;
		}
	}

	distance=t;

	return true; 
}

bool Ray::intersect(const BoundingSphere &sphere, float& distance) const
{
    float num5 = sphere.center.x - this->position.x;
    float num4 = sphere.center.y - this->position.y;
    float num3 = sphere.center.z - this->position.z;
    float num7 = num5 * num5 + num4 * num4 + num3 * num3;
    float num2 = sphere.radius*sphere.radius;
    if (num7 <= num2)
    {
		distance=0;
        return true;
    }
    float num = (num5 *this->direction.x) + (num4 * this->direction.y) + (num3* this->direction.z);
    if (num < 0)
    {
        return false;
    }
    float num6 = num7 - (num * num);
    if (num6 > num2)
    {
        return false;
    }
	float num8 = sqrtf(((num2 - num6)));
    distance=num - num8;
	return true;
}
bool Ray::intersect(const AABoundingBox &box, float& distance) const
{
	distance = 0;
	float maxValue = FLT_MAX;
	if (MathUtils::Abs(this->direction.x) < 1e-06f)
	{
		if ((this->position.x < box.min.x) || (this->position.x > box.max.x))
		{
			return false;
		}
	}			
	else
	{
		float num11 = 1.0f/this->direction.x;
		float num8 = ((box.min.x - this->position.x)* num11);
		float num7 = ((box.max.x - this->position.x)* num11);
		if (num8 > num7)
		{
			float num14 = num8;
			num8 = num7;
			num7 = num14;
		}
		using rde::min;
		distance = rde::max<float>(num8, distance);
		maxValue = rde::min<float>(num7, maxValue);
		if (distance > maxValue)
		{
			return false;
		}
	}

	if (MathUtils::Abs(this->direction.y) < 1e-06f)
	{
		if ((this->position.y < box.min.y) || (this->position.y > box.max.y))
		{
			return false;
		}
	}
	else
	{
		float num10 =  (1.0f /this->direction.y);
		float num6 = ((box.min.y - this->position.y) * num10);
		float num5 = ((box.max.y - this->position.y) * num10);
		if (num6 > num5)
		{
			float num13 = num6;
			num6 = num5;
			num5 = num13;
		}
		distance = rde::max<float>(num6, distance);
		maxValue = rde::min<float>(num5, maxValue);
		if (distance > maxValue)
		{
			return false;
		}
	}

	if (MathUtils::Abs(this->direction.z) < 1e-06f)
	{
		if ((this->position.z < box.min.z) || (this->position.z > box.max.z))
		{
			return false;
		}
	}
	else
	{
		float num9 = (1.0f /this->direction.z);
		float num4 = ((box.min.z - this->position.z) * num9);
		float num3 = ((box.max.z - this->position.z) * num9);
		if (num4 > num3)
		{
			float num12 = num4;
			num4 = num3;
			num3 = num12;
		}
		distance = rde::max<float>(num4, distance);
		maxValue = rde::min<float>(num3, maxValue);
		if (distance > maxValue)
		{
			return false;
		}
	}

	return true;
}
bool Ray::intersect(const Plane &plane, float& distance) const
{

	float denom = plane.normal.dot(this->direction);
	if (MathUtils::Abs(denom) < 1e-06f)
	{
		return false;
	}
	else
	{
		float nom = plane.normal.dot(this->position) + plane.d;
		float t = -(nom/denom);
		distance=t;
		return (t>=0);
	}
}



Plane::Plane(const Vector3& normal, float d)
{
	this->normal=normal;
	this->d=d;
}

Plane::Plane(const Vector3& p0,const Vector3& p1,const Vector3& p2)
{
	Vector3 kEdge1 = p1 - p0;
	Vector3 kEdge2 = p2 - p0;
	normal = kEdge1.cross(kEdge2);
	normal.normalize();
	d = -normal.dot(p0);

}
Plane::Plane(const Vector3& normal, const Vector3 &p)
{
	this->normal=normal;
	this->d=normal.dot(p);	
}


IntersectionType Plane::intersect(const AABoundingBox &box) const
{
	//return box.intersects(*this);

	Vector3 vector;
	Vector3 vector2;
	vector2.x = (normal.x >= 0) ? box.min.z : box.max.x;
	vector2.y = (normal.y >= 0) ? box.min.y : box.max.y;
	vector2.z = (normal.z >= 0) ? box.min.z : box.max.z;
	vector.x = (normal.x >= 0) ? box.max.x : box.min.x;
	vector.y = (normal.y >= 0) ? box.max.y : box.min.y;
	vector.z = (normal.z >= 0) ? box.max.z : box.min.z;
	float num = ((normal.x * vector2.x) + (normal.y * vector2.y)) + (normal.z * vector2.z);
	if ((num + this->d) > 0)
	{
		return Front;
	}
	num = ((normal.x * vector.x) + (normal.y * vector.y)) + (normal.z * vector.z);
	if ((num + this->d) < 0)
	{
		return Back;
	}
	return Intersects;

}

IntersectionType Plane::intersect(const BoundingSphere &sphere) const
{
    float num2 = (sphere.center.x * this->normal.x) + (sphere.center.y * this->normal.y) + (sphere.center.z *this->normal.z);
    float num = num2 + this->d;
    if (num > sphere.radius)
    {
		return Front;
    }
    if (num < -sphere.radius)
    {
        return Back;
    }
    return Intersects;
}


BoundingFrustum::BoundingFrustum(const Matrix4& matrix)
{
	setMatrix(matrix);
}

void BoundingFrustum::setMatrix(const Matrix4& matrix)
{
    this->viewProjection = matrix;
    planes[2].normal.x = -matrix.f[3] - matrix.f[0];

    planes[2].normal.y = -matrix.f[7] - matrix.f[4];
    planes[2].normal.z = -matrix.f[11] - matrix.f[8];
    planes[2].d = -matrix.f[15] - matrix.f[12];
    planes[3].normal.x = -matrix.f[3] + matrix.f[0];
    planes[3].normal.y = -matrix.f[7] + matrix.f[4];
    planes[3].normal.z = -matrix.f[11] + matrix.f[8];
    planes[3].d = -matrix.f[15] + matrix.f[12];
    planes[4].normal.x = -matrix.f[3] + matrix.f[1];
    planes[4].normal.y = -matrix.f[7] + matrix.f[5];
    planes[4].normal.z = -matrix.f[11] + matrix.f[9];
    planes[4].d = -matrix.f[15] + matrix.f[13];
    planes[5].normal.x = -matrix.f[3] - matrix.f[1];
    planes[5].normal.y = -matrix.f[7] - matrix.f[5];
    planes[5].normal.z = -matrix.f[11] - matrix.f[9];
    planes[5].d = -matrix.f[15] - matrix.f[13];
    planes[0].normal.x = -matrix.f[2];
    planes[0].normal.y = -matrix.f[6];
    planes[0].normal.z = -matrix.f[10];
    planes[0].d = -matrix.f[14];
    planes[1].normal.x = -matrix.f[3] + matrix.f[2];
    planes[1].normal.y = -matrix.f[7] + matrix.f[6];
    planes[1].normal.z = -matrix.f[11] + matrix.f[10];
    planes[1].d = -matrix.f[15] + matrix.f[14];

    for (int i = 0; i < 6; i++)
    {
        float num2 = planes[i].normal.length();
        planes[i].normal = (planes[i].normal / num2);
        planes[i].d /= num2;
    }
	Ray ray;
    computeIntersectionLine(planes[0],planes[2],ray);
    computeIntersection(planes[4], ray,corners[0]);
    computeIntersection(planes[5], ray,corners[3]);
    computeIntersectionLine(planes[3],planes[0],ray);
    computeIntersection(planes[4],ray, corners[1]);
    computeIntersection(planes[5],ray, corners[2]);
    computeIntersectionLine(planes[2],planes[1],ray);
    computeIntersection(planes[4],ray,corners[4]);
    computeIntersection(planes[5],ray,corners[7]);
    computeIntersectionLine(planes[1],planes[3],ray);
    computeIntersection(planes[4], ray,corners[5]);
    computeIntersection(planes[5], ray,corners[6]);
}

bool BoundingFrustum::isVisible(const Vector3& point) const
{
	for(int i=0;i<6;i++)
	{
		float num2= (planes[i].normal.x*point.x) + (planes[i].normal.y* point.y) + (planes[i].normal.z* point.z) + planes[i].d;
        if (num2 > 1e-06f)
        {
			return false;
        }
	}
    return true;
}

bool	BoundingFrustum::isVisible(const AABoundingBox& box) const
{
	float halfPoint=0.5f;

	Vector3 center(((box.max.x + box.min.x) * halfPoint),
					  ((box.max.y + box.min.y)* halfPoint),
					  ((box.max.z + box.min.z)* halfPoint));
	
	Vector3 halfSize=(box.max - box.min);
	halfSize*=halfPoint;

	for (int i = 0; i < 6; ++i)
	{
        float dist = planes[i].normal.dot(center)+ planes[i].d;
        float maxAbsDist = planes[i].normal.dotAbs(halfSize);
        if (dist > maxAbsDist)
            return false;
	}
	return true;
}

bool	BoundingFrustum::isVisible(const BoundingSphere& sphere) const
{
	for (int i = 0; i < 6; ++i)
	{
		if ((planes[i].normal.dot(sphere.center)+ planes[i].d) > sphere.radius)
		{
			return false;
		}
	}
	return true;
}


void  BoundingFrustum::computeIntersection(const Plane& plane, const Ray& ray, Vector3& result)
{

	float num=  (-plane.d - plane.normal.dot(ray.position)/ plane.normal.dot(ray.direction));
	result=ray.position+(ray.direction*num);
}

 

void  BoundingFrustum::computeIntersectionLine(const Plane& p1, const Plane& p2, Ray& result)
{
    result.direction = p1.normal.cross(p2.normal);
	const Vector3 tmp1=(-p1.d * p2.normal)+(p2.d * p1.normal);
	result.position=tmp1.cross(result.direction);
}


BoundingSphere::BoundingSphere(const Vector3& center, float radius)
{
	this->center=center;
	this->radius=radius;
}


bool BoundingSphere::intersects(const AABoundingBox& box)
{
	return box.intersects(*this);
}


bool BoundingSphere::intersects(const BoundingSphere& sphere)
{
	float num3=center.distanceSqr(sphere.center);
    if ((( (this->radius/ this->radius) + ((2.0f/ this->radius)/  sphere.radius)) + (sphere.radius/ sphere.radius)) <= num3)
    {
        return false;
    }
    return true;
}


// tested
void BoundingSphere::merge(const BoundingSphere& other)
{
	Vector3 vector2=other.center-center;
	float num = vector2.length();
	float num2 = other.radius;
	if ((radius + num2) >= num)
	{
		if ((radius - num2) >= num)
		{
			return;
		}
		if ((num2 - radius) >= num)
		{
			*this=other;
			return;
		}
	}
	Vector3 vector(vector2);
	vector*= (1.0f/num);
	float num5 = rde::min<float>(-radius, num - num2);
	float num4 = ((rde::max<float>(radius, num + num2) - num5)/0.5f);
	vector*=num4+num5;
	center = center + vector;
    radius = num4;
}

//tested
void BoundingSphere::createMerged(const BoundingSphere& first,const  BoundingSphere& second, BoundingSphere& result)
{

	Vector3 vector2=second.center-first.center;
	float num = vector2.length();
	float radius = first.radius;
	float num2 = second.radius;
	if ((radius + num2) >= num)
	{
		if ((radius - num2) >= num)
		{
			result=first;
			return;
		}
		if ((num2 - radius) >= num)
		{
			result=second;
			return;
		}
	}
	Vector3 vector(vector2);
	vector*= (1.0f/num);
	float num5 = rde::min<float>(-radius, num - num2);
	float num4 = ((rde::max<float>(radius, num + num2) - num5)/0.5f);
	vector*=num4+num5;
	result.center = first.center + vector;
    result.radius = num4;

}

void BoundingSphere::transform(const Matrix4 &mat)
{	
	center=mat*center;
    float num = ((mat[0][0] + mat[1][1] + mat[2][2])/3.0f);
    this->radius *= num;
}

void BoundingSphere::createFromPoints(unsigned char	*points, size_t numPoints, size_t stride, size_t offset)
{
	float num;
	float num2;
	Vector3 vector2;
	float num4;
	float num5;
	Vector3 vector5;
	Vector3 vector6;
	Vector3 vector7;
	Vector3 vector8;
	Vector3 vector9;

	unsigned char* tmpPoints=points;
	tmpPoints+=offset;

	Vector3 vector4=vector5= vector6 = vector7 = vector8 = vector9 = Vector3(0,0,0);

	for (size_t i=0; i<numPoints; i++)
	{		
		Vector3 *vector=(Vector3*)tmpPoints;
		if (vector->x < vector4.x)
		{
			vector4 = *vector;
		}
		if (vector->x > vector5.x)
		{
			vector5 = *vector;
		}
		if (vector->y < vector6.y)
		{
			vector6 = *vector;
		}
		if (vector->y > vector7.y)
		{
			vector7 = *vector;
		}
		if (vector->z < vector8.z)
		{
			vector8 = *vector;
		}
		if (vector->z > vector9.z)
		{
			vector9 = *vector;
		}
		tmpPoints+=stride;
	}
	num5=vector5.distance(vector4);
	num4=vector7.distance(vector6);
	num2=vector9.distance(vector8);

	float half=0.5f;

	if (num5 > num4)
	{
		if (num5 > num2)
		{
			vector2=Vector3::lerp(vector5,vector4, half);
			num = (num5*half);
		}
		else
		{
			vector2=Vector3::lerp(vector9, vector8, half);
			num = (num2 *half);
		}
	}
	else if (num4 > num2)
	{
		vector2=Vector3::lerp(vector7,vector6,half);
		num = (num4 *half);
	}
	else
	{
		vector2=Vector3::lerp(vector9, vector8,half);
		num = (num2 *half);
	}

	tmpPoints=points;
	tmpPoints+=offset;
	for (size_t i=0; i<numPoints; i++)
	{		
		Vector3 *vector10=(Vector3*)tmpPoints;

		Vector3 vector3;
		vector3.x = vector10->x - vector2.x;
		vector3.y = vector10->y - vector2.y;
		vector3.z = vector10->z - vector2.z;
		float num3 = vector3.length();
		if (num3 > num)
		{
			num = ((num + num3)* half);
			vector2 += (1.0f - (num/num3))*vector3;
		}
		tmpPoints+=stride;
	}
	this->center=vector2;
	this->radius=num;

}


AABoundingBox::AABoundingBox()
{
	setNull();
}

AABoundingBox::AABoundingBox(const Vector3&	min, const Vector3&	max)
{
	this->min=min;
	this->max=max;
}

AABoundingBox::AABoundingBox(const BoundingSphere& sphere)
{
	this->min.x = sphere.center.x - sphere.radius;
	this->min.y = sphere.center.y - sphere.radius;
	this->min.z = sphere.center.z - sphere.radius;
	this->max.x = sphere.center.x + sphere.radius;
	this->max.y = sphere.center.y + sphere.radius;
	this->max.z = sphere.center.z + sphere.radius;
}

AABoundingBox::AABoundingBox(unsigned char *points, size_t numPoints,size_t stride, size_t offset)
{
    createFromPoints(points,numPoints,stride,offset);
}


void AABoundingBox::createFromPoints(unsigned char *points, size_t numPoints,size_t stride, size_t offset)
{
	this->min.x=FLT_MAX;
	this->min.y=FLT_MAX;
	this->min.z=FLT_MAX;

	this->max.x=-FLT_MAX;
	this->max.y=-FLT_MAX;
	this->max.z=-FLT_MAX;


	unsigned char* tmpPoints=points;
	tmpPoints+=offset;

	for (int i=1; i<numPoints; i++)
	{		
		Vector3 *tmp=(Vector3*)tmpPoints;
		Vector3::createMin(this->min, *tmp,this->min);
		Vector3::createMax(this->max, *tmp,this->max);
		tmpPoints+=stride;
	}
}






void AABoundingBox::createMerged(const AABoundingBox& first,const  AABoundingBox& second, AABoundingBox& result)
{
	Vector3::createMin(first.min,second.min,result.min);
	Vector3::createMax(first.max,second.max,result.max);
}


void AABoundingBox::corners(Vector3	*corners) const
{
	corners[0].x = min.x;
	corners[0].y = max.y;
	corners[0].z = max.z;
	corners[1].x = max.x;
	corners[1].y = max.y;
	corners[1].z = max.z;
	corners[2].x = max.x;
	corners[2].y = min.y;
	corners[2].z = max.z;
	corners[3].x = min.x;
	corners[3].y = min.y;
	corners[3].z = max.z;
	corners[4].x = min.x;
	corners[4].y = max.y;
	corners[4].z = min.z;
	corners[5].x = max.x;
	corners[5].y = max.y;
	corners[5].z = min.z;
	corners[6].x = max.x;
	corners[6].y = min.y;
	corners[6].z = min.z;
	corners[7].x = min.x;
	corners[7].y = min.y;
	corners[7].z = min.z;
}



bool AABoundingBox::intersects(const BoundingSphere& sphere) const
{			
	Vector3 vector;
	Vector3::clamp(sphere.center,this->min, this->max, vector);
	sphere.center.distanceSqr(vector);
	return (sphere.center.distanceSqr(vector) <= sphere.radius*sphere.radius);

}

bool AABoundingBox::intersects(const AABoundingBox& box) const
{
	if ((this->max.x < box.min.x) || (this->min.x > box.max.x)) { return false;}
	if ((this->max.y < box.min.y) || (this->min.y > box.max.y)) { return false;}
	return ((this->max.z >= box.min.z) && (this->min.z <= box.max.z));
}


//this thing would benefit from NEON optimization ( or even VFP)

void AABoundingBox::transform(const Matrix4 &mat)
{
	Vector3 oldMin, oldMax, currentCorner;

	oldMin = this->min;
	oldMax = this->max;	

	currentCorner = oldMin;
	Vector3 tmp = mat * currentCorner;
	this->min=tmp;
	this->max=tmp;

	// min,min,max
	currentCorner.z = oldMax.z;
	tmp = mat * currentCorner;
	Vector3::createMax(this->max,tmp,this->max);
	Vector3::createMin(this->min,tmp,this->min);

	// min max max
	currentCorner.y = oldMax.y;
	tmp = mat * currentCorner;
	Vector3::createMax(this->max,tmp,this->max);Vector3::createMin(this->min,tmp,this->min);

	// min max min
	currentCorner.z = oldMin.z;
	tmp = mat * currentCorner;
	Vector3::createMax(this->max,tmp,this->max);Vector3::createMin(this->min,tmp,this->min);

	// max max min
	currentCorner.x = oldMax.x;
	tmp = mat * currentCorner;
	Vector3::createMax(this->max,tmp,this->max);Vector3::createMin(this->min,tmp,this->min);

	// max max max
	currentCorner.z = oldMax.z;
	tmp = mat * currentCorner;
	Vector3::createMax(this->max,tmp,this->max);Vector3::createMin(this->min,tmp,this->min);

	// max min max
	currentCorner.y = oldMin.y;
	tmp = mat * currentCorner;
	Vector3::createMax(this->max,tmp,this->max);Vector3::createMin(this->min,tmp,this->min);

	// max min min
	currentCorner.z = oldMin.z;
	tmp = mat * currentCorner;
	Vector3::createMax(this->max,tmp,this->max);Vector3::createMin(this->min,tmp,this->min);

}



void  RectangleI::normalize()
{   
	int tmp;

	if ( y2 < y1 ) 
	{
		tmp=y1;
		y1 = y2;
		y2 = tmp;
    } 
	if ( x2 < x1 ) 
	{	
		tmp=x1;
		x1 = x2;
		x2 = tmp;
    } 

}


RectangleI  RectangleI::unite( const RectangleI &b ) const
{
	RectangleI nr;
	
	using rde::min;
	using rde::max;
	nr.x1=rde::min<int>( x1, b.x1 );
	nr.x2=rde::max<int>( x2, b.x2 );
	nr.y1=rde::min<int>( y1, b.y1 );
	nr.y2=rde::max<int>( y2, b.y2 );

	return nr;
}
RectangleI  RectangleI::intersect( const RectangleI &b ) const
{
    RectangleI nr;
	using rde::min;
	using rde::max;
    nr.x1 = rde::max<int>(x1, b.x1 );
    nr.x2 = rde::min<int>(x2, b.x2 );
    nr.y1 = rde::max<int>(y1, b.y1 );
    nr.y2 = rde::min<int>(y2, b.y2 );
    return nr;
}
bool   RectangleI::intersects( const RectangleI &b ) const
{
	using rde::min;
	using rde::max;
	return (rde::max<int>( y1, b.y1 ) <= rde::min<int>( y2, b.y2 )  &&
	      rde::max<int>( x1, b.x1 ) <= rde::min<int>( x2, b.x2 ));
}

RectangleI RectangleI::aligned(const RectangleI &other , Origin alignment)
{
	switch(alignment)
	{
	case Center: // Fixed
		return RectangleI(other.x1+((other.width()-width())/2),other.y1+((other.height()-height())/2),width(),height()); 
	case Left: // fixed
		return RectangleI(other.x1,other.y1+((other.height()-height())/2),width(),height()); 
	case Right: // fixed
		return RectangleI(x1+other.x2-x2,other.y1+((other.height()-height())/2),width(),height());
	case TopLeft: // fixed
		return RectangleI(other.x1,other.y1,width(),height());
	case TopRight:// fixed
        return RectangleI(x1+other.x2-x2,other.y1,width(),height());   
	//	return RectangleI(x1+other.width()-width(),other.y1,width(),height());
	case Top:  // fixed 
        return RectangleI(other.x1+((other.width()-width())/2),other.y1,width(),height());  
	case Bottom:
		return RectangleI(other.x1+((other.width()-width())/2),other.y1+other.height()-height(),width(),height());
	case BottomLeft:
		return RectangleI(other.x1,other.y1+other.height()-height(),width(),height());
	case BottomRight:
		return RectangleI(x1+other.x2-x2,other.y1+other.height()-height(),width(),height());
	}
	return RectangleI(x1,y1,width(),height());	
}
void RectangleI::align(const RectangleI &other , Origin alignment)
{
    switch(alignment)
    {
        case Center:
            x1=other.x1+((other.width()-width())/2); y1=other.y1+((other.height()-height())/2);
            break;
        case Left:
            x1=other.x1; y1=other.y1+((other.height()-height())/2);
            break;
        case Right:
            x1=x1+other.x2-x2; y1=other.y1+((other.height()-height())/2);
            break;
        case TopLeft:
            x1=other.x1; y1=other.y1;
            break;
        case TopRight:
            x1=x1+other.x2-x2; y1=other.y1;
            break;
        case Top:
            x1=other.x1+((other.width()-width())/2); y1=other.y1;
            break;
        case Bottom:
            x1=other.x1+((other.width()-width())/2); y1=other.y1+other.height()-height();
            break;
        case BottomLeft:
            x1=other.x1; y1=other.y1+other.height()-height();
            break;
        case BottomRight:
            x1=x1+other.x2-x2; y1=other.y1+other.height()-height();
            break;
    }

}

RectangleI	RectangleI::centered(const RectangleI &other ) const
{
	return RectangleI(x1+((width()-other.width())/2),y1+((height()-other.height())/2),other.width(),other.height());
}

void	RectangleI::center(const RectangleI &other)
{
	moveBy((width()-other.width())/2,(height()-other.height())/2);
}

Vector2  RectangleF::pointAt(Origin align)
{
	switch(align)
	{
	case Center:
		return Vector2(width*0.5f+x1,height*0.5f+y1);
	case Left:
		return Vector2(x1,height*0.5f+y1);
	case Right:
		return Vector2(width+x1,height*0.5f+y1);
	case TopLeft:
		return Vector2(x1,y1);
	case TopRight:
		return Vector2(x1+width,y1);
	case Top:
		return Vector2(width*0.5f+x1,y1);
	case Bottom:
		return Vector2(width*0.5f+x1,height+y1);
	case BottomLeft:
		return Vector2(x1,y1+height);
	case BottomRight:
		return Vector2(x1+width,y1+height);
	}
	return Vector2::Zero;
}


bool RectangleF::contains(float ax, float ay) const
{
    float l = x1;
    float r = x1;
    if (width < 0)
        l += width;
    else
        r += width;
    if (l == r) 
        return false;

    if (ax < l || ax > r)
        return false;

    float t = y1;
    float b = y1;
    if (height < 0)
        t += height;
    else
        b += height;
    if (t == b) 
        return false;

    if (ay < t || ay > b)
        return false;

    return true;
}

bool RectangleF::contains(const RectangleF &r) const
{
    float l1 = x1;
    float r1 = x1;
    if (width < 0)
        l1 += width;
    else
        r1 += width;
    if (l1 == r1) 
        return false;

    float l2 = r.x1;
    float r2 = r.x1;
    if (r.width < 0)
        l2 += r.width;
    else
        r2 += r.width;
    if (l2 == r2) 
        return false;

    if (l2 < l1 || r2 > r1)
        return false;

    float t1 = y1;
    float b1 = y1;
    if (height < 0)
        t1 += height;
    else
        b1 += height;
    if (t1 == b1) 
        return false;

    float t2 = r.y1;
    float b2 = r.y1;
    if (r.height < 0)
        t2 += r.height;
    else
        b2 += r.height;
    if (t2 == b2) 
        return false;

    if (t2 < t1 || b2 > b1)
        return false;

    return true;
}

bool   RectangleF::intersects( const RectangleF &b ) const
{
    float l1 = x1;
    float r1 = x1;
    if (width < 0)
        l1 += width;
    else
        r1 += width;
    if (l1 == r1) 
        return false;

    float l2 = b.x1;
    float r2 = b.x1;
    if (b.width < 0)
        l2 += b.width;
    else
        r2 += b.width;
    if (l2 == r2) 
        return false;

    if (l1 >= r2 || l2 >= r1)
        return false;

    float t1 = y1;
    float b1 = y1;
    if (height < 0)
        t1 += height;
    else
        b1 += height;
    if (t1 == b1) 
        return false;

    float t2 = b.y1;
    float b2 = b.y1;
    if (b.height < 0)
        t2 += b.height;
    else
        b2 += b.height;
    if (t2 == b2) 
        return false;

    if (t1 >= b2 || t2 >= b1)
        return false;

    return true;
}

RectangleF  RectangleF::intersect( const RectangleF &r ) const
{
	float l1 = x1;
    float r1 = x1;
    if (width < 0)
        l1 += width;
    else
        r1 += width;
    if (l1 == r1) 
        return RectangleF();

    float l2 = r.x1;
    float r2 = r.x1;
    if (r.width < 0)
        l2 += r.width;
    else
        r2 += r.width;
    if (l2 == r2) 
        return RectangleF();

    if (l1 >= r2 || l2 >= r1)
        return RectangleF();

    float t1 = y1;
    float b1 = y1;
    if (height < 0)
        t1 += height;
    else
        b1 += height;
    if (t1 == b1) 
        return RectangleF();

    float t2 = r.y1;
    float b2 = r.y1;
    if (r.height < 0)
        t2 += r.height;
    else
        b2 += r.height;
    if (t2 == b2) 
        return RectangleF();

    if (t1 >= b2 || t2 >= b1)
        return RectangleF();

	using rde::min;
	using rde::max;

    RectangleF tmp;
    tmp.x1 = rde::max<float>(l1, l2);
    tmp.y1 = rde::max<float>(t1, t2);
    tmp.width = rde::min<float>(r1, r2) - tmp.x1;
    tmp.height = rde::min<float>(b1, b2) - tmp.y1;
    return tmp;
}

RectangleF RectangleF::unite(const RectangleF &r) const
{
    float l1 = x1;
    float r1 = x1;
    if (width < 0)
        l1 += width;
    else
        r1 += width;
    if (l1 == r1) 
        return r;

    float l2 = r.x1;
    float r2 = r.x1;
    if (r.width < 0)
        l2 += r.width;
    else
        r2 += r.width;
    if (l2 == r2) 
        return *this;

    float t1 = y1;
    float b1 = y1;
    if (height < 0)
        t1 += height;
    else
        b1 += height;
    if (t1 == b1) 
        return r;

    float t2 = r.y1;
    float b2 = r.y1;
    if (r.height < 0)
        t2 += r.height;
    else
        b2 += r.height;
    if (t2 == b2) 
        return *this;

	using rde::min;
	using rde::max;

    RectangleF tmp;
    tmp.x1 = rde::min<float>(l1, l2);
    tmp.y1 = rde::min<float>(t1, t2);
    tmp.width = rde::max<float>(r1, r2) - tmp.x1;
    tmp.height = rde::max<float>(b1, b2) - tmp.y1;
	return tmp;
}