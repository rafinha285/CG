//
// Created by abacate on 28/08/2025.
//

#include "GraphicsFrame.h"
#include "aShape.h"
#include "src/shapes/Square.h"

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

    Window myWindow(0, 0, 200, 200);
    Viewport myViewport(0, 0, width(), width());

    int margin = 0;

    double x_min = myWindow.min.x() + margin; // -350.0
    double y_min = myWindow.min.y() + margin; // -350.0
    double side = (myWindow.max.x() - myWindow.min.x()) - (2.0 * margin); // 700.0

    Square mySquare((int)x_min, (int)y_min, (int)side, {255,255,255});
    addShape(std::make_unique<Square>(mySquare));

    Matrix3x3 viewportMatrix = Matrix3x3::createViewportMatrix(myWindow, myViewport);
    // painter.setRenderHint(QPainter::Antialiasing);

    for (const auto& shape : displayFile)
    {
        if (shape)
        {
            shape->draw(painter, viewportMatrix, &myWindow);
        }
    }
}