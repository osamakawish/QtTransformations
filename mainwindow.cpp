#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drawing.h"
#include <QDebug>

void MainWindow::setupConnections()
{
    auto connection = Qt::ConnectionType::DirectConnection;

    // Change transform type based on combo box.
    connect(ui->shapeComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,&MainWindow::setDrawingImage,connection);
    connect(ui->transformComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,&MainWindow::setTransformType,connection);
    connect(ui->applyButton,&QPushButton::clicked,
            this,&MainWindow::apply,connection);

    connect(ui->xSpinBox,static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
            this,&MainWindow::setX,connection);
    connect(ui->ySpinBox,static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
            this,&MainWindow::setY,connection);
    connect(ui->zSpinBox,static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
            this,&MainWindow::setZ,connection);
    // Implement list view

}

void MainWindow::setupVariables()
{
    scene = new QGraphicsScene;
    ui->graphicsViewFinal->setScene(scene);
    drawings.append(new Drawing);
    scene->addItem(drawings[0]);
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
{ delete ui; }

void MainWindow::apply(bool clicked)
{
    if (clicked)
    {
        ui->beforeGrid->setTransform(ui->afterGrid->transform());
        ui->transformComboBox->setCurrentIndex(1);
        ui->currentGrid->setTransform(QTransform());

        // Create new drawing with appropriate previous transform and shape.
        drawings.append(new Drawing(ui->shapeComboBox->currentIndex(),drawings.last()->finalTransform()));
        scene->addItem(drawings.last());
        scene->update();
    }
}

void MainWindow::setDrawingImage(int i)
{
    auto it = drawings.begin();
    while (it != drawings.end()) { (*it)->setDrawingImage(i); }
    scene->update();
}

void MainWindow::setTransformType(int i)
{
    int p = drawings.last()->setTransformType(i);
    ui->ySpinBox->setDisabled(p < 2); ui->zSpinBox->setDisabled(p < 3);
    scene->update();
}

void MainWindow::setX(qreal x)
{ setInputs(x,ui->ySpinBox->value(),ui->zSpinBox->value()); }

void MainWindow::setY(qreal y)
{ setInputs(ui->xSpinBox->value(),y,ui->zSpinBox->value()); }

void MainWindow::setZ(qreal z)
{ setInputs(ui->xSpinBox->value(),ui->ySpinBox->value(),z); }

void MainWindow::setInputs(qreal x, qreal y, qreal z)
{
    Drawing *current = drawings.last();
    current->setValues(x,y,z);

    ui->currentGrid->setTransform(current->currentTransform());
    ui->afterGrid->setTransform(current->finalTransform());

    scene->update();
}

