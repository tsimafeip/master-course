#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTextStream>
#include <QMouseEvent>
#include <ctime>
#include <iostream>
#include <vector>
#include "predicates.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int COUNT = 10000;
    int HEIGHT = 150;
    bool rectangleReady;
    bool isIn;

protected:
    void redrawPixmap();
    bool Cross(double, double);
    void mousePressEvent(QMouseEvent *);
    bool eventFilter(QObject *, QEvent *);
    void paintEvent(QPaintEvent *);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    std::vector<QPoint> polygon;
    QPainter painter;
    QPixmap mPix;
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
