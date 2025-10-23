//
// Created by abacate on 23/10/2025.
//

#ifndef CG_SQUARE_H
#define CG_SQUARE_H
#include "Polygon.h"


class Square: public Polygon
{
public:
    explicit Square(const int x, const int y, const int sideLength, const Color color )
    {
        const Point p1(x, y, color);
        const Point p2(x + sideLength, y, color);
        const Point p3(x + sideLength, y + sideLength, color);
        const Point p4(x, y + sideLength, color);

        this->addLine(Line(p1, p2));
        this->addLine(Line(p2, p3));
        this->addLine(Line(p3, p4));
        this->addLine(Line(p4, p1));

        this->color = color;
    };
};


#endif //CG_SQUARE_H