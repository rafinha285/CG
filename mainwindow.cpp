#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "GraphicsFrame.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    graphicsFrame = new GraphicsFrame(this);
    setCentralWidget(graphicsFrame);

    // this->graphicsFrame->addShape(
    //     std::make_unique<PointClass>(
    //         Point{100,100},
    //         Color{255,255,255}
    //     )
    // );
    //
    // this->graphicsFrame->addShape(
    //     std::make_unique<LineClass>(
    //         Point{120,120},
    //         Point{200,200},
    //         Color{255,255,0}
    //     )
    // );

    Point p1{100,100},p2{100,200},p3{200,100},p4{200,200};

    this->graphicsFrame->addShape(
        std::make_unique<LineClass>(
            p1,p2,Color{255,255,255}
        )
    );
    this->graphicsFrame->addShape(
        std::make_unique<LineClass>(
            p2,p4,Color{255,255,255}
        )
    );
    this->graphicsFrame->addShape(
        std::make_unique<LineClass>(
            p4,p3,Color{255,255,255}
        )
    );
    this->graphicsFrame->addShape(
        std::make_unique<LineClass>(
            p3,p1,Color{255,255,255}
        )
    );
}

MainWindow::~MainWindow()
{
    delete ui;
}
