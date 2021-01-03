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
    int SIZE = 50;
    int HEIGHT = 150;
    bool mousePressed;
    bool pointsGenerated;
    bool pointsDrown;
    bool rectangleReady;
    QPair<int *, int *> coords;
    size_t selectedCount;

protected:
    void redrawPixmap();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    bool eventFilter(QObject *obj, QEvent *event);
    void paintEvent(QPaintEvent *event);
    //std::vector<QPoint> generatePoints(int count, int maxX, int maxY);
    std::vector<std::vector<std::vector<QPoint>>> generateGrid(int count, int maxX, int maxY);
    std::vector<QPoint> selected;

private slots:
    void on_drawButton_clicked();

private:
    QPainter painter;
    QPixmap mPix;
    QLine mLine;
    QRect mRect, tempRect;
    Ui::MainWindow *ui;
    QPoint startPoint;
    std::vector<std::vector<std::vector<QPoint>>> grid;
};
#endif // MAINWINDOW_H
