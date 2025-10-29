//
// Created by abacate on 29/10/2025.
//

#ifndef CG_POLYGON3D_H
#define CG_POLYGON3D_H
#include "Shape3D.h"
#include "Line3D.h"


class Polygon3D: public Shape3D
{
public:
    Polygon3D()
    {
        this->lines = std::vector<Line3D>();
    }

    Polygon3D(const std::vector<Line3D>& lines)
    {
        this->lines = lines;
    }

    void addLine(const Line3D& line)
    {
        this->lines.push_back(line);
    }

    void rotateX(double angle)
    {
        Vector3D center = this->getCenter();
        for (Line3D& line : this->lines)
        {
            line.rotateX(angle, center);
        }
    }

    void rotateY(double angle)
    {
        Vector3D center = this->getCenter();
        for (Line3D& line : this->lines)
        {
            line.rotateY(angle, center);
        }
    }

    void rotateZ(double angle)
    {
        Vector3D center = this->getCenter();
        for (Line3D& line : this->lines)
        {
            line.rotateZ(angle, center);
        }
    };


    Vector3D getCenter() const
    {
        if (lines.empty())
        {
            return Vector3D(0, 0, 0);
        }
        double sumX = 0;
        double sumY = 0;
        double sumZ = 0;

        for (Line3D line : lines)
        {
            sumX += line.getP1().vector.x();
            sumY += line.getP1().vector.y();
            sumZ += line.getP1().vector.z();
        }

        const double avgX = sumX / lines.size();
        const double avgY = sumY / lines.size();
        const double avgZ = sumZ / lines.size();
        return Vector3D(avgX, avgY, avgZ);
    }

    void serColor(Color color)
    {
        this->color = color;
    }
protected:
    void drawTransformed(QPainter& painter, const Matrix4x4& finalTransform) const override
    {
        for (const Line3D& line : lines)
        {
            line.draw(painter, finalTransform);
        }
    };
private:
    std::vector<Line3D> lines;
};


#endif //CG_POLYGON3D_H