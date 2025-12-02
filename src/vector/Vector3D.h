//
// Created by abacate on 23/10/2025.
//

#ifndef CG_VECTOR3D_H
#define CG_VECTOR3D_H

#include <cmath>

class Matrix4x4;

class Vector3D
{
    friend class Matrix4x4;
public:
    Vector3D()
    {
        m_coords[0] = 0.0;
        m_coords[1] = 0.0;
        m_coords[2] = 0.0;
        m_coords[3] = 1.0;
    };

    Vector3D(double x, double y, double z)
    {
        m_coords[0] = x;
        m_coords[1] = y;
        m_coords[2] = z;
        m_coords[3] = 1.0;
    }

    double x() const {return m_coords[0];}
    double y() const {return m_coords[1];}
    double z() const {return m_coords[2];}
    double w() const {return m_coords[3]; }

    static double dot(const Vector3D& v1, const Vector3D& v2) {
        return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
    }

    Vector3D operator+(const Vector3D& other) const
    {
        return Vector3D(
            m_coords[0] + other.m_coords[0],
            m_coords[1] + other.m_coords[1],
            m_coords[2] + other.m_coords[2]
        );
    }

    Vector3D operator-(const Vector3D& other) const
    {
        return Vector3D(
            m_coords[0] - other.m_coords[0],
            m_coords[1] - other.m_coords[1],
            m_coords[2] - other.m_coords[2]
        );
    }

    Vector3D operator*(double scalar) const
    {
        return Vector3D(
            m_coords[0] * scalar,
            m_coords[1] * scalar,
            m_coords[2] * scalar
        );
    }

    Vector3D operator/(double scalar) const
    {
        if (scalar == 0) return Vector3D();
        return Vector3D(
            m_coords[0] / scalar,
            m_coords[1] / scalar,
            m_coords[2] / scalar
        );
    }

    Vector3D& operator*=(const Matrix4x4& m);

    Vector3D& operator+=(const Vector3D& other);

    static Vector3D cross(const Vector3D& v1, const Vector3D& v2) {
        return Vector3D(
            v1.y() * v2.z() - v1.z() * v2.y(),
            v1.z() * v2.x() - v1.x() * v2.z(),
            v1.x() * v2.y() - v1.y() * v2.x()
        );
    }

    static double length(const Vector3D& v) {
        return std::sqrt(dot(v, v));
    }

    static Vector3D normalize(const Vector3D& v) {
        double l = length(v);
        if (l == 0.0) return Vector3D(0, 0, 0);
        return Vector3D(v.x() / l, v.y() / l, v.z() / l);
    }

private:
    double m_coords[4];
};


#endif //CG_VECTOR3D_H