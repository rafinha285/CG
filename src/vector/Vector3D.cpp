//
// Created by abacate on 02/12/2025.
//

#include "Vector3D.h"
#include "../matrix/Matrix4x4.h"

Vector3D& Vector3D::operator*=(const Matrix4x4& m)
{
    *this = m * (*this);
    return *this;
}

Vector3D& Vector3D::operator+=(const Vector3D& other)
{
    *this = *this + other;
    return *this;
}
