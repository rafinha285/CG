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

    void updateBounds(double& minX, double& maxX,
                      double& minY, double& maxY,
                      double& minZ, double& maxZ) const override
    {
        p1.updateBounds(minX, maxX, minY, maxY, minZ, maxZ);
        p2.updateBounds(minX, maxX, minY, maxY, minZ, maxZ);
    }

protected:

    void drawTransformed(
        QPainter& painter,
        const Matrix4x4& finalTransform,
        const Matrix4x4 viewportTransform,
        Window3D* window
    ) const override
    {
        Vector3D v1 = finalTransform * this->p1.vector;
        Vector3D v2 = finalTransform * this->p2.vector;

        const double wMin = 0.1;

        bool v1_visible = v1.w() > wMin;
        bool v2_visible = v2.w() > wMin;

        if (!v1_visible && !v2_visible) return;

        if (v1_visible != v2_visible)
        {
            double t = (wMin - v1.w()) / (v2.w() - v1.w());

            double newX = v1.x() + (v2.x() - v1.x()) * t;
            double newY = v1.y() + (v2.y() - v1.y()) * t;
            double newZ = v1.z() + (v2.z() - v1.z()) * t;
            double newW = wMin;

            if (v1_visible)
            {
                v2 = v2 = Vector3D(newX, newY, newZ);;
            }else
            {
                v1 = Vector3D(newX, newY, newZ);
            }

            if (v1_visible)
            {
                auto p1_screen = Vector2D(v1.x() / v1.w(), v1.y() / v1.w());
                auto p2_screen = Vector2D(newX / newW, newY / newW);

                drawLine2D(painter, p1_screen, p2_screen, color, viewportTransform, window);
                return;
            }else
            {
                auto p1_screen = Vector2D(newX / newW, newY / newW);
                auto p2_screen = Vector2D(v2.x() / v2.w(), v2.y() / v2.w());

                drawLine2D(painter, p1_screen, p2_screen, color, viewportTransform, window);
                return;
            }

        }

        double w1_final = v1.w();
        double w2_final = v2.w();

        auto p1_screen = Vector2D(v1.x() / w1_final, v1.y() / w1_final);
        auto p2_screen = Vector2D(v2.x() / w2_final, v2.y() / w2_final);

        drawLine2D(painter, p1_screen, p2_screen, color, viewportTransform, window);
    };

    void drawLine2D(QPainter& painter, Vector2D p1_scr, Vector2D p2_scr, Color c, const Matrix4x4& vp, Window3D* win) const
    {
        auto p12D = Point(p1_scr.x(), p1_scr.y(), c);
        auto p22D = Point(p2_scr.x(), p2_scr.y(), c);
        auto line = Line(p12D, p22D, c);

        const Matrix3x3 viewportTransform2D = Matrix3x3(vp);
        auto* window2D = static_cast<Window*>(win);
        line.clipAndDraw(painter, viewportTransform2D, window2D);
    }
private:
    Point3D p1, p2;
};


#endif //CG_LINE3D_H