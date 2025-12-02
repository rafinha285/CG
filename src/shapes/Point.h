//
// Created by abacate on 23/10/2025.
//

#ifndef CG_POINT_H
#define CG_POINT_H
#include "Shape.h"


class Point: public Shape
{
public:
    Vector2D vector;

    // Construtor simplificado, agora não precisa da PointSimple
    explicit Point(const double x = 0, const double y = 0, const Color c = {0,0,0})
    {
        this->color = c;
        this->vector = Vector2D(x, y);
    }

    // Construtor para criar a partir de um Vector2D existente
    explicit Point(const Vector2D& v, const Color c = {0,0,0}) : vector(v) {
        this->color = c;
    }


    void translate(double tx, double ty) override
    {
        Matrix3x3 transMatrix = Matrix3x3::createTranslationMatrix(tx, ty);
        this->vector = transMatrix * this->vector; // Usa o operador *
    }

    void scaleWithCenter(double sx, double sy, const Vector2D& center)
    {
        Matrix3x3 scaleMatrix = Matrix3x3::createScaleInCenterMatrix(sx, sy, center);
        this->vector = scaleMatrix * this->vector;
    }

    void scale(double sx, double sy) override
    {
        this->scaleWithCenter(sx, sy, this->vector);
    }

    void rotate(double angle, const Vector2D& center) override
    {
        Matrix3x3 rotationMatrix = Matrix3x3::createRotationInCenterMatrix(angle, center);
        this->vector = rotationMatrix * this->vector;
    }

    void rotate(double angle)
    {
        this->rotate(angle, this->vector);
    }

    Color getColor() const
    {
        return this->color;
    }

    void clipAndDraw(QPainter& painter,Matrix3x3 viewportTransform, Window* window) const override
    {
        if (this->vector.x() >= window->min.x() &&
            this->vector.x() <= window->max.x() &&
            this->vector.y() >= window->min.y() &&
            this->vector.y() <= window->max.y())
        {
            Vector2D transformedVector = viewportTransform * this->vector;
            painter.setPen(QPen(QColor(color.r, color.g, color.b), 2));
            painter.drawPoint(QPoint(static_cast<int>(transformedVector.x()), static_cast<int>(transformedVector.y())));
        }
    };
};

#endif //CG_POINT_H