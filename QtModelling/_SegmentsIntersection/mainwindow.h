#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTextStream>
#include <QMouseEvent>
#include <ctime>
#include <iostream>
#include <vector>
#include <set>
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

    int COUNT;
    int HEIGHT = 100;
    bool segmentsGenerated, segmentsDrown, segmentsIntersected, segmentsIntersectedDrown;

protected:
    void redrawPixmap();
    void paintEvent(QPaintEvent *);
    std::vector<QLine> generateSegments(int maxX, int maxY, int maxLen);
    //std::map<int, std::vector<int>> orderedXValues;
    std::multimap<int, int> orderedXValues;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    std::vector<QLine> segments;
    QPainter painter;
    QPixmap mPix;
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
