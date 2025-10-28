//
// Created by abacate on 28/10/2025.
//

#ifndef CG_LINE3D_H
#define CG_LINE3D_H
#include "Point3D.h"
#include "Shape3D.h"


struct clippedPoints
{
    Point3D p1;
    Point3D p2;
};

class Line3D: public Shape3D
{   friend class Point3D;

public:
    Line3D(const Point3D& point1, const Point3D& point2, Color color): p1(point1), p2(point2)
    {
        this->color = color;
    };

    Vector3D calculateMiddle() const
    {
        return {
            (this->p1.vector.x() + this->p2.vector.x())/2,
            (this->p1.vector.y() + this->p2.vector.y())/2,
            (this->p1.vector.z() + this->p2.vector.z())/2
        };
    }

    void translate(const double tx, const double ty, const double tz) override
    {
        this->p1.translate(tx, ty, tz);
        this->p2.translate(tx, ty, tz);
    }

    void scale(const double sx,const double sy, const double sz) override
    {
        this->p1.scale(sx, sy, sz);
        this->p2.scale(sx, sy, sz);
    }

    // void rotate(const double angle, const Vector2D& center) override
    // {
    //     this->p1.rotate(angle, center);
    //     this->p2.rotate(angle, center);
    // }

protected:

    void drawTransformed(QPainter& painter, const Matrix4x4& finalTransform) const override
    {
        // p1.drawTransformed(painter, finalTransform);
        auto p1 = finalTransform * this->p1.vector;
        auto p2 = finalTransform * this->p2.vector;
        // p2.drawTransformed(painter, finalTransform);
        painter.setPen(QPen(QColor::fromRgb(color.r, color.g, color.b), 2));
        painter.drawLine(
            QPoint(static_cast<int>(p1.x()),static_cast<int>(p1.y())),
            QPoint(static_cast<int>(p2.x()),static_cast<int>(p2.y()))
        );
    };

private:
    Point3D p1, p2;
};


#endif //CG_LINE3D_H