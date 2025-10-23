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

    void scale(double sx, double sy) override
    {
        Matrix3x3 scaleMatrix = Matrix3x3::createScaleMatrix(sx, sy);
        this->vector = scaleMatrix * this->vector; // Usa o operador *
    }

    void rotate(double angle, const Vector2D& center) override
    {
        Matrix3x3 T_neg = Matrix3x3::createTranslationMatrix(-center.x(), -center.y());

        Matrix3x3 R =  Matrix3x3::createRotationMatrix(angle);

        // 3. Cria matriz de translação DE VOLTA para o local
        Matrix3x3 T_pos =  Matrix3x3::createTranslationMatrix(center.x(), center.y());

        // 4. Aplica a transformação composta ao vetor deste ponto
        // A ordem é importante: P' = T_pos * R * T_neg * P
        this->vector = T_pos * R * T_neg * this->vector;
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