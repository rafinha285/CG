#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>

#include "GraphicsFrame.h"

struct batata
{
    double X;
    double Y;
    double Z;
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleImport();
    void handleExit();

private:
    void changeColor() const;
    batata translation;
    Ui::MainWindow *ui;
    GraphicsFrame *graphicsFrame;

    QSpinBox *redValue;
    QSpinBox *greenValue;
    QSpinBox *blueValue;
};
#endif // MAINWINDOW_H
