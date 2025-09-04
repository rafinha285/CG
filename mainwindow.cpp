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

    auto white = Color{255, 255, 255};

    Point p1{0,0,white},p2{0,100,white},p3{100,100,white},p4{100, 0,white};

    Line line1{p1,p2},line2{p2,p3},line3{p3,p4},line4{p4,p1};

    auto *quadrado = new Polygon();

    quadrado->addLines(std::vector{line1,line2,line3,line4});

    // this->graphicsFrame->addShape(
    //     std::make_unique<Polygon>(*quadrado)
    // );

    // quadrado->scale(10,10);
    Point simple = quadrado->calculateMiddle();
    simple.setColor({0,255,255});
    // quadrado->translateToOrigin();
    this->graphicsFrame->addShape(
        std::make_unique<Point>(simple)
    );
    this->graphicsFrame->addShape(
        std::make_unique<Polygon>(*quadrado)
    );
    // quadrado->scale(2,2);
    quadrado->scale(1.2,1.2);
    Point simple2 = quadrado->calculateMiddle();
    simple2.setColor({0,255,255});
    this->graphicsFrame->addShape(
        std::make_unique<Point>(simple2)
    );
    this->graphicsFrame->addShape(
        std::make_unique<Polygon>(*quadrado)
    );
    // free(quadrado);
}

MainWindow::~MainWindow()
{
    delete ui;
}
