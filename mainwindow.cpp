#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "GraphicsFrame.h"
#include "src/shapes/Line.h"
#include "src/shapes/Square.h"
#include "src/shapes/Line3D.h"
#include "src/shapes/Point3D.h"
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
