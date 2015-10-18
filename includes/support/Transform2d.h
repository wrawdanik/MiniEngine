//
//  Transform2.h
//  test
//
//  Created by Walter Rawdanik on 9/16/15.
//
//

#ifndef Transform2d_H
#define Transform2d_H

#include "Vector2.h"

namespace MiniEngine
{
    
    class Transform2d
    {
        public :
        
        
        Transform2d()
        {
            reset();
        }
        
        Transform2d(const Vector2 &position)
        {
            this->position=position;
            scale=Vector2::One;
            orientation=Vector2::UnitX;
            
        }
        
        Transform2d(const Vector2 &position,float degree, const Vector2 &scale=Vector2::One)
        {
            this->position=position;
            this->scale=scale;
            orientation.setRotation(degree);
        }
        
        
        
        inline void reset()
        {
            position=Vector2::Zero;
            scale=Vector2::One;
            orientation=Vector2::UnitX;
        }
        
        
        inline void setOrientation(float degree)
        {
            orientation.setRotation(degree);
        }
        
        float orientationRadians() const
        {
            return atan2f(orientation.y,orientation.x);
        }
        
        void transformBy(const Transform2d &transform);
        
        
        Vector2 position;
        Vector2 scale;
        Vector2 orientation;
    };
}
#endif
