//
// Created by abacate on 23/10/2025.
//

#ifndef CG_POINT3D_H // <-- MUDANÇA AQUI
#define CG_POINT3D_H
#include "Shape3D.h"


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

    void drawTransformed(QPainter& painter, const Matrix4x4& finalTransform) const override
    {
        Vector3D screenPoint = finalTransform * this->vector;
        double w = screenPoint.w();
        if (w == 0.0) return; // Evita divisão por zero

        double screenX = screenPoint.x() / w;
        double screenY = screenPoint.y() / w;

        // 3. Desenha o ponto na tela
        painter.setPen(QPen(QColor(color.r, color.g, color.b), 3)); // Caneta de 3px
        painter.drawPoint(static_cast<int>(screenX), static_cast<int>(screenY));
    }
};


#endif //CG_POINT_H