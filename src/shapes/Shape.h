//
// Created by abacate on 23/10/2025.
//

#ifndef CG_SHAPE_H
#define CG_SHAPE_H

#include <QPainter>

#include "../matrix/Matrix3x3.h"


struct Color
{
    int r, g, b;
};

class Shape
{
public:
    virtual ~Shape() = default;
    void draw(QPainter &painter, const Matrix3x3 viewportTransform, Window *window) const
    {
        clipAndDraw(painter,viewportTransform,window);
    };



    virtual void translate(double tx, double ty) {}
    virtual void scale(double sx, double sy) {}
    virtual void rotate(double angle, const Vector2D& center) {}


    void setColor(const Color color)
    {
        this->color = color;
    }

protected:
    virtual void clipAndDraw(QPainter &painter, Matrix3x3 viewportTransform, Window *window) const = 0;
    Color color{};
};


#endif //CG_SHAPE_H