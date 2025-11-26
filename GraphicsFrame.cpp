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
    setFrameShape(QFrame::StyledPanel);

    setFocusPolicy(Qt::StrongFocus);
}


void GraphicsFrame::addShape(std::unique_ptr<::Shape3D> shape)
{
    displayFile.push_back(std::move(shape));
    update();
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
}

void GraphicsFrame::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        m_isRightMouseDown = true;
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
}

void GraphicsFrame::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isRightMouseDown)
    {
        QPoint currentPos = event->pos();
        int dx = currentPos.x() - m_lastMousePos.x();
        int dy = currentPos.y() - m_lastMousePos.y();

        // Sensibilidade do mouse
        double sensitivity = 0.01;

        // Delegamos a rotação para a classe Camera
        // Invertemos dx/dy conforme gosto pessoal (arrastar cenário vs mover camera)
        m_camera.rotate(dx * sensitivity, dy * sensitivity);

        m_lastMousePos = currentPos;
        update(); // Redesenha a tela
        event->accept();
    }
}

void GraphicsFrame::wheelEvent(QWheelEvent* event)
{
    int delta = event->angleDelta().y();

    // Define a velocidade do zoom
    double zoomSpeed = 20.0;

    // Delegamos o zoom para a classe Camera
    // Se delta > 0 (roda pra frente), aproximamos (negativo no raio ou largura)
    if (delta > 0)
        m_camera.zoom(-zoomSpeed);
    else
        m_camera.zoom(zoomSpeed);

    update(); // Redesenha a tela
    event->accept();
}