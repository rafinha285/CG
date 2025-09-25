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

    Line line = Line(0,-0,200,-200, {255,255,255});

    graphicsFrame->addShape(std::make_unique<Line>(line));
}

MainWindow::~MainWindow()
{
    delete ui;
}
