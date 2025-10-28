//
// Created by abacate on 28/08/2025.
//

#include "GraphicsFrame.h"
#include "src/2d/shapes/Square.h"
#include "src/2d/shapes/Shape.h"
#include "src/2d/viewport/Viewport.h"
#include "src/2d/window/Window.h"
#include "src/3d/matrix/Matrix4x4.h"
#include "QTimer"

GraphicsFrame::GraphicsFrame(QWidget *parent)
    : QFrame(parent)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &GraphicsFrame::startAnimation);
    m_timer->start(16);
    setFrameShape(QFrame::StyledPanel);
}

void GraphicsFrame::startAnimation()
{
    m_cameraAngle += 1.0; // Incrementa o ângulo a cada frame
    if (m_cameraAngle > 360.0) m_cameraAngle -= 360.0;

    update(); // Força o Qt a chamar o paintEvent()
}

void GraphicsFrame::addShape(std::unique_ptr<::Shape3D> shape)
{
    displayFile.push_back(std::move(shape));
    update();
}

void GraphicsFrame::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);
    Viewport myViewport(0, 0, width(), width());

    Window3D myWorldWindow(-400, -400, -400, 400, 400, 400);

    double radius = 1000.0; // Distância da câmera
    double angleRad = m_cameraAngle * M_PI / 180.0;

    // Calcula a posição da câmera em um círculo
    double camX = radius * std::sin(angleRad);
    double camY = 300.0; // Posição Y fixa (olhando levemente de cima)
    // double camZ = radius * std::cos(angleRad);
    double camZ = radius * std::cos(angleRad);

    Vector3D cameraPos(camX, camY, camZ);
    Vector3D target(0, 0, 0); // Olhando para o centro
    Vector3D up(0, 1, 0);     // "Cima" é o eixo Y positivo

    Matrix4x4 viewMatrix = Matrix4x4::createLookAt(cameraPos, target, up);

    Matrix4x4 projectionMatrix = Matrix4x4::createOrthographic(myWorldWindow);
    Matrix4x4 viewportMatrix = Matrix4x4::createViewport(myViewport);

    Matrix4x4 finalTransform = viewportMatrix * projectionMatrix * viewMatrix;


    for (const auto& shape : displayFile)
    {
        if (shape)
        {
            shape->draw(painter, finalTransform);
        }
    }
}