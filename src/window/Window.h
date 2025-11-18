//
// Created by abacate on 23/10/2025.
//

#ifndef CG_WINDOW_H
#define CG_WINDOW_H

#include "vector/Vector2D.h"


class Window
{
public:
    Vector2D min; // Coordenada inferior esquerda (xmin, ymin)
    Vector2D max; // Coordenada superior direita (xmax, ymax)

    Window(double xmin, double ymin, double xmax, double ymax)
    {
        min = Vector2D(xmin, ymin);
        max = Vector2D(xmax, ymax);
    }
};

#endif //CG_WINDOW_H