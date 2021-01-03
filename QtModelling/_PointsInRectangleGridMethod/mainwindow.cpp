#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    mPix = QPixmap(this->width(), this->height() - HEIGHT);
    mPix.fill(Qt::white);

    mousePressed = false;
    pointsDrown = false;
    rectangleReady = false;
    pointsGenerated = false;
    selectedCount = 0;
    qApp->installEventFilter(this);
    ui->label->setStyleSheet("QLabel { background-color : gray; }");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    mousePressed = true;
    if (!rectangleReady) {
        mRect.setTopLeft(event->pos());
        mRect.setBottomRight(event->pos());
    } else {
        startPoint = event->pos();
        tempRect = mRect;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    if(pointsGenerated && event->type() == QEvent::MouseMove) {
        if (!rectangleReady) {
            mRect.setBottomRight(event->pos());
        } else {
            this->setCursor(Qt::ClosedHandCursor);
            QPoint diff = event->pos() - startPoint;
            mRect.setBottomRight(tempRect.bottomRight() + diff);
            mRect.setBottomLeft(tempRect.bottomLeft() + diff);
            mRect.setTopLeft(tempRect.topLeft() + diff);
            mRect.setTopRight(tempRect.topRight() + diff);
        }
        update();
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event){
  if (event->type() == QEvent::MouseMove && rectangleReady && pointsGenerated) {
      statusBar()->showMessage(QString("Selected points: %1").arg(selectedCount));
  }

  return false;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (pointsGenerated) {
        mousePressed = false;
        rectangleReady = true;
        this->setCursor(Qt::ArrowCursor);
        update();
    }
}

void MainWindow::paintEvent(QPaintEvent *event) {
    painter.begin(this);
    QPainter tempPainter(&mPix);

    if (pointsGenerated) {
        int d1 = this->width() / SIZE;
        int d2 = (this->height() - HEIGHT) / SIZE;
        tempPainter.setPen(QPen(Qt::black));

        for (size_t i = 0; i < selected.size(); i++) {
            tempPainter.drawPoint(selected[i].x(), selected[i].y());
        }
        selected.clear();
        tempPainter.setPen(QPen(Qt::red));

        int minX = std::min(mRect.topLeft().x(), mRect.bottomRight().x());
        int maxX = std::max(mRect.topLeft().x(), mRect.bottomRight().x());
        int minY = std::min(mRect.topLeft().y(), mRect.bottomRight().y());
        int maxY = std::max(mRect.topLeft().y(), mRect.bottomRight().y());

        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                int minCellX = d1 * i, minCellY = d2 * j, maxCellX = d1 * (i + 1), maxCellY = d2 * (j + 1);

                bool overlapped = !(minX > maxCellX || maxX < minCellX || minY > maxCellY || maxY < minCellY);

                if (overlapped) {
                    std::vector<QPoint> cell = grid[i][j];
                    for (size_t k = 0; k < cell.size(); k++) {
                        if (cell[k].x() >= minX && cell[k].x() <= maxX && cell[k].y() >= minY && cell[k].y() <= maxY) {
                            selected.push_back(cell[k]);
                            tempPainter.drawPoint(cell[k].x(), cell[k].y());
                        }
                    }
                }
            }
        }

        selectedCount = selected.size();
    }

    // draw first changing rectangle (while mouse is moved)
    if (!rectangleReady && mousePressed) {
        redrawPixmap();
        painter.setPen(QPen(Qt::magenta));
        painter.drawRect(mRect);
    }

    // draw the points (only once)
    if (pointsGenerated && !pointsDrown) {
        tempPainter.setPen(QPen(Qt::black));
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                for (size_t k = 0; k < grid[i][j].size(); k++){
                    tempPainter.drawPoint(grid[i][j][k]);
                }
            }
        }
        pointsDrown = true;
    }
    redrawPixmap();

    painter.setPen(QPen(Qt::green));
    painter.drawRect(mRect);

    painter.end();
}

void MainWindow::redrawPixmap() {
    painter.drawPixmap(0, 0, mPix);
}

std::vector<std::vector<std::vector<QPoint>>> MainWindow::generateGrid(int count, int maxX, int maxY) {
    int d1 = maxX / SIZE;
    int d2 = maxY / SIZE;
    int x, y;

    std::vector<std::vector<std::vector<QPoint>>> newGrid(SIZE);

    for (int i = 0; i < SIZE; i++) {
        newGrid[i].resize(SIZE);
    }

    for (int i = 0; i < count; i++) {
        x = (rand() % maxX);
        y = (rand() % maxY);

        newGrid[x / d1][y / d2].push_back(QPoint(x, y));
    }

    return newGrid;
}

void MainWindow::on_drawButton_clicked() {
    pointsGenerated = false;
    mPix.fill(Qt::white);

    redrawPixmap();
    const QString text = ui->pointsEditor->text();
    bool is_parsed = true;
    COUNT = text.toInt(&is_parsed);
    if (!is_parsed) return;

    selected.clear();

    QPoint zero(-1, -1);
    mRect.setTopLeft(zero);
    mRect.setBottomRight(zero);

    grid = generateGrid(COUNT, this->width(), this->height() - HEIGHT);
    pointsGenerated = true;
    pointsDrown = false;
    rectangleReady = false;

    update();
}
