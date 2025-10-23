//
// Created by abacate on 23/10/2025.
//

#ifndef CG_VECTOR2D_H
#define CG_VECTOR2D_H

class Matrix3x3;

class Vector2D
{
    friend class Matrix3x3;
public:
    ~Vector2D() = default;
    Vector2D()
    {
        m_coords[0] = 0.0;
        m_coords[1] = 0.0;
        m_coords[2] = 1.0;
    }
    Vector2D(const double x, const double y)
    {
        m_coords[0] = x;
        m_coords[1] = y;
        m_coords[2] = 1.0;
    }

    double x() const { return m_coords[0]; }
    double y() const { return m_coords[1]; }

private:
    double m_coords[3];
};


#endif //CG_VECTOR2D_H