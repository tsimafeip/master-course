#include "mainwindow.h"
#include "ui_mainwindow.h"

// helper methods
float get_x(QMatrix1x3 t) {
    return t.data()[0];
}

float get_y(QMatrix1x3 t) {
    return t.data()[1];
}

float get_z(QMatrix1x3 t) {
    return t.data()[2];
}

double convert_to_angle(double action) {
    return action / 360 * 2 * M_PI;
}

QMatrix1x3 make_triple(float x, float y, float z) {
    float data[] = {x, y, z};
    return QMatrix1x3(data);
}

// setting up initial cube projection
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    mPix = QPixmap(this->width(), this->height() - HEIGHT);
    mPix.fill(Qt::white);
    double a = 100;

    QMatrix1x3 p1 = make_triple(-a, a, a);
    QMatrix1x3 p2 = make_triple(-a, a, -a);
    QMatrix1x3 p3 = make_triple(a, a, a);
    QMatrix1x3 p4 = make_triple(a, a, -a);

    QMatrix1x3 p5 = make_triple(-a, -a, a);
    QMatrix1x3 p6 = make_triple(-a, -a, -a);
    QMatrix1x3 p7 = make_triple(a, -a, a);
    QMatrix1x3 p8 = make_triple(a, -a, -a);

    edges = {
         std::make_pair(p1, p2),
         std::make_pair(p1, p3),
         std::make_pair(p1, p5),
         std::make_pair(p2, p4),
         std::make_pair(p2, p6),
         std::make_pair(p4, p3),
         std::make_pair(p4, p8),
         std::make_pair(p3, p7),
         std::make_pair(p5, p6),
         std::make_pair(p6, p8),
         std::make_pair(p7, p8),
         std::make_pair(p5, p7)
    };
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::redrawPixmap() {
    painter.begin(this);
    painter.drawPixmap(0, 0, mPix);
    painter.end();
}

void MainWindow::update_edges(QMatrix3x3 rotate_m) {
    for (size_t i = 0; i < edges.size(); i++) {
        edges[i] = std::make_pair(rotate_m * edges[i].first, rotate_m * edges[i].second);
    }

    mPix.fill(Qt::white);
    update();
}

QMatrix3x3 x_matrix(double action) {
    double theta = convert_to_angle(action);
    float data[] = {1, 0, 0,
                    0, (float) cos(theta), (float) sin(theta),
                    0, (float) -sin(theta), (float) cos(theta)};
    return QMatrix3x3(data);
}

QMatrix3x3 y_matrix(double action) {
    double theta = convert_to_angle(action);
    float data[] = {(float) cos(theta), 0, (float) -sin(theta),
                    0, 1, 0,
                    (float) sin(theta), 0, (float) cos(theta)};
    return QMatrix3x3(data);
}

QMatrix3x3 z_matrix(double action) {
    double theta = convert_to_angle(action);
    float data[] = {(float) cos(theta), (float) sin(theta), 0,
                    (float) -sin(theta), (float) cos(theta), 0,
                    0, 0, 1};
    return QMatrix3x3(data);
}

QMatrix3x3 rotate_matrix(double x, double y, double z) {
    return x_matrix(x) * y_matrix(y) * z_matrix(z);
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter tempPainter(&mPix);

    int centerX = this->width() / 2, centerY = (this->height() - HEIGHT) / 2;

    for (size_t i = 0; i < edges.size(); i++) {
        float x1 = get_x(edges[i].first), y1 = get_y(edges[i].first), z1 = get_z(edges[i].first);
        float x2 = get_x(edges[i].second), y2 = get_y(edges[i].second), z2 = get_z(edges[i].second);

        QPoint p1(x1 + centerX + z1 * cos(angle), y1 + centerY + z1 * sin(angle));
        QPoint p2(x2 + centerX + z2 * cos(angle), y2 + centerY + z2 * sin(angle));
        tempPainter.drawLine(p1, p2);
    }
    redrawPixmap();
}

void MainWindow::on_xSlider_actionTriggered(int action) {
    update_edges(rotate_matrix(action, 0, 0));
}

void MainWindow::on_ySlider_actionTriggered(int action) {
    update_edges(rotate_matrix(0, action, 0));
}

void MainWindow::on_zSlider_actionTriggered(int action) {
    update_edges(rotate_matrix(0, 0, action));
}
