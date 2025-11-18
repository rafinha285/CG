//
// Created by abacate on 23/10/2025.
//

#ifndef CG_POLYGON_H
#define CG_POLYGON_H
#include "Line.h"
#include "Shape.h"


class Polygon: public Shape
{
public:
    Polygon()
    {
        this->lines = std::vector<Line>();
    }
    void clipAndDraw(QPainter& painter, Matrix3x3 viewportTransform, Window* window) const override
    {
        for (const Line& line: lines)
        {
            line.clipAndDraw(painter, viewportTransform, window);
        }
    };

    void addLine(const Line& line)
    {
        this->lines.push_back(line);
    }

    void rotate(double angle)
    {
        Vector2D center = this->getCenter();
        for (Line& line: this->lines)
        {
            line.rotate(angle, center);
        }
    };

    Vector2D getCenter() const
    {
        if (lines.empty()) {
            return Vector2D(0, 0); // Retorna origem se não houver linhas
        }

        double sumX = 0.0;
        double sumY = 0.0;

        // Itera sobre cada linha e soma os 'x' e 'y' do seu primeiro ponto
        for (Line line : lines)
        {
            // (Você pode precisar tornar 'p1' público em Line,
            // ou criar um getter line.getP1())
            sumX += line.getP1().vector.x();
            sumY += line.getP1().vector.y();
        }

        // Retorna a média
        return Vector2D(sumX / lines.size(), sumY / lines.size());
    }

protected:
    std::vector<Line> lines;
};


#endif //CG_POLYGON_H