//
// Created by abacate on 28/08/2025.
//

#ifndef CG_SHAPE_H
#define CG_SHAPE_H
#include <QPainter>
#include <cmath>

constexpr double PI = 3.14;

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
    Vector2D(const double x, const double y)
    {
        coords[0] = x;
        coords[1] = y;
        coords[2] = 1.0;
    }
    double coords[3];
};

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

class Viewport
{
public:
    double xmin, ymin, xmax, ymax;

    Viewport(double xmin, double ymin, double xmax, double ymax)
        : xmin(xmin), ymin(ymin), xmax(xmax), ymax(ymax) {}
};

class Matrix3x3
{
public:
    double m[3][3];

    Matrix3x3() : m{
          {1.0, 0.0, 0.0},
          {0.0, 1.0, 0.0},
          {0.0, 0.0, 1.0}
    }{}

    Matrix3x3 operator*(const Matrix3x3& other) const
    {
        Matrix3x3 result;
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                result.m[i][j] = 0;
                for (int k = 0; k < 3; ++k)
                {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }

    // Sobrecarga do operador * para multiplicação de matriz por vetor
    Vector2D operator*(const Vector2D &v) const
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
    static Matrix3x3 createViewportMatrix(const Window& window, const Viewport& viewport)
    {
        Matrix3x3 transform;
        double tx = -window.min.coords[0];
        double ty = -window.min.coords[1];

        Matrix3x3 T1 = Matrix3x3::createTranslationMatrix(tx,ty);

        double sx = (viewport.xmax - viewport.xmin) / (window.max.coords[0] - window.min.coords[0]);
        double sy = (viewport.ymax - viewport.ymin) / (window.max.coords[1] - window.min.coords[1]);
        Matrix3x3 S = Matrix3x3::createScaleMatrix(sx, sy);

        double tx2 = viewport.xmin;
        double ty2 = viewport.ymin;
        Matrix3x3 T2 = Matrix3x3::createTranslationMatrix(tx2, ty2);

        Matrix3x3 temp = T2 * S;
        transform = temp * T1;
        return transform;
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
    void draw(QPainter &painter, const Matrix3x3 viewportTransform, Window *window) const
    {
        clipAndDraw(painter,viewportTransform,window);
    };



    virtual void translate(double tx, double ty) {}
    virtual void scale(double sx, double sy) {}
    virtual void rotate(double angle) {}


    void setColor(const Color color)
    {
        this->color = color;
    }

protected:
    virtual void clipAndDraw(QPainter &painter, Matrix3x3 viewportTransform, Window *window) const = 0;
    Color color{};
};

class Point : public Shape
{
public:
    Vector2D vector;

    // Construtor simplificado, agora não precisa da PointSimple
    Point(const double x = 0, const double y = 0, const Color c = {0,0,0})
    {
        this->color = c;
        this->vector = Vector2D(x, y);
    }

    // Construtor para criar a partir de um Vector2D existente
    Point(const Vector2D& v, const Color c = {0,0,0}) : vector(v) {
        this->color = c;
    }


    void translate(double tx, double ty) override
    {
        Matrix3x3 transMatrix = Matrix3x3::createTranslationMatrix(tx, ty);
        this->vector = transMatrix * this->vector; // Usa o operador *
    }

    void scale(double sx, double sy) override
    {
        Matrix3x3 scaleMatrix = Matrix3x3::createScaleMatrix(sx, sy);
        this->vector = scaleMatrix * this->vector; // Usa o operador *
    }

    void rotate(double angle) override
    {
        Matrix3x3 rotMatrix = Matrix3x3::createRotationMatrix(angle);
        this->vector = rotMatrix * this->vector; // Usa o operador *
    }

    Color getColor() const
    {
        return this->color;
    }

    void clipAndDraw(QPainter& painter,Matrix3x3 viewportTransform, Window* window) const override
    {
        if (this->vector.coords[0] >= window->min.coords[0] &&
            this->vector.coords[0] <= window->max.coords[0] &&
            this->vector.coords[1] >= window->min.coords[1] &&
            this->vector.coords[1] <= window->max.coords[1])
        {
            Vector2D transformedVector = viewportTransform * this->vector;
            painter.setPen(QPen(QColor(color.r, color.g, color.b), 2));
            painter.drawPoint(QPoint(static_cast<int>(transformedVector.coords[0]), static_cast<int>(transformedVector.coords[1])));
        }
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

    Vector2D calculateMiddle()
    {
        return {
            (this->p1.vector.coords[0] + this->p2.vector.coords[0]) / 2,
            (this->p1.vector.coords[1] + this->p2.vector.coords[1]) / 2
        };
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

    void clipAndDraw(QPainter& painter, Matrix3x3 viewportTransform, Window* window) const override
    {
        double dx = this->p2.vector.coords[0] - this->p1.vector.coords[0];
        double dy = this->p2.vector.coords[1] - this->p1.vector.coords[1];

        double p[4] = {-dx, dx, -dy, dy};
        double q[4] = {p1.vector.coords[0] - window->min.coords[0], window->max.coords[0] - p1.vector.coords[0],
                       p1.vector.coords[1] - window->min.coords[1], window->max.coords[1] - p1.vector.coords[1]};

        double u1 = 0.0, u2 = 1.0;
        bool visible = true;
        for (int i = 0; i < 4; i++)
        {
            if (p[i] == 0)
            {
                if (q[i] < 0) {visible = false; break;}
            } else
            {
                double t = q[i] / p[i];
                if (p[i] < 0)
                {
                    if (t>u1) u1 = t;
                }else
                {
                    if (t<u2) u2 = t;
                }
            }
        }
        if (u1 > u2) visible = false;

        if (visible)
        {
            Vector2D clipped_p1 = Vector2D(p1.vector.coords[0] + u1 * dx, p1.vector.coords[1] + u1 * dy);
            Vector2D clipped_p2 = Vector2D(p1.vector.coords[0] + u2 * dx, p1.vector.coords[1] + u2 * dy);

            Vector2D transformed_p1 = viewportTransform * clipped_p1;
            Vector2D transformed_p2 = viewportTransform * clipped_p2;

            painter.setPen(QPen(QColor(color.r, color.g, color.b), 2));
            painter.drawLine(
                QPoint(static_cast<int>(transformed_p1.coords[0]), static_cast<int>(transformed_p1.coords[1])),
                QPoint(static_cast<int>(transformed_p2.coords[0]), static_cast<int>(transformed_p2.coords[1]))
            );
        }
    }
private:
    Point p1, p2;
};

// class Polygon : public Shape
// {
// public:
//     Polygon()
//     {
//         this->lines = std::vector<Line>();
//     }
//
//     void draw(QPainter& painter,  const Matrix3x3 viewportTransform ,Window *window) const override
//     {
//         for (int i = 0; i < this->lines.size(); i++)
//         {
//             lines.at(i).draw(painter, viewportTransform, window);
//         }
//     };
//
//     void addLine(const Line &line)
//     {
//         this->lines.push_back(line);
//     }
//     void addLines(const std::vector<Line> &lines)
//     {
//         for (const auto & line : lines)
//         {
//             this->addLine(line);
//         }
//     }
//
//     Vector2D calculateMiddle()
//     {
//         double sumX = 0, sumY = 0;
//         for (auto & line : this->lines)
//         {
//             Vector2D point = line.calculateMiddle();
//             sumX += + point.coords[0];
//             sumY += + point.coords[1];
//         }
//         return {sumX/lines.size(), sumY/lines.size()};
//     }
//
//     void translateToOrigin()
//     {
//         Vector2D middle = this->calculateMiddle();
//         qDebug("%f %f", middle.coords[0], middle.coords[1]);
//         this->translate(-middle.coords[0], -middle.coords[1]);
//     }
//
//     void translate(const double tx,const double ty) override
//     {
//         for (auto & line : this->lines)
//         {
//             line.translate(tx, ty);
//         }
//     }
//
//     void scale(const double sx,const double sy) override
//     {
//         Vector2D temp = this->calculateMiddle();
//         this->translateToOrigin();
//         for (auto & line : this->lines)
//         {
//             line.scale(sx, sy);
//         }
//         this->translate(temp.coords[0], temp.coords[1]);
//     }
//
//     void rotate(double angle) override
//     {
//         Vector2D temp = this->calculateMiddle();
//         this->translateToOrigin();
//         for (auto & line : this->lines)
//         {
//             line.rotate(angle);
//         }
//         this->translate(temp.coords[0], temp.coords[1]);
//     }
// protected:
//     std::vector<Line> lines;
// };

// class Square final : public Polygon
// {
// public:
//     Square(double x, double y, double sideLength, const Color& color)
//     {
//         // Define os quatro pontos do quadrado no espaço 'mundo'
//         Point p1(x, y, color);
//         Point p2(x + sideLength, y, color);
//         Point p3(x + sideLength, y + sideLength, color);
//         Point p4(x, y + sideLength, color);
//
//         // Cria as linhas a partir dos pontos e as adiciona ao polígono
//         this->addLine(Line(p1, p2));
//         this->addLine(Line(p2, p3));
//         this->addLine(Line(p3, p4));
//         this->addLine(Line(p4, p1));
//     }
//
//     void clipAndDraw(QPainter& painter, Matrix3x3 viewportTransform, Window* window) const override
//     {
//         for (Line line : lines)
//         {
//             line.clipAndDraw();
//         }
//     };
// };

#endif //CG_SHAPE_H