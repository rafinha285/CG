#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "GraphicsFrame.h"
#include "src/2d/shapes/Line.h"
#include "src/2d/shapes/Square.h"
#include "src/3d/shapes/Line3D.h"
#include "src/3d/shapes/Point3D.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

#include "src/objreader/ObjLoader.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    graphicsFrame = new GraphicsFrame(this);
    setCentralWidget(graphicsFrame);

    connect(ui->actionImport, &QAction::triggered, this, &MainWindow::handleImport);

    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::handleExit);
    // Color white = {255, 255, 255};
    //
    // Point3D v0_bll(-100, -100, -100);
    // Point3D v1_blr( 100, -100, -100); // Vértice 1: back-bottom-right
    // Point3D v2_btr( 100,  100, -100); // Vértice 2: back-top-right
    // Point3D v3_btl(-100,  100, -100); // Vértice 3: back-top-left
    // Point3D v4_fll(-100, -100,  100); // Vértice 4: front-bottom-left (frente-baixo-esquerda)
    // Point3D v5_flr( 100, -100,  100); // Vértice 5: front-bottom-right
    // Point3D v6_ftr( 100,  100,  100); // Vértice 6: front-top-right
    // Point3D v7_ftl(-100,  100,  100); // Vértice 7: front-top-left
    //
    // // --- 2. Adicione as 12 arestas (linhas) ao graphicsFrame ---
    //
    // // Face de trás (4 linhas)
    // graphicsFrame->addShape(std::make_unique<Line3D>(v0_bll, v1_blr, white));
    // graphicsFrame->addShape(std::make_unique<Line3D>(v1_blr, v2_btr, white));
    // graphicsFrame->addShape(std::make_unique<Line3D>(v2_btr, v3_btl, white));
    // graphicsFrame->addShape(std::make_unique<Line3D>(v3_btl, v0_bll, white));
    //
    // // Face da frente (4 linhas)
    // graphicsFrame->addShape(std::make_unique<Line3D>(v4_fll, v5_flr, white));
    // graphicsFrame->addShape(std::make_unique<Line3D>(v5_flr, v6_ftr, white));
    // graphicsFrame->addShape(std::make_unique<Line3D>(v6_ftr, v7_ftl, white));
    // graphicsFrame->addShape(std::make_unique<Line3D>(v7_ftl, v4_fll, white));
    //
    // // Arestas que conectam as faces (4 linhas)
    // graphicsFrame->addShape(std::make_unique<Line3D>(v0_bll, v4_fll, white)); // Conecta back-left-bottom com front-left-bottom
    // graphicsFrame->addShape(std::make_unique<Line3D>(v1_blr, v5_flr, white)); // Conecta back-right-bottom com front-right-bottom
    // graphicsFrame->addShape(std::make_unique<Line3D>(v2_btr, v6_ftr, white)); // Conecta back-right-top com front-right-top
    // graphicsFrame->addShape(std::make_unique<Line3D>(v3_btl, v7_ftl, white));
}

void MainWindow::handleExit()
{
    close();
}

void MainWindow::handleImport()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Importar arquivo"), QDir::homePath(), tr("Todos os Arquivos(*, *)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("Erro"),
            tr("Não foi possivel abrir o arquivo: %1").arg(file.errorString()));
        return;
    }
    QTextStream in(&file);

    ObjLoader::translateValues(&in, graphicsFrame);

    file.close();


}

MainWindow::~MainWindow()
{
    delete ui;
}
