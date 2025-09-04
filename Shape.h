//
// Created by abacate on 28/08/2025.
//

#ifndef CG_SHAPE_H
#define CG_SHAPE_H
#include <QPainter>
#include <cmath>

constexpr double PI = 3.14;

struct PointSimple
{
    int x;
    int y;
};

class Vector2D
{
public:
    ~Vector2D() = default;
    Vector2D()
    {
        coords[0] = 0.0;
        coords[1] = 0.0;
        coords[2] = 1.0;
    }
    Vector2D(const int x,const int y)
    {
        coords[0] = x;
        coords[1] = y;
        coords[2] = 1.0;
    }
    int coords[3];
};

class Matrix3x3
{
public:
    double m[3][3];

    Matrix3x3() : m{
                      {1.0, 0.0, 0.0},
                      {0.0, 1.0, 0.0},
                      {0.0, 0.0, 1.0}}
    {
    }
    Vector2D multiply(const Vector2D &v) const
    {
        Vector2D result;
        const int DIMENSION = 3;

        for (int i = 0; i < DIMENSION; i++)
        {
            double sum = 0;
            for (int j = 0; j < DIMENSION; j++)
            {
                sum += this->m[i][j] * v.coords[j];
            }
            result.coords[i] = sum;
        }
        return result;
    }

    static Matrix3x3 createRotationMatrix(const double angleDegrees)
    {
        Matrix3x3 rotMatrix;
        const double angleRad = angleDegrees * PI / 180.0;
        const double c = cos(angleRad);
        const double s = sin(angleRad);

        rotMatrix.m[0][0] = c;
        rotMatrix.m[0][1] = -s;
        rotMatrix.m[1][0] = s;
        rotMatrix.m[1][1] = c;

        rotMatrix.m[0][2] = 0.0;
        rotMatrix.m[1][2] = 0.0;
        rotMatrix.m[2][0] = 0.0;
        rotMatrix.m[2][1] = 0.0;
        rotMatrix.m[2][2] = 1.0;


        return rotMatrix;
    }

    static Matrix3x3 createTranslationMatrix(double tx, double ty)
    {
        Matrix3x3 transMatrix;
        transMatrix.m[0][2] = tx;
        transMatrix.m[1][2] = ty;
        return transMatrix;
    }

    static Matrix3x3 createScaleMatrix(double sx, double sy)
    {
        Matrix3x3 scaleMatrix;
        scaleMatrix.m[0][0] = sx;
        scaleMatrix.m[1][1] = sy;
        return scaleMatrix;
    }
};

struct Color
{
    int r, g, b;
};

class Shape
{
public:
    virtual ~Shape() = default;
    virtual void draw(QPainter &painter) const = 0;

    virtual void translate(double tx, double ty) {}
    virtual void scale(double sx, double sy) {}
    virtual void rotate(double angle) {}

    void setColor(const Color color)
    {
        this->color = color;
    }

protected:
    Color color{};
};

class Point final : public Shape
{
public:
    Vector2D vector;

    Point(PointSimple point) : vector(point.x, point.y){};

    Point(const double x = 0, const double y = 0, const Color c = {0,0,0})
    {
        this->color = c;
        this->vector = Vector2D(x, y);
    }

    void translate(double tx, double ty) override
    {
        Matrix3x3 transMatrix = Matrix3x3::createTranslationMatrix(tx, ty);
        this->vector = transMatrix.multiply(this->vector);
    }

    void scale(double sx, double sy) override
    {
        Matrix3x3 scaleMatrix = Matrix3x3::createScaleMatrix(sx, sy);
        this->vector = scaleMatrix.multiply(this->vector);
    }

    void rotate(double angle) override
    {
        Matrix3x3 rotMatrix = Matrix3x3::createRotationMatrix(angle);
        this->vector = rotMatrix.multiply(this->vector);
    }

    Color getColor() const
    {
        return this->color;
    }

    void draw(QPainter &painter) const override
    {
        painter.setPen(QPen(QColor(color.r, color.g, color.b), 2));
        painter.drawPoint(QPoint(static_cast<int>(vector.coords[0]), static_cast<int>(vector.coords[1])));
    };
};

class Line final : public Shape
{
public:
    Line(Point& p1, Point& p2) : p1(p1), p2(p2){};
    Line(double x1, double y1, double x2, double y2, Color color) : p1(x1, y1, color), p2(x2, y2, color)
    {
       this->color = color;
    }

    PointSimple calculateMiddle()
    {
        return {
            (this->p1.vector.coords[0] + this->p2.vector.coords[0]) / 2,
            (this->p1.vector.coords[1] + this->p2.vector.coords[1]) / 2
        };
    }

    void draw(QPainter &painter) const override
    {
        painter.setPen(QPen(QColor(p1.getColor().r, p1.getColor().g, p1.getColor().b), 2));
        painter.drawLine(
            QPoint(p1.vector.coords[0],p1.vector.coords[1]),
            QPoint(p2.vector.coords[0],p2.vector.coords[1])
        );
    }

    void translate(const double tx, const double ty) override
    {
        this->p1.translate(tx, ty);
        this->p2.translate(tx, ty);
    }

    void scale(const double sx,const double sy) override
    {
        this->p1.scale(sx, sy);
        this->p2.scale(sx, sy);
    }

    void rotate(const double angle) override
    {
        this->p1.rotate(angle);
        this->p2.rotate(angle);
    }

    Point getP1()
    {
        return this->p1;
    }
    Point getP2()
    {
        return this->p2;
    }

private:
    Point p1, p2;
};

class Polygon final : public Shape
{
public:
    Polygon()
    {
        this->lines = std::vector<Line>();
    }

    void draw(QPainter& painter) const override
    {
        for (int i = 0; i < this->lines.size(); i++)
        {
            lines.at(i).draw(painter);
        }
    };

    void addLine(const Line &line)
    {
        this->lines.push_back(line);
    }
    void addLines(const std::vector<Line> &lines)
    {
        for (const auto & line : lines)
        {
            this->addLine(line);
        }
    }

    PointSimple calculateMiddle()
    {
        int sumX = 0, sumY = 0;
        for (auto & line : this->lines)
        {
            PointSimple point = line.calculateMiddle();
            sumX += + point.x;
            sumY += + point.y;
        }
        return {static_cast<int>(sumX/lines.size()), static_cast<int>(sumY/lines.size())};
    }

    void translateToOrigin()
    {
        PointSimple middle = this->calculateMiddle();
        qDebug("%d %d",middle.x, middle.y);
        this->translate(-middle.x,-middle.y);
    }

    void translate(const double tx,const double ty) override
    {
        for (auto & line : this->lines)
        {
            line.translate(tx, ty);
        }
    }

    void scale(const double sx,const double sy) override
    {
        PointSimple temp = this->calculateMiddle();
        this->translateToOrigin();
        for (auto & line : this->lines)
        {
            line.scale(sx, sy);
        }
        this->translate(temp.x, temp.y);
    }

    void rotate(double angle) override
    {
        PointSimple temp = this->calculateMiddle();
        this->translateToOrigin();
        for (auto & line : this->lines)
        {
            line.rotate(angle);
        }
        this->translate(temp.x, temp.y);
    }
private:
    std::vector<Line> lines;
};

#endif //CG_SHAPE_H