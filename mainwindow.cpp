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
#include <QVBoxLayout>

#include "src/objreader/ObjLoader.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    graphicsFrame = new GraphicsFrame(this);

    if (ui->container3D){
        QVBoxLayout *layout = new QVBoxLayout(ui->container3D);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(graphicsFrame);
    }
    connect(ui->actionImport, &QAction::triggered, this, &MainWindow::handleImport);

    connect(ui->translationButton, &QPushButton::clicked, this, [=]()
    {
        double x = ui->xTranslationValue->value();
        double y = ui->yTranslationValue->value();
        double z = ui->zTranslationValue->value();

        graphicsFrame->translateSelected(x,y,z);
    });

    connect(ui->scaleButton, &QPushButton::clicked, this, [=]()
    {
        double x = ui->xScaleValue->value();
        double y = ui->yScaleValue->value();
        double z = ui->zScaleValue->value();
        graphicsFrame->scaleSelected(x,y,z);
    });

    connect(graphicsFrame, &GraphicsFrame::objectAdded, this, [=](const QString &name)
    {
        ui->mainObjectList->addItem(name);
    });

    connect(ui->mainObjectList, &QListWidget::currentRowChanged,
        graphicsFrame, &GraphicsFrame::setSelectedObject);

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


void MainWindow::handleTranslate()
{
    // GraphicsFrame::
}


MainWindow::~MainWindow()
{

    delete ui;
}
