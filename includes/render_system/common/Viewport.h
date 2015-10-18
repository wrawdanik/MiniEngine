#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "GeometryUtils.h"

namespace MiniEngine
{


    class Viewport
    {
    public:


        Viewport();

        Viewport(const Vector2 &position, const Vector2 &size);

        void set(const Vector2 &position, const Vector2 &size);

        void set(const RectangleI &rect);

        inline Vector2 position() const
        {
            return mPosition;
        }

        Vector2 size() const
        {
            return mSize;
        }

        inline RectangleI rectangleI() const
        {
            return RectangleI((int) mPosition.x, (int) mPosition.y, (int) mSize.x, (int) mSize.y);
        }

        inline RectangleF rectangleF() const
        {
            return RectangleF(mPosition.x, mPosition.y, mSize.x, mSize.y);
        }

        inline float width() const
        {
            return mSize.x;
        }

        inline float height() const
        {
            return mSize.y;
        }

        inline friend bool operator==(const Viewport &lhs, const Viewport &rhs)
        {
            return (lhs.mPosition == rhs.mPosition && lhs.mSize == rhs.mSize);
        }

        inline friend bool operator!=(const Viewport &lhs, const Viewport &rhs)
        {
            return (lhs.mPosition != rhs.mPosition || lhs.mSize != rhs.mSize);
        }

        inline bool isValid()
        {
            return mRect.isValid();
        }

    private:

        RectangleI mRect;
        Vector2 mPosition;
        Vector2 mSize;
    };


    using ViewportArray=rde::vector<Viewport>;
}

#endif