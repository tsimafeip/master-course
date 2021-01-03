#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTextStream>
#include <QMouseEvent>
#include <ctime>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <set>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int HEIGHT = 100;
    //int DELTA = 0.001;
    int DELTA = 1;
    //int DELTA = 10000;

protected:
    void redrawPixmap();
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *);
    void updateLAndDeltasOnBuild();
    void updateBVectorGeneral();
    void updateBVectorOnBuild();
    QRect createDRect(QPoint dPoint);

    int indexOfDragStartPoint;
    QRect tempRect;

private:
    std::vector<QPoint> d_points;
    std::vector<QPoint> b_points;
    std::vector<double> deltas;
    int L;

    QPainter painter;
    QPixmap mPix;
    Ui::MainWindow *ui;

private slots:
    void on_pushButton_clicked();
};
#endif // MAINWINDOW_H
