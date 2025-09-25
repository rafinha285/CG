//
// Created by abacate on 28/08/2025.
//

#include "GraphicsFrame.h"
#include "Shape.h"

GraphicsFrame::GraphicsFrame(QWidget *parent)
    : QFrame(parent)
{
    setFrameShape(QFrame::StyledPanel);
}

void GraphicsFrame::addShape(std::unique_ptr<::Shape> shape)
{
    displayFile.push_back(std::move(shape));
    update();
}

void GraphicsFrame::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);

    Window myWindow(-400, -400, 400, 400);
    Viewport myViewport(0, 0, width(), height());

    double quadradoX = -200;
    double quadradoY = -200;
    double lado = 200;
    Color corVermelha = {255, 0, 0};

    Square square(quadradoX, quadradoY, lado, corVermelha);
    addShape(std::make_unique<Square>(square));

    Matrix3x3 viewportMatrix = Matrix3x3::createViewportMatrix(myWindow, myViewport);
    // painter.setRenderHint(QPainter::Antialiasing);

    for (const auto& shape : displayFile)
    {
        if (shape)
        {
            shape->draw(painter, viewportMatrix);
        }
    }
}