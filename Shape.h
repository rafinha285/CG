//
// Created by abacate on 28/08/2025.
//

#ifndef CG_SHAPE_H
#define CG_SHAPE_H
#include <QPainter>

struct Point
{
    int x, y;
};
struct Color
{
    int r,g,b;
};


class Shape
{
public:
    virtual ~Shape() = default;
    virtual void draw(QPainter& painter) const = 0;

    protected:
    Color color;
};

class PointClass: public Shape
{
public:
    PointClass(const Point& point, const Color& c): p(point) {color=c;}
    void draw(QPainter& painter) const override
    {
        painter.setPen(QPen(QColor(color.r,color.g,color.b),2));
        painter.drawPoint(QPoint(p.x,p.y));
    };
private:
    Point p;
};


#endif //CG_SHAPE_H