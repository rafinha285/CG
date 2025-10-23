//
// Created by abacate on 23/10/2025.
//

#ifndef CG_VIEWPORT_H
#define CG_VIEWPORT_H


class Viewport
{
public:
    double xmin, ymin, xmax, ymax;

    Viewport(double xmin, double ymin, double xmax, double ymax)
        : xmin(xmin), ymin(ymin), xmax(xmax), ymax(ymax) {}
};



#endif //CG_VIEWPORT_H