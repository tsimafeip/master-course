#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    mPix = QPixmap(this->width(), this->height() - HEIGHT);
    mPix.fill(Qt::white);
    exact::Init();
    segmentsGenerated = false;
    segmentsIntersected = false;
    segmentsDrown = false;
    segmentsIntersectedDrown = false;
    qApp->installEventFilter(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter tempPainter(&mPix);

    // drawing segments only once
    if(segmentsGenerated && !segmentsDrown){
        for (size_t i = 0; i < segments.size(); i++) {
            tempPainter.drawLine(segments[i]);
        }
        segmentsDrown = true;
    }

    if (segmentsGenerated && segmentsIntersected && !segmentsIntersectedDrown){
        //checking for intersections and drawing them
        tempPainter.setPen(QPen(Qt::red));
        std::set<int> segments_to_draw;
        QLine cur_segment;
        std::multimap<int, int> L;

        for(auto curXSegmentIndexPair: orderedXValues){
            cur_segment = segments[curXSegmentIndexPair.second];
            // vertical detected.
            if (cur_segment.x1() == cur_segment.x2()){
                bool isIntersected = false;
                for( auto horizSegmentIndexYPair: L){
                    if(horizSegmentIndexYPair.second >= cur_segment.y1() && cur_segment.y2() >= horizSegmentIndexYPair.second){
                        segments_to_draw.insert(horizSegmentIndexYPair.first);
                        isIntersected = true;
                    }
                }
                if(isIntersected) segments_to_draw.insert(curXSegmentIndexPair.second);
            }
            // left side of the segment: adding segment + y value
            else if(curXSegmentIndexPair.first == cur_segment.x1()){
                L.emplace(curXSegmentIndexPair.second, cur_segment.y1());
            }
            // right side of the segment: removing segment
            else{
                L.erase(curXSegmentIndexPair.second);
            }
        }

        for(auto segment_index: segments_to_draw){
            tempPainter.drawLine(segments[segment_index]);
        }

        segmentsIntersectedDrown = true;
    }

    redrawPixmap();
}

std::vector<QLine> MainWindow::generateSegments(int maxX, int maxY, int maxLen) {
    orderedXValues.clear();
    int x1, y1, x2, y2, len;

    std::vector<QLine> newSegments;
    srand(time(NULL));
    int middle = COUNT/2;
    std::multimap<int, int> endsOfHorizontalSegments;

    for (int i = 0; i < COUNT; i++) {
        x1 = (rand() % maxX);
        y1 = (rand() % maxY);
        len = (rand() % maxLen);

        // we insert verticals after horizontals to keep order of checking
        if (i > middle){
            x2 = x1;
            while(y1 + len > maxY){
                len = (rand() % maxLen);
            }
            y2 = y1+len;
        }
        else{
            y2 = y1;
            while(x1+len > maxX){
                len = (rand() % maxLen);
            }
            x2 = x1+len;
            // we will insert ends after to erase them after checking during intersection process
            endsOfHorizontalSegments.emplace(x2, i);
        }

        orderedXValues.emplace(x1, i);

        newSegments.push_back(QLine(QPoint(x1, y1), QPoint(x2, y2)));
    }

    for(auto endOfHor: endsOfHorizontalSegments){
        orderedXValues.insert(endOfHor);
    }

    return newSegments;
}

void MainWindow::redrawPixmap() {
    painter.begin(this);
    painter.drawPixmap(0, 0, mPix);
    painter.end();
}

void MainWindow::on_pushButton_clicked() {
    segmentsGenerated = false;
    mPix.fill(Qt::white);

    const QString text = ui->segmentsCount->text();
    bool is_parsed = true;
    COUNT = text.toInt(&is_parsed);
    if (!is_parsed) return;

    segments = generateSegments(this->width(), this->height() - HEIGHT, this->width()/30);
    segmentsGenerated = true;
    segmentsIntersected = false;
    segmentsDrown = false;
    segmentsIntersectedDrown = false;

    update();
}

void MainWindow::on_pushButton_2_clicked() {
    segmentsIntersected = true;
    update();
}
