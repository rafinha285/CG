//
// Created by abacate on 23/10/2025.
//

#ifndef CG_SHAPE3D_H
#define CG_SHAPE3D_H
#include <QPainter>

#include "Shape.h"
#include "window/Window.h"
#include "matrix/Matrix4x4.h"


class Shape3D
{
public:
    virtual ~Shape3D() = default;

    void draw(
        QPainter &painter,
        const Matrix4x4& finalTransform,
        const Matrix4x4 viewportTransform,
        Window3D* window
    ) const
    {
        drawTransformed(painter, finalTransform, viewportTransform, window);
    }

    virtual void translate(double tx, double ty, double tz){}
    virtual void scale(double sx, double sy, double sz, const Vector3D& center = Vector3D(0,0,0)){}
    virtual void rotateX(double angle, const Vector3D& center){}
    virtual void rotateY(double angle, const Vector3D& center){}
    virtual void rotateZ(double angle, const Vector3D& center){}
    virtual Vector3D getCenter(){}

    void setColor(const Color color)
    {
        this->color = color;
    }

    void setName(const std::string& n) { m_name = n; }
    std::string getName() const { return m_name; }

protected:
    std::string m_name = "Objeto sem nome";
    virtual void drawTransformed(
        QPainter& painter,
        const Matrix4x4& finalTransform,
        Matrix4x4 viewportTransform,
        Window3D* window
    ) const = 0;
    Color color{};

};


#endif //CG_SHAPE3D_H