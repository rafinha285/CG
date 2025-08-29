//
// Created by abacate on 28/08/2025.
//

#ifndef CG_GRAPHICSFRAME_H
#define CG_GRAPHICSFRAME_H
#include <QFrame>

#include "Shape.h"


class GraphicsFrame : public QFrame
{
    Q_OBJECT

    public:
        explicit GraphicsFrame(QWidget *parent = nullptr);
        void addShape(std::unique_ptr<::Shape> shape);

    protected:
        void paintEvent(QPaintEvent *event) override;
    private:
        std::vector<std::unique_ptr<::Shape>> displayFile;
};


#endif //CG_GRAPHICSFRAME_H