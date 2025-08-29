//
// Created by abacate on 28/08/2025.
//

#include "GraphicsFrame.h"

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
    // painter.setRenderHint(QPainter::Antialiasing);

    for (const auto& shape : displayFile)
    {
        if (shape)
        {
            shape->draw(painter);
        }
    }
}