#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTextStream>
#include <QMouseEvent>
#include <ctime>
#include <iostream>
#include <vector>
#include <QGenericMatrix>
#include <math.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

typedef QGenericMatrix<1, 3, float> QMatrix1x3;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void redrawPixmap();
    void update_edges(QMatrix3x3);
    void paintEvent(QPaintEvent *);

private slots:
    void on_xSlider_actionTriggered(int action);
    void on_ySlider_actionTriggered(int action);
    void on_zSlider_actionTriggered(int action);

private:
    std::vector<std::pair<QMatrix1x3, QMatrix1x3>> edges;
    QPainter painter;
    QPixmap mPix;
    Ui::MainWindow *ui;
    int HEIGHT = 100;
    const double angle = M_PI/4;
};
#endif // MAINWINDOW_H
