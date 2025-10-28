//
// Created by abacate on 23/10/2025.
//

#ifndef CG_SHAPE3D_H
#define CG_SHAPE3D_H
#include <QPainter>

#include "../../2d/shapes/Shape.h"
#include "../../2d/window/Window.h"
#include "../matrix/Matrix4x4.h"


class Shape3D
{
public:
    virtual ~Shape3D() = default;

    void draw(QPainter &painter, const Matrix4x4& finalTransform) const
    {
        drawTransformed(painter, finalTransform);
    }

    virtual void translate(double tx, double ty, double tz){}
    virtual void scale(double sx, double sy, double sz){}
    virtual void rotateX(double angle, const Vector3D& center){}
    virtual void rotateY(double angle, const Vector3D& center){}
    virtual void rotateZ(double angle, const Vector3D& center){}

    void setColor(const Color color)
    {
        this->color = color;
    }

protected:
    virtual void drawTransformed(QPainter &painter, const Matrix4x4 &finalTransform) const = 0;
    Color color{};

};


#endif //CG_SHAPE3D_H