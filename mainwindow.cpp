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

    this->graphicsFrame->addShape(
        std::make_unique<PointClass>(
            Point{100,100},
            Color{255,255,255}
        )
    );

    this->graphicsFrame->addShape(
        std::make_unique<PointClass>(
            Point{50,50},
            Color{255,255,0}
        )
    );
}

MainWindow::~MainWindow()
{
    delete ui;
}
