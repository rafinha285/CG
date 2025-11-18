//
// Created by abacate on 23/10/2025.
//

#ifndef CG_WINDOW3D_H
#define CG_WINDOW3D_H
#include "Window.h"
#include "../vector/Vector3D.h"


class Window3D
    : public Window{
public:
    Vector3D min;
    Vector3D max;

    Window3D(Vector3D min, Vector3D max) :
        Window(min.x(), min.y(), max.x(), max.y())
    {
        this->min = min;
        this->max = max;
    }

    // Construtor com 6 doubles
    Window3D(
        double xmin,
        double ymin,
        double zmin,
        double xmax,
        double ymax,
        double zmax
    ) : Window(xmin, ymin, xmax, ymax) // Chama o construtor base 2D
    {
        this->min = Vector3D(xmin, ymin, zmin);
        this->max = Vector3D(xmax, ymax, zmax);
    }
};


#endif //CG_WINDOW3D_H