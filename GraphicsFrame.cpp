//
// Created by abacate on 28/08/2025.
//

#include "GraphicsFrame.h"
#include "src/shapes/Square.h"
#include "src/shapes/Shape.h"
#include "src/viewport/Viewport.h"
#include "src/window/Window.h"
#include "src/matrix/Matrix4x4.h"
#include "QTimer"
#include <QMouseEvent>

GraphicsFrame::GraphicsFrame(QWidget *parent)
    : QFrame(parent),
    m_camera(800,600)
{
    m_isRightMouseDown = false;
    m_isMiddleMouseDown = false;
    setFrameShape(QFrame::StyledPanel);

    setFocusPolicy(Qt::StrongFocus);
}


void GraphicsFrame::addShape(std::unique_ptr<::Shape3D> shape)
{
    if (shape->getName() == "Objeto Sem Nome" || shape->getName().empty()) {
        shape->setName("Objeto " + std::to_string(displayFile.size() + 1));
    }
    emit objectAdded(QString::fromStdString(shape->getName()));
    displayFile.push_back(std::move(shape));
    update();
}

void GraphicsFrame::setSelectedObject(int index)
{
    if (index >= 0 && index < displayFile.size())
    {
        m_selectedObjectIndex = index;
    }else
    {
        m_selectedObjectIndex = -1;
    }
    update();
}

void GraphicsFrame::deleteSelected()
{
    if (m_selectedObjectIndex >= 0 && m_selectedObjectIndex < displayFile.size())
    {
        displayFile.erase(displayFile.begin() + m_selectedObjectIndex);

        m_selectedObjectIndex = -1;

        update();
    }
}


void GraphicsFrame::translateSelected(double x, double y, double z)
{
    if (m_selectedObjectIndex <0)
    {
        return;
    }
    const auto object = &displayFile[m_selectedObjectIndex];
    object->get()->translate(x,y,z);
    update();
}

void GraphicsFrame::scaleSelected(const double x, const double y,const double z)
{
    if (m_selectedObjectIndex <0)
    {
        return;
    }
    const auto object= &displayFile[m_selectedObjectIndex];
    const auto center = object->get()->getCenter();
    object->get()->scale(x,y,z, center);
    update();
}

void GraphicsFrame::rotateSelected(double angle, char axis)
{
    if (m_selectedObjectIndex != -1 && m_selectedObjectIndex < displayFile.size()) {

        Vector3D center = displayFile[m_selectedObjectIndex]->getCenter();

        if (axis == 'X') {
            displayFile[m_selectedObjectIndex]->rotateX(angle, center);
        }
        else if (axis == 'Y') {
            displayFile[m_selectedObjectIndex]->rotateY(angle, center);
        }
        else if (axis == 'Z') {
            displayFile[m_selectedObjectIndex]->rotateZ(angle, center);
        }

        update();
    }
}

void GraphicsFrame::resetCamera()
{
    printf("Camera resetada");
    if (displayFile.empty()) return;
    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double minY = minX, maxY = maxX;
    double minZ = minX, maxZ = maxX;

    for (const auto& shape : displayFile) {
        shape->updateBounds(minX, maxX, minY, maxY, minZ, maxZ);
    }

    double centerX = (minX + maxX) / 2;
    double centerY = (minY + maxY) / 2;
    double centerZ = (minZ + maxZ) / 2;

    double sizeX = maxX - minX;
    double sizeY = maxY - minY;
    double sizeZ = maxZ - minZ;

    double maxDimension = std::max({sizeX, sizeY, sizeZ});

    if (maxDimension == 0) maxDimension = 100;

    m_camera.m_vrp = Vector3D(centerX, centerY, centerZ);
    m_camera.m_radius = maxDimension * 1.5;
    m_camera.m_orthoWidth = m_camera.m_radius;

    m_camera.m_theta = 0.0;
    m_camera.m_phi = M_PI / 6.0;

    m_camera.updateEyeFromAngles();
    update();
}

