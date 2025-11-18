//
// Created by abacate on 18/11/2025.
//

#ifndef CG_CAMERA_H
#define CG_CAMERA_H
#include <math.h>

#include "matrix/Matrix4x4.h"
#include "vector/Vector3D.h"

enum ProjectionType
{
    ORTHOGRAPHIC,
    PERSPECTIVE
};

class Camera
{
public:
    Vector3D m_cop;
    Vector3D m_vrp;
    Vector3D m_vpn;

    ProjectionType m_type;
    double m_fov;
    double m_aspect;
    double m_near;
    double m_far;

    double m_orthoWidth;

    double m_radius;
    double m_theta;
    double m_phi;

    Camera(double width, double height):
        m_type(PERSPECTIVE),
        m_fov(45.0),
        m_near(0.1),
        m_far(5000.0),
        m_theta(0.0),
        m_phi(M_PI / 6),
        m_radius(600)
    {
        m_aspect = width / height;
        m_orthoWidth = m_radius;

        m_vpn = Vector3D(0, 1, 0);
        m_vrp = Vector3D(0, 0, 0);
    }

    void updateEyeFromAngles() {
        // Impede gimbal lock (virar de ponta cabeça)
        if (m_phi > 89.0 * M_PI / 180.0) m_phi = 89.0 * M_PI / 180.0;
        if (m_phi < -89.0 * M_PI / 180.0) m_phi = -89.0 * M_PI / 180.0;

        double y = m_radius * std::sin(m_phi);
        double r_xz = m_radius * std::cos(m_phi);
        double x = r_xz * std::sin(m_theta);
        double z = r_xz * std::cos(m_theta);

        m_cop = m_vrp + Vector3D(x, y, z);
    }

    Matrix4x4 getViewMatrix() const
    {
        return Matrix4x4::createLookAt(m_cop, m_vrp, m_vpn);
    }
    Matrix4x4 getProjectionMatrix() const
    {
        if (m_type == PERSPECTIVE)
        {
            return Matrix4x4::createPerspective(m_fov, m_aspect, m_near, m_far);
        }else
        {
            double halfW = m_orthoWidth / 2;
            double halfH = halfW / m_aspect;

            Window3D orthoWin(-halfW, -halfH, -m_far, halfW, halfH, m_far);
            return Matrix4x4::createOrthographic(orthoWin);
        }
    }

    void zoom(double delta)
    {
        if (m_type == PERSPECTIVE) {
            m_radius += delta;
            if (m_radius < 1.0) m_radius = 1.0;
            updateEyeFromAngles();
        } else {
            m_orthoWidth += delta;
            if (m_orthoWidth < 1.0) m_orthoWidth = 1.0;
        }
    }

    void rotate(double dTheta, double dPhi) {
        m_theta += dTheta;
        m_phi += dPhi;
        updateEyeFromAngles();
    }

    void updateAspect(double width, double height) {
        if (height == 0) height = 1;
        m_aspect = width / height;
    }

    Window3D getOrthoWindow() const {
        double halfW = m_orthoWidth / 2.0;
        double halfH = halfW / m_aspect;
        return Window3D(-halfW, -halfH, -m_far, halfW, halfH, m_far);
    }

    void toggleProjection() {
        if (m_type == PERSPECTIVE) {
            m_type = ORTHOGRAPHIC;
            // Sincroniza o zoom visual ao trocar
            m_orthoWidth = m_radius;
        } else {
            m_type = PERSPECTIVE;
        }
    }

};

#endif //CG_CAMERA_H