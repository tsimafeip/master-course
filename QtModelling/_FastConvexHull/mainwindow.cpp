#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mPix = QPixmap(this->width(), this->height() - HEIGHT);
    mPix.fill(Qt::white);

    exact::Init();

    pointsGenerated = false;
    pointsDrown = false;
    hullBuilt = false;
    hullDrown = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter tempPainter(&mPix);

    // draw points only once
    if(pointsGenerated && !pointsDrown){
        for (int i = 0; i < points.size(); i++) {
            points.size() > 1000
                    ? tempPainter.drawPoint(points[i])
                    : tempPainter.drawEllipse(points[i], 2, 2);
        }

        pointsDrown = true;
    }

    if(hullBuilt && !hullDrown){
        tempPainter.setBrush(QBrush(Qt::red));
        tempPainter.setPen(QPen(Qt::red));
        tempPainter.drawEllipse(S[0], 2, 2);
        for (int i = 1; i < S.size(); i++) {
            tempPainter.drawEllipse(S[i], 3, 3);

            tempPainter.drawLine(S[i-1], S[i]);
        }

        hullDrown = true;
    }

    redrawPixmap();
}

void MainWindow::redrawPixmap() {
    painter.begin(this);
    painter.drawPixmap(0, 0, mPix);
    painter.end();
}

QVector<QPoint> MainWindow::generatePointsCircle(int count, int centerX, int centerY, int radius){
//    srand((unsigned)time(NULL));
//    double rand_num =  ((double)rand()/(double)RAND_MAX);
//    int x, y;

    QVector<QPoint> points(count);

//    for (int i = 0; i < count; i++) {

//        srand((unsigned)time(NULL));
//        rand_num =  ((double)rand()/(double)RAND_MAX);
//        int theta = rand_num * 2 * M_PI;

//        x = centerX + radius * cos(theta);
//        y = centerY + radius * sin(theta);

//        points[i]=QPoint(x, y);
//    }

    return points;
}


QVector<QPoint> MainWindow::generatePointsRectangle(int count, int minX, int minY, int maxX, int maxY) {
    int x, y;

    QVector<QPoint> points(count);

    for (int i = 0; i < count; i++) {
        x = (rand() % maxX + minX);
        y = (rand() % maxY + minY);

        points[i]=QPoint(x, y);
    }

    return points;
}

void MainWindow::preprocess(bool full_mode) {
    pl = points[0];
    pr = points[0];

    if (full_mode){
        pb = points[0];
        pt = points[0];
    }

    QPoint curPoint;
    for (int i = 0; i < points.size(); i++){
        curPoint = points[i];
        if (curPoint.x() > pr.x())
            pr = curPoint;
        if (curPoint.x() < pl.x())
            pl = curPoint;

        if (full_mode){
            if (curPoint.y() < pt.y())
                pt = curPoint;
            if (curPoint.y() > pb.y())
                pb = curPoint;
        }
    }

    if(full_mode){
        S.append(pl);
        S.append(pt);
        S.append(pr);
        S.append(pb);
        REAL plPoint[2] = {pl.x() * 1.0, pl.y() * 1.0};
        REAL ptPoint[2] = {pt.x() * 1.0, pt.y() * 1.0};
        REAL prPoint[2] = {pr.x() * 1.0, pr.y() * 1.0};
        REAL pbPoint[2] = {pb.x() * 1.0, pb.y() * 1.0};

        int side = exact::right;
        for (auto local_point: points){
            REAL point[2] = {local_point.x()*1.0, local_point.y()*1.0};

            if (exact::orient2d(plPoint,ptPoint,point) == side)
            {
                S.append(local_point);
            }
            else if (exact::orient2d(ptPoint,prPoint,point) == side)
            {
                S.append(local_point);
            }
            else if (exact::orient2d(prPoint,pbPoint,point) == side)
            {
                S.append(local_point);
            }
            else if (exact::orient2d(pbPoint,plPoint,point) == side)
            {
                S.append(local_point);
            }
        }
    }
}

QVector<QPoint> MainWindow::getPointsOnTheSpecificSide(QVector<QPoint> local_s, QPoint local_pl, QPoint local_pr, int side){
    QVector<QPoint> res;
    REAL plPoint[2] = {local_pl.x() * 1.0, local_pl.y() * 1.0};
    REAL prPoint[2] = {local_pr.x() * 1.0, local_pr.y() * 1.0};
    for (auto local_point: local_s){
        REAL point[2] = {local_point.x()*1.0, local_point.y()*1.0};

        if (exact::orient2d(plPoint, prPoint, point) == side)
        {
            res.append(local_point);
        }
    }

    return res;
}

int lineDist(QPoint p1, QPoint p2, QPoint p) {
    return abs ((p.y() - p1.y()) * (p2.x() - p1.x()) -
               (p2.y() - p1.y()) * (p.x() - p1.x()));
}

QPoint MainWindow::getTheMostFarPoint(QVector<QPoint> local_s, QPoint local_pl, QPoint local_pr){
    QPoint ph = local_s[0];
    if (local_s.size() > 1){
        int maxDist = lineDist(local_pl, local_pr, local_s[0]);
        local_s.removeFirst();
        for(auto point: local_s){
            int curDist = lineDist(local_pl, local_pr, point);
            if (curDist > maxDist)
            {
                maxDist = curDist;
                ph = point;
            }
        }
    }

    return ph;
}

void MainWindow::on_pushButton_clicked() {
    pointsGenerated = false;
    S.clear();
    points.clear();
    mPix.fill(Qt::white);

    const QString text = ui->pointsEditor->text();
    bool is_parsed = true;
    COUNT = text.toInt(&is_parsed);
    if (!is_parsed) return;

    points = generatePointsRectangle(COUNT, HEIGHT, HEIGHT, this->width()-2*HEIGHT, this->height() - 3*HEIGHT);
    //points = generatePointsCircle(COUNT, 150, 150, 10);
    pointsGenerated = true;
    pointsDrown = false;

    update();
}

QVector<QPoint> concatVectors(QVector<QPoint> v1, QVector<QPoint> v2) {
    if (v1.empty()) return v2;

    for(auto right_el: v2)
        v1.append(right_el);
    return v1;
}

QVector<QPoint> MainWindow::leftConvexHull(QVector<QPoint> local_s, QPoint local_pl, QPoint local_pr) {
    if(local_s.size() == 0){
        local_s.append(local_pl);
        local_s.append(local_pr);
        return local_s;
    }

    QPoint ph = getTheMostFarPoint(local_s, local_pl, local_pr);

    QVector<QPoint> S_l = getPointsOnTheSpecificSide(local_s, local_pl, ph, exact::right);
    QVector<QPoint> S_r = getPointsOnTheSpecificSide(local_s, ph, local_pr, exact::right);

    QVector<QPoint> res1 = leftConvexHull(S_l, local_pl, ph);
    QVector<QPoint> res2 = leftConvexHull(S_r, ph, local_pr);

    return concatVectors(res1, res2);
}

QVector<QPoint> MainWindow::quickHull() {
    preprocess(false);
    QVector<QPoint> S_l = getPointsOnTheSpecificSide(points, pl, pr, exact::right);
    QVector<QPoint> S_r = getPointsOnTheSpecificSide(points, pl, pr, exact::left);

    QVector<QPoint> res1 = leftConvexHull(S_l, pl, pr);
    QVector<QPoint> res2 = leftConvexHull(S_r, pr, pl);

    return concatVectors(res1, res2);
}

void MainWindow::on_pushButton2_clicked() {
    S = quickHull();
    hullBuilt = true;
    hullDrown = false;
    update();
}
