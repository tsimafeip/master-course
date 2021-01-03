#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QPainter>
#include <QTextStream>
#include <QMouseEvent>
#include <ctime>
#include <iostream>
#include <vector>

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
    bool segmentsGenerated;

protected:
    void redrawPixmap();
    void paintEvent(QPaintEvent *);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QPainter painter;
    QPixmap mPix;
};
#endif // MAINWINDOW_H
