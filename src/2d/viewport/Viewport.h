//
// Created by abacate on 23/10/2025.
//

#ifndef CG_VIEWPORT_H
#define CG_VIEWPORT_H


class Viewport
{
public:
    // double xmin, ymin, xmax, ymax;
    Vector2D min,max;

    Viewport(double xmin, double ymin, double xmax, double ymax)
    {
        this->min = Vector2D(xmin, ymin);
        this->max = Vector2D(xmax, ymax);
    }
};



#endif //CG_VIEWPORT_H