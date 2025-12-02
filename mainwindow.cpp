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

    //color
    redValue = ui->redValue;
    greenValue = ui->greenValue;
    blueValue = ui->blueValue;


    if (ui->container3D){
        QVBoxLayout *layout = new QVBoxLayout(ui->container3D);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(graphicsFrame);
    }
    connect(ui->actionImport, &QAction::triggered, this, &MainWindow::handleImport);

    //translate button
    connect(ui->translationButton, &QPushButton::clicked, this, [=]()
    {
        double x = ui->xTranslationValue->value();
        double y = ui->yTranslationValue->value();
        double z = ui->zTranslationValue->value();

        graphicsFrame->translateSelected(x,y,z);
    });

    //scale button
    connect(ui->scaleButton, &QPushButton::clicked, this, [=]()
    {
        double x = ui->xScaleValue->value();
        double y = ui->yScaleValue->value();
        double z = ui->zScaleValue->value();
        graphicsFrame->scaleSelected(x,y,z);
        ui->xScaleValue->setValue(1);
        ui->yScaleValue->setValue(1);
        ui->zScaleValue->setValue(1);
    });

    //rotate button
    connect(ui->rotateButton, &QPushButton::clicked, this, [=]()
    {
        double angX = ui->xRotationValue->value();
        double angY = ui->yRotationValue->value();
        double angZ = ui->zRotationValue->value();

        if (angX != 0.0)
        {
            graphicsFrame->rotateSelected(angX, 'X');
        }

        if (angY != 0.0)
        {
            graphicsFrame->rotateSelected(angY, 'Y');
        }
        if (angZ != 0.0)
        {
            graphicsFrame->rotateSelected(angZ, 'Z');
        }
    });

    //reset camera
    if (ui->resetCameraButton) {
        connect(ui->resetCameraButton, &QPushButton::clicked, this, [=]()
        {
            graphicsFrame->resetCamera();
        });
    }

    // delete button
    connect(ui->deleteButton, &QPushButton::clicked, this, [=]()
    {
        int currentRow = ui->mainObjectList->currentRow();
        if (currentRow >= 0)
        {
            graphicsFrame->deleteSelected();
            ui->mainObjectList->takeItem(currentRow);
        }
    });

    connect(ui->perspectiveRadioButton, &QRadioButton::toggled, this, [=](bool checked)
    {
        if (checked)
        {
            graphicsFrame->setProjection(true);
        }
    });

    connect(ui->orthogonalRadioButton, &QRadioButton::toggled, this, [=](bool checked)
    {
        if (checked)
        {
            graphicsFrame->setProjection(false);
        }
    });

    ui->perspectiveRadioButton->setChecked(true);
    graphicsFrame->setProjection(true);

    connect(ui->cameraSpeedValue, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [=](double value)
    {
        graphicsFrame->speed = value;
    });

    ui->cameraSpeedValue->setValue(graphicsFrame->speed);

    connect(graphicsFrame, &GraphicsFrame::objectAdded, this, [=](const QString &name)
    {
        ui->mainObjectList->addItem(name);
    });

    connect(ui->sensibilityValue, &QDoubleSpinBox::valueChanged, this, [=](double value)
    {
        graphicsFrame->sensitivity = value;
    });

    ui->sensibilityValue->setValue(graphicsFrame->sensitivity);

    connect(ui->mainObjectList, &QListWidget::currentRowChanged,graphicsFrame, [=](int index)
    {
        printf("%d",index);
        graphicsFrame->setSelectedObject(index);
        if (index >= 0)
        {
            auto color = graphicsFrame->getSelectedShape()->getColor();
            redValue->setValue(color.r);
            greenValue->setValue(color.g);
            blueValue->setValue(color.b);
        }
    });


    connect(ui->redValue, &QSpinBox::valueChanged, this, [=](int value)
    {
        changeColor();
    });
    connect(ui->greenValue, &QSpinBox::valueChanged, this, [=](int value)
    {
        changeColor();
    });
    connect(ui->blueValue, &QSpinBox::valueChanged, this, [=](int value)
    {
        changeColor();
    });



    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::handleExit);
}
void MainWindow::changeColor() const
{
    graphicsFrame->changeColor({redValue->value(), greenValue->value(), blueValue->value()});
};

void MainWindow::handleExit()
{
    close();
}

void MainWindow::handleImport()
{
    const QString fileName = QFileDialog::getOpenFileName(this,
        tr("Importar arquivo"), QDir::homePath(), tr("Todos os Arquivos(*, *)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("Erro"),
            tr("Não foi possivel abrir o arquivo: %1").arg(file.errorString()));
        return;
    }
    QTextStream in(&file);

    const QFileInfo fileInfo(fileName);

    const std::string objectName = fileInfo.baseName().toStdString();

    ObjLoader::translateValues(&in, graphicsFrame, objectName);

    file.close();
}

MainWindow::~MainWindow()
{

    delete ui;
}