void GraphicsFrame::setProjection(bool projection)
{
    if (projection)
    {
        m_camera.m_type = PERSPECTIVE;
    }else
    {
        m_camera.m_type = ORTHOGRAPHIC;
        double fovRad = m_camera.m_fov * M_PI / 180.0;
        m_camera.m_orthoWidth = 2.0 * m_camera.m_radius * std::tan(fovRad / 2.0) * m_camera.m_aspect;
    }
    update();
}


Vector3D GraphicsFrame::getAverageCenter() const
{
    if (displayFile.empty()) {
        return Vector3D(0, 0, 0);
    }

    double sumX = 0.0;
    double sumY = 0.0;
    double sumZ = 0.0;

    for (const auto& object : displayFile)
    {
        Vector3D c = object->getCenter();
        sumX += c.x();
        sumY += c.y();
        sumZ += c.z();
    }

    double count = static_cast<double>(displayFile.size());

    return Vector3D(sumX / count, sumY / count, sumZ / count);
}

void GraphicsFrame::resizeEvent(QResizeEvent *event)
{
    m_camera.updateAspect(width(), height());
    QFrame::resizeEvent(event);
}

void GraphicsFrame::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);

    Viewport viewport(0,0,width(),height());
    Matrix4x4 viewportMatrix = Matrix4x4::createViewport(viewport);

    Matrix4x4 projectionMatrix = m_camera.getProjectionMatrix();
    Matrix4x4 viewMatrix = m_camera.getViewMatrix();

    Matrix4x4 finalTransform = projectionMatrix * viewMatrix;

    Window3D clipWindow = m_camera.getOrthoWindow();

    for (const auto& shape : displayFile)
    {
        if (shape)
        {
            shape->draw(painter, finalTransform, viewportMatrix, &clipWindow);
        }
    }

}

void GraphicsFrame::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_W:
            m_camera.translate(speed, 0, 0); // Frente
            break;
        case Qt::Key_S:
            m_camera.translate(-speed, 0, 0); // Trás
            break;
        case Qt::Key_A:
            m_camera.translate(0, -speed, 0); // Esquerda
            break;
        case Qt::Key_D:
            m_camera.translate(0, speed, 0); // Direita
            break;
        case Qt::Key_Space:
            m_camera.translate(0, 0, speed); // Subir (Eixo Y Global)
            break;
        case Qt::Key_Shift:
            m_camera.translate(0, 0, -speed); // Descer (Eixo Y Global)
            break;
        default:
            QFrame::keyPressEvent(event);
            return;
    }
    update();
}

void GraphicsFrame::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        m_isRightMouseDown = true;
        m_lastMousePos = event->pos();
        event->accept();
    }else if (event->button() == Qt::MiddleButton)
    {
        m_isMiddleMouseDown = true;
        m_lastMousePos = event->pos();
        event->accept();
    }
}

void GraphicsFrame::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        m_isRightMouseDown = false;
        event->accept();
    }
    else if (event->button() == Qt::MiddleButton)
    {
        m_isMiddleMouseDown = false;
        event->accept();
    }
}

void GraphicsFrame::mouseMoveEvent(QMouseEvent *event)
{
    QPoint currentPos = event->pos();
    int dx = currentPos.x() - m_lastMousePos.x();
    int dy = currentPos.y() - m_lastMousePos.y();

    double sensitivity = 0.01;
    if (m_isRightMouseDown)
    {
        m_camera.rotate(-dx * sensitivity, -dy * sensitivity);

        m_lastMousePos = currentPos;
        update();
        event->accept();
    }
    else if (m_isMiddleMouseDown)
    {
        m_camera.rotateAroundSelf(-dx * sensitivity, dy * sensitivity);
        m_lastMousePos = currentPos;
        update();
        event->accept();
    }
}

void GraphicsFrame::wheelEvent(QWheelEvent* event)
{
    int delta = event->angleDelta().y();

    double currentZoomSpeed = this->speed * 2.0;
    if (delta > 0)
        m_camera.zoom(-currentZoomSpeed);
    else
        m_camera.zoom(currentZoomSpeed);

    update(); // Redesenha a tela
    event->accept();
}