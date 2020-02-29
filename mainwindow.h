#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Drawing;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    void setupConnections();
    void setupVariables();

    // Private Variables
    Drawing *MAIN_DRAWING;
    Drawing *PREVIEW;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setTransformType(int i);

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
