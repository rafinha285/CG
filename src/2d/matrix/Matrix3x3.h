//
// Created by abacate on 23/10/2025.
//

#ifndef CG_MATRIX3X3_H
#define CG_MATRIX3X3_H
#include "../vector/Vector2D.h"
#include "../viewport/Viewport.h"
#include "../window/Window.h"
#include <cmath>


class Window;

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
        constexpr int DIMENSION = 3;

        for (int i = 0; i < DIMENSION; i++)
        {
            double sum = 0;
            for (int j = 0; j < DIMENSION; j++)
            {
                sum += this->m[i][j] * v.m_coords[j];
            }
            result.m_coords[i] = sum;
        }
        return result;
    }

    static Matrix3x3 createRotationMatrix(const double angleDegrees)
    {
        Matrix3x3 rotMatrix;
        const double angleRad = angleDegrees * (M_PI / 180.0);
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

    static Matrix3x3 createTranslationMatrix(const double tx, const double ty)
    {
        Matrix3x3 transMatrix;
        transMatrix.m[0][2] = tx;
        transMatrix.m[1][2] = ty;
        return transMatrix;
    }

    static Matrix3x3 createScaleMatrix(const double sx, const double sy)
    {
        Matrix3x3 scaleMatrix;
        scaleMatrix.m[0][0] = sx;
        scaleMatrix.m[1][1] = sy;
        return scaleMatrix;
    }
    static Matrix3x3 createViewportMatrix(const Window& window, const Viewport& viewport)
    {
        const double tx = -window.min.m_coords[0];
        const double ty = -window.min.m_coords[1];

        const Matrix3x3 T1 = createTranslationMatrix(tx,ty);

        const double sx = (viewport.max.x() - viewport.min.x()) / (window.max.m_coords[0] - window.min.m_coords[0]);
        const double sy = (viewport.max.y() - viewport.min.y()) / (window.max.m_coords[1] - window.min.m_coords[1]);
        const Matrix3x3 S = createScaleMatrix(sx, sy);

        const double tx2 = viewport.min.x();
        const double ty2 = viewport.min.y();
        const Matrix3x3 T2 = createTranslationMatrix(tx2, ty2);

        const Matrix3x3 temp = T2 * S;
        const Matrix3x3 transform = temp * T1;
        return transform;
    }
};


#endif //CG_MATRIX3X3_H