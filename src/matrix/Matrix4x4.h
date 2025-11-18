//
// Created by abacate on 23/10/2025.
//

#ifndef CG_MATRIX4X4_H
#define CG_MATRIX4X4_H
#include <cmath>

#include "../vector/Vector3D.h"
#include "../window/Window3D.h"
#include "viewport/Viewport.h"

class Vector3D;

class Matrix4x4
{
    static constexpr int DIMENSION = 4;

    friend class Vector3D;
public:
    double m[DIMENSION][DIMENSION];
    Matrix4x4(): m{
        {1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}
    }{};

    Matrix4x4 operator*(const Matrix4x4 &other) const
    {
        Matrix4x4 result;
        for(int i = 0; i < DIMENSION; i++)
        {
            for(int j = 0; j < DIMENSION; j++)
            {
                result.m[i][j] = 0.0;
                for(int k = 0; k < DIMENSION; k++)
                {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }

    Vector3D operator*(const Vector3D &v) const
    {
        Vector3D result;
        for (int i = 0; i < DIMENSION; i++)
        {
            double sum = 0.0;
            for(int j = 0; j < DIMENSION; j++)
            {
                sum += this->m[i][j] * v.m_coords[j];
            }
            result.m_coords[i] = sum;
        }
        return result;
    }

    static Matrix4x4 createTranslation(double tx, double ty, double tz)
    {
        class Matrix4x4 result;
        result.m[0][3] = tx;
        result.m[1][3] = ty;
        result.m[2][3] = tz;
        return result;
    }

    static Matrix4x4 createScale(double sx, double sy, double sz)
    {
        class Matrix4x4 result;
        result.m[0][0] = sx;
        result.m[1][1] = sy;
        result.m[2][2] = sz;
        return result;
    }

    static Matrix4x4 createRotationX(double angle)
    {
        class Matrix4x4 result;
        double c = cos(angle);
        double s = sin(angle);

        result.m[1][1] = c;
        result.m[1][2] = -s;
        result.m[2][1] = s;
        result.m[2][2] = c;
        return result;
    }

    static Matrix4x4 createRotationZ(double angle)
    {
        Matrix4x4 result;
        double c = cos(angle);
        double s = sin(angle);
        result.m[0][0] = c;
        result.m[1][1] = c;
        result.m[0][1] = s;
        result.m[1][0] = -s;
        return result;
    }

    static Matrix4x4 createRotationY(double angle)
    {
        class Matrix4x4 result;
        double c = cos(angle);
        double s = sin(angle);
        result.m[0][0] = c;
        result.m[2][0] = s;
        result.m[0][2] = -s;
        result.m[2][2] = c;
        return result;
    }

    static Matrix4x4 createOrthographic(const Window3D& window)
    {
        double r = window.max.x(); // Right
        double l = window.min.x(); // Left
        double t = window.max.y(); // Top
        double b = window.min.y(); // Bottom
        double f = window.max.z(); // Far
        double n = window.min.z(); // Near

        Matrix4x4 result; // Começa como uma matriz identidade

        result.m[0][0] = 2.0 / (r - l);
        result.m[1][1] = 2.0 / (t - b);
        result.m[2][2] = -2.0 / (f - n); // Inverte Z (OpenGL/Vulkan)

        result.m[0][3] = -(r + l) / (r - l);
        result.m[1][3] = -(t + b) / (t - b);
        result.m[2][3] = -(f + n) / (f - n);
        result.m[3][3] = 1.0;

        // Zera os outros elementos da diagonal principal (se não for identidade)
        result.m[3][0] = 0.0;
        result.m[3][1] = 0.0;
        result.m[3][2] = 0.0;
        // ... (zere outros elementos se sua matriz não inicializar como 0)

        return result;
    }

    static Matrix4x4 createViewport(const Viewport& viewport)
    {
        double vp_x = viewport.min.x();
        double vp_y = viewport.min.y();
        double vp_w = viewport.max.x() - viewport.min.x();
        double vp_h = viewport.max.y() - viewport.min.y();

        Matrix4x4 result; // Começa como identidade

        result.m[0][0] = vp_w / 2.0;
        result.m[1][1] = -vp_h / 2.0; // Inverte Y (comum em Qt/Windows)
        result.m[2][2] = 1.0; // Mapeia Z (pode ser 0.5 e 0.5 se quiser [0,1])

        result.m[0][3] = vp_x + (vp_w / 2.0);
        result.m[1][3] = vp_y + (vp_h / 2.0); // Ponto de origem (canto superior esquerdo)
        result.m[2][3] = 0.0; // (ou 0.5)
        result.m[3][3] = 1.0;

        return result;
    }

    static Matrix4x4 createLookAt(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
    {
        // 1. Calcula os eixos da câmera
        // (O Vector3D() é usado para subtrair vetores)
        Vector3D zaxis = Vector3D::normalize(Vector3D(eye.x() - target.x(), eye.y() - target.y(), eye.z() - target.z()));
        Vector3D xaxis = Vector3D::normalize(Vector3D::cross(up, zaxis));
        Vector3D yaxis = Vector3D::cross(zaxis, xaxis);

        Matrix4x4 viewMatrix; // Começa como identidade

        // 2. Cria a matriz de rotação da câmera
        viewMatrix.m[0][0] = xaxis.x();
        viewMatrix.m[0][1] = xaxis.y();
        viewMatrix.m[0][2] = xaxis.z();

        viewMatrix.m[1][0] = yaxis.x();
        viewMatrix.m[1][1] = yaxis.y();
        viewMatrix.m[1][2] = yaxis.z();

        viewMatrix.m[2][0] = zaxis.x();
        viewMatrix.m[2][1] = zaxis.y();
        viewMatrix.m[2][2] = zaxis.z();

        // 3. Adiciona a translação (o "olho" da câmera)
        viewMatrix.m[0][3] = -Vector3D::dot(xaxis, eye);
        viewMatrix.m[1][3] = -Vector3D::dot(yaxis, eye);
        viewMatrix.m[2][3] = -Vector3D::dot(zaxis, eye);

        return viewMatrix;
    }

    static Matrix4x4 createPerspective(
        const double fovDegrees,
        const double aspectRatio,
        const double nearPlane,
        const double farPlane
    )
    {
        Matrix4x4 result;

        for (auto & i : result.m) for (double & j : i) j = 0.0;

        const double fovRad = fovDegrees * (M_PI / 180.0);
        const double tanHalfFov = std::tan(fovRad / 2.0);
        const double f = 1.0 / tanHalfFov;

        result.m[0][0] = f / aspectRatio;
        result.m[1][1] = f;
        result.m[2][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
        result.m[2][3] = -(2.0 * farPlane * nearPlane) / (farPlane- nearPlane);

        result.m[3][2] = -1.0;
        result.m[3][3] = 0.0;
        return result;
    }

    double get(int x, int y) const
    {
        return this->m[x][y];
    }
};


#endif //CG_MATRIX4X4_H