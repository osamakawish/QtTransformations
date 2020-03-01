#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "drawing.h"

void MainWindow::setupConnections()
{
    // Change transform type based on combo box.
    connect(ui->shapeComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,&MainWindow::setDrawingImage);
    connect(ui->transformComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,&MainWindow::setTransformType);
    connect(ui->applyButton,&QPushButton::clicked,
            this,[=](bool clicked) {if (clicked) {MAIN_DRAWING->apply();} });
    connect(ui->transformGrid,&TransformGrid::transformChanged,
            this,[=](QTransform t) {PREVIEW->setTransform(t); PREVIEW->apply();});
    // Implement list view

}

void MainWindow::setupVariables()
{
    ui->graphicsViewFinal->setScene(new QGraphicsScene);
    ui->graphicsViewPreview->setScene(new QGraphicsScene);

    MAIN_DRAWING = new Drawing; PREVIEW = new Drawing;

    ui->graphicsViewFinal->scene()->addItem(MAIN_DRAWING);
    ui->graphicsViewPreview->scene()->addItem(PREVIEW);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupConnections();
    setupVariables();
}

MainWindow::~MainWindow()
{
    delete MAIN_DRAWING;
    delete PREVIEW;

    delete ui;
}

void MainWindow::setDrawingImage(int i)
{
    PREVIEW->setDrawingImage(i); MAIN_DRAWING->setDrawingImage(i);
    PREVIEW->apply(); MAIN_DRAWING->apply();
}

void MainWindow::setTransformType(int i)
{
    PREVIEW->setTransformType(i); int p = MAIN_DRAWING->setTransformType(i); PREVIEW->apply();
    ui->zSpinBox->setDisabled(p < 3); ui->ySpinBox->setDisabled(p < 2);
}

