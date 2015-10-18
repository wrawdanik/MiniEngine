

#include "Viewport.h"
#include "DataTypes.h"

using namespace MiniEngine;
Viewport::Viewport():mPosition(Vector2::Zero),mSize(Vector2::NegOne)
{
    
}
Viewport::Viewport(const Vector2 &position, const Vector2 &size):mPosition(position),mSize(size)
{
    
}


