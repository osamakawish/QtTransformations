#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

class Drawing;
class QPointF;
class QGraphicsScene;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    void setupConnections();
    void setupVariables();

    QVector<Drawing *> drawings;
    QGraphicsScene *scene;

    void setStep(qreal diff);
    void setMax(qreal max);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void apply(bool clicked);
    void setDrawingImage(int i);
    void setTransformType(int i);

    void setX(qreal x);
    void setY(qreal y);
    void setZ(qreal z);

    void setInputs(qreal x, qreal y, qreal z);
    void debug();

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
