#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    mPix = QPixmap(this->width(), this->height() - HEIGHT);
    mPix.fill(Qt::white);
    exact::Init();
    rectangleReady = false;
    qApp->installEventFilter(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (!rectangleReady) {
        polygon.push_back(event->pos());
        update();
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseMove && rectangleReady) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (isIn != Cross(mouseEvent->pos().x(), mouseEvent->pos().y())) {
            isIn = !isIn;
            update();
        }
    }
    return false;
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter tempPainter(&mPix);
    for (size_t i = 0; i < polygon.size(); i++) {
        tempPainter.drawEllipse(polygon[i], 1, 1);
    }

    if (rectangleReady) {
        QPoint arr[100];
        std::copy(polygon.begin(), polygon.end(), arr);
        if (isIn) {
            tempPainter.setBrush(Qt::blue);
        } else {
            tempPainter.setBrush(Qt::white);
        }
        tempPainter.drawPolygon(arr, polygon.size());
    }
    redrawPixmap();
}

bool MainWindow::Cross(double xh, double yh) {
    REAL point[2] = {xh, yh};
    int count = 0;

    QPoint arr[100];
    std::copy(polygon.begin(), polygon.end(), arr);

    for (size_t j = 0; j < polygon.size(); j++) {
        REAL minPoint[2] = {polygon[j].x() * 1.0, polygon[j].y() * 1.0};
        REAL maxPoint[2] = {polygon[(j + 1) % polygon.size()].x() * 1.0, polygon[(j + 1) % polygon.size()].y() * 1.0};
        if (minPoint[1] > maxPoint[1]) {
            std::swap(minPoint, maxPoint);
        }
        if (maxPoint[1] <= point[1] || minPoint[1] > point[1]) {
            continue;
        }
        exact::position orient = exact::orient2d(minPoint, maxPoint, point);
        if (orient == exact::on) {
            return true;
        }
        if (orient == exact::right) {
            count++;
        }
    }

    return count % 2;
}

void MainWindow::redrawPixmap() {
    painter.begin(this);
    painter.drawPixmap(0, 0, mPix);
    painter.end();
}

void MainWindow::on_pushButton_clicked() {
    rectangleReady = true;
    update();
}

void MainWindow::on_pushButton_2_clicked() {
    rectangleReady = false;
    isIn = false;
    polygon.clear();
    mPix.fill(Qt::white);
    update();
}
