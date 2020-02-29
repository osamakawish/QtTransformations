#ifndef TRANSFORMGRID_H
#define TRANSFORMGRID_H

#include <QWidget>

class QLabel;
class QGridLayout;

namespace Ui {
class TransformGrid;
}

class TransformGrid : public QWidget
{
    Q_OBJECT

    QGridLayout *gridLayout();
    void updateTransform();

    QTransform TRANSFORM;

signals:
    void transformChanged(QTransform);

public:
    explicit TransformGrid(QWidget *parent = nullptr);

    void setTransform(QTransform transform);

    void setM11(double val);
    void setM12(double val);
    void setM13(double val);

    void setM21(double val);
    void setM22(double val);
    void setM23(double val);

    void setM31(double val);
    void setM32(double val);
    void setM33(double val);

    QTransform transform();

    ~TransformGrid();

private:
    Ui::TransformGrid *ui;
};

#endif // TRANSFORMGRID_H
