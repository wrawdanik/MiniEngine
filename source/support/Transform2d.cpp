#include "Transform2d.h"

using namespace MiniEngine;


void Transform2d::transformBy(const Transform2d &transform)
{
    orientation=transform.orientation.rotatedWithComplexNumber(orientation);
    scale=transform.scale*scale;
    position=(transform.scale * position).rotatedWithComplexNumber(transform.orientation);
    position+=transform.position;
}

