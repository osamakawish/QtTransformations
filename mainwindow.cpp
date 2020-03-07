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

void MainWindow::setStep(qreal diff)
{
    ui->xSpinBox->setSingleStep(diff);
    ui->ySpinBox->setSingleStep(diff);
}

void MainWindow::setMax(qreal max)
{
    QDoubleSpinBox *x = ui->xSpinBox, *y = ui->ySpinBox;

    x->setRange(-max,max); y->setRange(-max,max);
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

void MainWindow::apply(bool)
{
    // Create new drawing with appropriate previous transform and shape.
    Drawing *dwg = drawings.last();
    dwg = new Drawing(ui->shapeComboBox->currentIndex(),dwg->finalTransform(),dwg->origin());
    drawings.append(dwg);
    scene->addItem(dwg);
    scene->update();

    ui->beforeGrid->setTransform(ui->afterGrid->transform());
    ui->transformComboBox->setCurrentIndex(1);
    ui->xSpinBox->setValue(0); ui->ySpinBox->setValue(0);
    ui->currentGrid->setTransform(QTransform());
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

    QDoubleSpinBox *x = ui->xSpinBox, *y = ui->ySpinBox, *z = ui->zSpinBox;

    y->setDisabled(p < 2); z->setDisabled(p < 3);

    QTransform id;

    switch (i) {
    case 1: x->setValue(id.dx()); y->setValue(id.dy()); break;
    case 2: x->setValue(id.m11()); y->setValue(id.m22()); break;
    case 3: x->setValue(id.m21()); y->setValue(id.m12()); break;
    case 4: x->setValue(id.m13()); y->setValue(id.m23()); z->setValue(id.m33()); break;
    }

    if (i < 2) { setStep(10); setMax(999.999); }
    else if (i < 4) { setStep(0.1); setMax(9.99); }
    else { setStep(0.001); setMax(0.99); }

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

    QPointF o = current->origin();
    ui->finalOriginLabel->setText("(" + QString::number(o.x()) + ", " + QString::number(o.y()) + ")");

    scene->update();
}

void MainWindow::debug()
{
    auto it = drawings.begin();
    while (it != drawings.end()) {
        qDebug() << *it << (*it)->origin() << (*it)->transform(); it++;
    }
    qDebug() << endl;
}

