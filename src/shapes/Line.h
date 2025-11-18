//
// Created by abacate on 23/10/2025.
//

#ifndef CG_LINE_H
#define CG_LINE_H
#include <utility>

#include "Point.h"
#include "Shape.h"


class Line final: public Shape
{
    public:
    Line(Point  p1, Point  p2) : p1(std::move(p1)), p2(std::move(p2)){};
    Line(Point p1 , Point p2 , Color color): p1(std::move(p1)), p2(std::move(p2))
    {
        this->color = color;
    };
    Line(const double x1, const double y1, const double x2, const double y2, const Color color) : p1(x1, y1, color), p2(x2, y2, color)
    {
       this->color = color;
    }

    Vector2D calculateMiddle() const
    {
        return {
            (this->p1.vector.x() + this->p2.vector.x()) / 2,
            (this->p1.vector.y() + this->p2.vector.y()) / 2
        };
    }

    void translate(const double tx, const double ty) override
    {
        this->p1.translate(tx, ty);
        this->p2.translate(tx, ty);
    }

    void scale(const double sx,const double sy) override
    {
        this->p1.scale(sx, sy);
        this->p2.scale(sx, sy);
    }

    void rotate(const double angle, const Vector2D& center) override
    {
        this->p1.rotate(angle, center);
        this->p2.rotate(angle, center);
    }

    Point getP1()
    {
        return this->p1;
    }
    Point getP2()
    {
        return this->p2;
    }

    void clipAndDraw(QPainter& painter, const Matrix3x3 viewportTransform, Window* window) const override
    {
        const double dx = this->p2.vector.x() - this->p1.vector.x();
        const double dy = this->p2.vector.y() - this->p1.vector.y();

        const double p[4] = {-dx, dx, -dy, dy};
        const double q[4] = {p1.vector.x() - window->min.x(), window->max.x() - p1.vector.x(),
                       p1.vector.y() - window->min.y(), window->max.y() - p1.vector.y()};

        double u1 = 0.0, u2 = 1.0;
        bool visible = true;
        for (int i = 0; i < 4; i++)
        {
            if (p[i] == 0)
            {
                if (q[i] < 0) {visible = false; break;}
            } else
            {
                const double t = q[i] / p[i];
                if (p[i] < 0)
                {
                    if (t>u1) u1 = t;
                }else
                {
                    if (t<u2) u2 = t;
                }
            }
        }
        if (u1 > u2) visible = false;

        if (visible)
        {
            const auto clipped_p1 = Vector2D(p1.vector.x() + u1 * dx, p1.vector.y() + u1 * dy);
            const auto clipped_p2 = Vector2D(p1.vector.x() + u2 * dx, p1.vector.y() + u2 * dy);

            const Vector2D transformed_p1 = viewportTransform * clipped_p1;
            const Vector2D transformed_p2 = viewportTransform * clipped_p2;

            painter.setPen(QPen(QColor(color.r, color.g, color.b), 2));
            painter.drawLine(
                QPoint(static_cast<int>(transformed_p1.x()), static_cast<int>(transformed_p1.y())),
                QPoint(static_cast<int>(transformed_p2.x()), static_cast<int>(transformed_p2.y()))
            );
        }
    }
private:
    Point p1, p2;
};


#endif //CG_LINE_H