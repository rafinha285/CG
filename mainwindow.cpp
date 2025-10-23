#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "GraphicsFrame.h"
#include "src/shapes/Line.h"
#include "src/shapes/Square.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    graphicsFrame = new GraphicsFrame(this);
    setCentralWidget(graphicsFrame);

    Color white = {255, 255, 255};

    int x_centro = 100; // Centro do corpo
    int y_base_cabeca = 100; // Onde o pescoço encontra a cabeça
    int tam_cabeca = 20;
    int alt_pescoco = 10;
    int alt_tronco = 40;
    int alt_pernas = 30;
    int larg_bracos = 20;

    Polygon stickMan = Polygon();

    // --- 1. Cabeça Quadrada ---
    // (x_centro - 10, y_base_cabeca) até (x_centro + 10, y_base_cabeca + 20)
    int cabeca_x_esq = x_centro - (tam_cabeca / 2);
    int cabeca_y_baixo = y_base_cabeca;
    int cabeca_x_dir = x_centro + (tam_cabeca / 2);
    int cabeca_y_cima = y_base_cabeca + tam_cabeca;

    // Linha de baixo da cabeça
    stickMan.addLine(Line(cabeca_x_esq, cabeca_y_baixo, cabeca_x_dir, cabeca_y_baixo, white));
    // Linha da direita
    stickMan.addLine(Line(cabeca_x_dir, cabeca_y_baixo, cabeca_x_dir, cabeca_y_cima, white));
    // Linha de cima
    stickMan.addLine(Line(cabeca_x_dir, cabeca_y_cima, cabeca_x_esq, cabeca_y_cima, white));
    // Linha da esquerda
    stickMan.addLine(Line(cabeca_x_esq, cabeca_y_cima, cabeca_x_esq, cabeca_y_baixo, white));


    // --- 2. Corpo (Pescoço e Tronco) ---
    int y_ombros = y_base_cabeca - alt_pescoco;
    int y_cintura = y_ombros - alt_tronco;

    // Pescoço (do centro da cabeça para baixo)
    stickMan.addLine(Line(x_centro, y_base_cabeca, x_centro, y_ombros, white));
    // Tronco (do pescoço para baixo)
    stickMan.addLine(Line(x_centro, y_ombros, x_centro, y_cintura, white));


    // --- 3. Braços ---
    // Saindo dos "ombros" (x_centro, y_ombros)
    stickMan.addLine(Line(x_centro, y_ombros, x_centro - larg_bracos, y_ombros - 10, white)); // Braço esquerdo
    stickMan.addLine(Line(x_centro, y_ombros, x_centro + larg_bracos, y_ombros - 10, white)); // Braço direito


    // --- 4. Pernas ---
    // Saindo da "cintura" (x_centro, y_cintura)
    int y_pes = y_cintura - alt_pernas;
    stickMan.addLine(Line(x_centro, y_cintura, x_centro - 15, y_pes, white)); // Perna esquerda
    stickMan.addLine(Line(x_centro, y_cintura, x_centro + 15, y_pes, white));

    stickMan.rotate(180);

    graphicsFrame->addShape(std::make_unique<Polygon>(stickMan));
}

MainWindow::~MainWindow()
{
    delete ui;
}
