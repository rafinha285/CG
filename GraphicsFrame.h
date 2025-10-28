//
// Created by abacate on 28/08/2025.
//

#ifndef CG_GRAPHICSFRAME_H
#define CG_GRAPHICSFRAME_H
#include <QFrame>

#include "src/2d/shapes/Shape.h"
#include "src/3d/shapes/Shape3D.h"


class GraphicsFrame : public QFrame
{
    Q_OBJECT

    public:
        explicit GraphicsFrame(QWidget *parent = nullptr);
        void addShape(std::unique_ptr<::Shape3D> shape);
        void startAnimation();

    protected:
        void paintEvent(QPaintEvent *event) override;
    private:
        QTimer* m_timer;
        double m_cameraAngle = 0.0;
        std::vector<std::unique_ptr<::Shape3D>> displayFile;
};


#endif //CG_GRAPHICSFRAME_H