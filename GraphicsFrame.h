//
// Created by abacate on 28/08/2025.
//

#ifndef CG_GRAPHICSFRAME_H
#define CG_GRAPHICSFRAME_H
#include <QFrame>

#include "Camera.h"
#include "src/shapes/Shape.h"
#include "src/shapes/Shape3D.h"


class GraphicsFrame : public QFrame
{
    Q_OBJECT


    public:
        Camera m_camera;
        explicit GraphicsFrame(QWidget *parent = nullptr);
        void addShape(std::unique_ptr<::Shape3D> shape);
        double speed = 10;
        double sensitivity = 0.01;
        void setSelectedObject(int index);

        void translateSelected(double x, double y, double z);
        void rotateSelected(double angle, char axis);
        void scaleSelected(double x, double y, double z);

        void setProjection(bool projection);

        void deleteSelected();

        void resetCamera();

        void changeColor(Color color);

        Shape3D *getSelectedShape() const;

    signals:
        void objectAdded(const QString &name);

    protected:
        void paintEvent(QPaintEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void wheelEvent(QWheelEvent *event) override;
    private:
    Vector3D getAverageCenter() const;
        bool m_isRightMouseDown;
        bool m_isMiddleMouseDown;
        QPoint m_lastMousePos;
        int m_selectedObjectIndex = -1;

        std::vector<std::unique_ptr<::Shape3D>> displayFile;
};


#endif //CG_GRAPHICSFRAME_H