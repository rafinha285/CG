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

    Color white = Color{255, 255, 255};

    Point p1{100,100,white},p2{100,200,white},p3{200,200,white},p4{200,100,white};

    Line line1{p1,p2},line2{p2,p3},line3{p3,p4},line4{p4,p1};


    this->graphicsFrame->addShape(
        std::make_unique<Line>(line1)
    );

    // this->graphicsFrame->addShape(
    //     std::make_unique<Line>(line2)
    // );
    //
    // this->graphicsFrame->addShape(
    //     std::make_unique<Line>(line3)
    // );
    //
    // this->graphicsFrame->addShape(
    //     std::make_unique<Line>(line4)
    // );
    line1.translate(10,10);
    this->graphicsFrame->addShape(
        std::make_unique<Line>(line1)
    );
}

MainWindow::~MainWindow()
{
    delete ui;
}
