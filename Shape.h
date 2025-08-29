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

class LineClass: public Shape
{
public:
    LineClass(const Point& p1, const Point& p2, const Color& c): p1(p1), p2(p2) {color=c;}
    void draw(QPainter& painter) const override
    {
        painter.setPen(QPen(QColor(color.r,color.g,color.b),2));
        painter.drawLine(QPoint(p1.x,p1.y),QPoint(p2.x,p2.y));
    }

private:
    Point p1,p2;
};

#endif //CG_SHAPE_H