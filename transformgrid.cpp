#include "transformgrid.h"
#include "ui_transformgrid.h"

#include <array>

QGridLayout *TransformGrid::gridLayout()
{ return static_cast<QGridLayout *>(layout()); }

void TransformGrid::updateTransform()
{
    QGridLayout *gl = gridLayout();

    std::array<int,9> m;

    int k=0;

    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            m[k] = static_cast<QLabel *>(gl->itemAtPosition(i,j)->widget())->text().toInt();
            k++;
        }
        k++;
    }

    TRANSFORM = QTransform(m[0],m[1],m[2],m[3],m[4],m[5],m[6],m[7],m[8]);
}

TransformGrid::TransformGrid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransformGrid)
{
    TRANSFORM = QTransform();
    ui->setupUi(this);
}

// I could simplify all this with a set(int i, int j, int val) method.
void TransformGrid::setM11(double val)
{ui->m11->setNum(val); updateTransform(); emit transformChanged(TRANSFORM);}
void TransformGrid::setM12(double val)
{ui->m12->setNum(val); updateTransform(); emit transformChanged(TRANSFORM);}
void TransformGrid::setM13(double val)
{ui->m13->setNum(val); updateTransform(); emit transformChanged(TRANSFORM);}

void TransformGrid::setM21(double val)
{ui->m21->setNum(val); updateTransform(); emit transformChanged(TRANSFORM);}
void TransformGrid::setM22(double val)
{ui->m22->setNum(val); updateTransform(); emit transformChanged(TRANSFORM);}
void TransformGrid::setM23(double val)
{ui->m23->setNum(val); updateTransform(); emit transformChanged(TRANSFORM);}

void TransformGrid::setM31(double val)
{ui->m31->setNum(val); updateTransform(); emit transformChanged(TRANSFORM);}
void TransformGrid::setM32(double val)
{ui->m32->setNum(val); updateTransform(); emit transformChanged(TRANSFORM);}
void TransformGrid::setM33(double val)
{ui->m33->setNum(val); updateTransform(); emit transformChanged(TRANSFORM);}

void TransformGrid::setTransform(QTransform transform)
{
    TRANSFORM = transform;

    ui->m11->setNum(transform.m11());
    ui->m12->setNum(transform.m12());
    ui->m13->setNum(transform.m13());

    ui->m21->setNum(transform.m21());
    ui->m22->setNum(transform.m22());
    ui->m23->setNum(transform.m23());

    ui->m31->setNum(transform.m31());
    ui->m32->setNum(transform.m32());
    ui->m33->setNum(transform.m33());
}

QTransform TransformGrid::transform()
{
    return TRANSFORM;
}

TransformGrid::~TransformGrid()
{
    delete ui;
}
