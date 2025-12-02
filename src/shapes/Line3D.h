//
// Created by abacate on 28/10/2025.
//

#ifndef CG_LINE3D_H
#define CG_LINE3D_H
#include "shapes/Point3D.h"
#include "Point.h"
#include "Line.h"
// #include "Polygon3D.h"
#include "Shape3D.h"


class Line3D: public Shape3D
{

public:
    Line3D(const Point3D& point1, const Point3D& point2, Color color): p1(point1), p2(point2)
    {
        this->color = color;
    };

    Line3D(Point3D point1, Point3D point2): p1(point1), p2(point2)
    {
        this->color = {255,255,255};
    }

    Vector3D calculateMiddle() const
    {
        return {
            (this->p1.vector.x() + this->p2.vector.x())/2,
            (this->p1.vector.y() + this->p2.vector.y())/2,
            (this->p1.vector.z() + this->p2.vector.z())/2
        };
    }

    void translate(const double tx, const double ty, const double tz) override
    {
        this->p1.translate(tx, ty, tz);
        this->p2.translate(tx, ty, tz);
    }

    void scale(const double sx,const double sy, const double sz, const Vector3D& center) override
    {
        this->p1.scale(sx, sy, sz, center);
        this->p2.scale(sx, sy, sz, center);
    }

    void rotateX(double angle, const Vector3D& center) override
    {
        this->p1.rotateX(angle, center);
        this->p2.rotateX(angle, center);
    };

    void rotateY(double angle, const Vector3D& center) override
    {
        this->p1.rotateY(angle, center);
        this->p2.rotateY(angle, center);
    }

    void rotateZ(double angle, const Vector3D& center) override
    {
        this->p1.rotateZ(angle, center);
        this->p2.rotateZ(angle, center);
    }

    Point3D getP1()
    {
        return p1;
    }

    Point3D getP2()
    {
        return p2;
    }

    Vector3D getCenter() override
    {
        return calculateMiddle();
    }

protected:

    void drawTransformed(
        QPainter& painter,
        const Matrix4x4& finalTransform,
        const Matrix4x4 viewportTransform,
        Window3D* window
    ) const override
    {
        // 1. Transformação 3D (Coordenadas de Cena -> Coordenadas de Clipping/Projeção)
        auto p1Transformed = finalTransform * this->p1.vector;
        auto p2Transformed = finalTransform * this->p2.vector;

        // Aplicação da Divisão por W (Perspectiva) - É fundamental antes de ir para 2D
        double w1 = p1Transformed.w();
        double w2 = p2Transformed.w();

        if (w1 == 0.0 || w2 == 0.0) return; // Evita divisão por zero

        auto p1_screen = Vector2D(p1Transformed.x() / w1, p1Transformed.y() / w1);
        auto p2_screen = Vector2D(p2Transformed.x() / w2, p2Transformed.y() / w2);

        auto p12D = Point(p1_screen.x(), p1_screen.y(), this->color);
        auto p22D = Point(p2_screen.x(), p2_screen.y(), this->color);
        auto line = Line(p12D, p22D, this->color);

        // 3. Conversão das Matrizes e Windows para o 2D (Clipping)

        // --- CORREÇÃO AQUI: Passe a matrix viewportTransform para o construtor ---
        const Matrix3x3 viewportTransform2D = Matrix3x3(viewportTransform);

        auto* window2D = static_cast<Window*>(window);

        // 4. Chamada da rotina 2D (Clipping + Desenho)
        line.clipAndDraw(painter, viewportTransform2D, window2D);
    };

private:
    Point3D p1, p2;
};


#endif //CG_LINE3D_H