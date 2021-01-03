#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTextStream>
#include <QMouseEvent>
#include <ctime>
#include <iostream>
#include <vector>
#include <cmath>
#include "predicates.h"
#include <math.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int COUNT;
    int HEIGHT = 100;
    QPoint NULL_POINT = QPoint(-1, -1);

    bool pointsGenerated;
    bool pointsDrown;

    bool hullBuilt;
    bool hullDrown;

protected:
    void redrawPixmap();
    void paintEvent(QPaintEvent *event);
    QVector<QPoint> generatePointsRectangle(int count, int minX, int minY, int maxX, int maxY);
    QVector<QPoint> generatePointsCircle(int count, int centerX, int centerY, int radius);
    QVector<QPoint> leftConvexHull(QVector<QPoint> local_s, QPoint local_pl, QPoint local_pr);
    QVector<QPoint> quickHull();
    QVector<QPoint> getPointsOnTheSpecificSide(QVector<QPoint> local_s, QPoint local_pl, QPoint local_pr, int side);
    QPoint getTheMostFarPoint(QVector<QPoint> local_s, QPoint local_pl, QPoint local_pr);
    void preprocess(bool full_mode);

private slots:
    void on_pushButton_clicked();
    void on_pushButton2_clicked();

private:
    QPainter painter;
    QPixmap mPix;
    QVector<QPoint> points;
    QVector<QPoint> S;
    QPoint pl;
    QPoint pr;
    QPoint pb;
    QPoint pt;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
