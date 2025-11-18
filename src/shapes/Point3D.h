//
// Created by abacate on 23/10/2025.
//

#ifndef CG_POINT3D_H // <-- MUDANÇA AQUI
#define CG_POINT3D_H
#include "Shape3D.h"
#include "../window/Window.h"
#include "../shapes/Point.h"   // <--- NOVO: Para poder usar a classe Point (2D)
#include "../matrix/Matrix3x3.h"


class Point3D: public Shape3D
{
public:
    Vector3D vector;

    explicit Point3D(const double x = 0, const double y = 0, const double z = 0, const Color c = {0,0,0})
    {
        this->color = c;
        this->vector = Vector3D(x, y, z);
    };

    explicit Point3D(const Vector3D& vector, const Color c = {0,0,0}): vector(vector)
    {
        this->color = c;
    }

    void translate(const double x, const double y, const double z) override
    {
        Matrix4x4 transMatrix = Matrix4x4::createTranslation(x, y, z);
        this->vector = transMatrix * this->vector;
    }

    void drawTransformed(
        QPainter& painter,
        const Matrix4x4& finalTransform,
        const Matrix4x4 viewportTransform,
        Window3D* window
    ) const override
    {
        Vector3D screenPoint = finalTransform * this->vector;
        double w = screenPoint.w();
        if (w == 0.0) return;

        double screenX = screenPoint.x() / w;
        double screenY = screenPoint.y() / w;

        // 1. CONVERSÃO DA MATRIZ (Usando o novo construtor)
        const Matrix3x3 viewportTransform2D = Matrix3x3(viewportTransform);

        // 2. CONVERSÃO DA WINDOW (Agora que a herança é pública)
        Window* window2D = static_cast<Window*>(window);

        auto point = Point(screenX, screenY, this->color); // Use o construtor com cor

        // 3. Chamada com os tipos 2D
        point.draw(painter, viewportTransform2D, window2D);
    }
};


#endif //CG_POINT_H