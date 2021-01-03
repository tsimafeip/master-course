#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mPix = QPixmap(this->width(), this->height()-HEIGHT);
    mPix.fill(Qt::white);
    indexOfDragStartPoint = -1;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter tempPainter(&mPix);

    // clean all the drawing field
    mPix.fill(Qt::white);

    //redrawDPoints
    for (size_t i = 0; i < d_points.size(); i++) {
        tempPainter.setPen(QPen(QBrush(Qt::black),2));
        tempPainter.drawRect(createDRect(d_points[i]));
        if(i>0){
            tempPainter.drawLine(d_points[i-1], d_points[i]);
        }
    }

    if (d_points.size() >= 4){
        for (int i = 0; i < L; i++){
            // drawing b points
            tempPainter.setPen(QPen(QBrush(Qt::green), 3));
            for (int j = 0; j < 4; j++){
                QPoint curBPoint = b_points[i*3+j];
                tempPainter.drawEllipse(curBPoint, 3, 3);
            }

            // drawing current spline
            tempPainter.setBrush(QBrush(Qt::red));
            tempPainter.setPen(QPen(Qt::red));
            for(double t = 0; t < 1; t+=0.001){
                QPoint val =
                        pow(1-t, 3)*b_points[i*3] +
                        3*t*pow(1-t, 2)*b_points[i*3+1] +
                        3*t*t*(1-t)*b_points[i*3+2] +
                        t*t*t*b_points[i*3+3];
                tempPainter.drawEllipse(val, 1, 1);
            }
        }
    }

    redrawPixmap();
}

void MainWindow::redrawPixmap() {
    painter.begin(this);
    painter.drawPixmap(0, 0, mPix);
    painter.end();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    // move d point if drag-n-grop started
    if(indexOfDragStartPoint != -1){
        this->setCursor(Qt::ClosedHandCursor);
        d_points[indexOfDragStartPoint] = event->pos();
        updateBVectorGeneral();
        update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    // finish drag-n-grop if started
    if (indexOfDragStartPoint != -1){
        d_points[indexOfDragStartPoint]=event->pos();
        indexOfDragStartPoint = -1;
        updateBVectorGeneral();
        this->setCursor(Qt::ArrowCursor);
        update();
    }
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    auto curPosition = event->pos();
    for (size_t i = 0; i < d_points.size(); i++){
        tempRect = createDRect(d_points[i]);
        // drag-n-drop
        if(tempRect.contains(curPosition)){
            indexOfDragStartPoint = i;
            break;
        }
    }

    // no drag-n-drop, just add a new d point
    if (indexOfDragStartPoint == -1){
        d_points.push_back(curPosition);
        updateBVectorOnBuild();
    }

    update();
}

void MainWindow::updateBVectorOnBuild(){
    if (d_points.size() <= 4){
        // the simpliest case
        b_points.resize(d_points.size());
        for(size_t i = 0; i < d_points.size(); i++){
            b_points[i] = d_points[i];
        }

        if (d_points.size() == 4){
            updateLAndDeltasOnBuild();
        }
    }
    else{
        updateLAndDeltasOnBuild();
        updateBVectorGeneral();
    }
}

void MainWindow::updateBVectorGeneral(){
    // first 3 elements
    b_points[0]=d_points[0];
    b_points[1]=d_points[1];
    if (L != 1){
        b_points[2]=(deltas[1]*d_points[1]+deltas[0]*d_points[2])/(deltas[0]+deltas[1]);
    }

    // cycle
    for(int i = 1; i < L; i++){
        if(i != 1){
            b_points[3*i-1]=(deltas[i]*d_points[i]+(deltas[i-2]+deltas[i-1])*d_points[i+1])/
                    (deltas[i-2]+deltas[i-1]+deltas[i]);
        }
        b_points[3*i+1]=((deltas[i]+deltas[i+1])*d_points[i+1] + deltas[i-1]*d_points[i+2])/
                (deltas[i-1]+deltas[i]+deltas[i+1]);
        b_points[3*i]=(deltas[i]*b_points[3*i-1])/(deltas[i-1]+deltas[i]) + (deltas[i-1]*b_points[3*i+1])/
                (deltas[i-1]+deltas[i]);
    }

    // last 3 elements
    if(L != 1){
        b_points[3*L-2]=(deltas[L-1]*d_points[L]+deltas[L-2]*d_points[L+1])/(deltas[L-2]+deltas[L-1]);
    }
    b_points[3*L-1]=d_points[L+1];
    b_points[3*L]=d_points.back();
}

void MainWindow::updateLAndDeltasOnBuild(){
    deltas.push_back(DELTA);
    L = d_points.size()-3;
    b_points.resize(3*L+1);
}

QRect MainWindow::createDRect(QPoint dPoint){
    return QRect(dPoint.x(), dPoint.y(), 7, 7);
}

void MainWindow::on_pushButton_clicked() {
    mPix.fill(Qt::white);
    d_points.clear();
    b_points.clear();
    deltas.clear();
    indexOfDragStartPoint = -1;
    update();
}
