#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();

    m_spinBox = new QSpinBox;
    m_spinBox->setMinimum(1);
    m_spinBox->setMaximum(10);
    m_spinBox->setValue(5);

    QPushButton *button = new QPushButton("Generate");
    connect(button, SIGNAL(clicked()), this, SLOT(clicked()));


    QLabel *label = new QLabel("Min num");

    layout->addWidget(label);
    layout->addWidget(m_spinBox);
    layout->addWidget(button);

    QWidget *w = new QWidget(this);
    w->setLayout(layout);
    this->setCentralWidget(w);
}

MainWindow::~MainWindow()
{
}

void MainWindow::drawHouse(QPainter &painter, int num, int floors, int width, int offsetX, int offsetY)
{

    int floorHeight = width / 2;
    int floorWidth = width;

    //painter.fillRect(m_centralWidget->rect(), QBrush(Qt::white));
    //painter.setBackground(Qt::white);
    painter.setPen(Qt::black);

    QFont font = painter.font() ;
    font.setPointSize(floorHeight / 2);
    painter.setFont(font);

    // generate pairs
    QList<QPair<int,int>> pairs;
    for (int i = 0; i <= num; i++) {
        pairs.push_back(QPair<int, int>(i, -1));
        pairs.push_back(QPair<int, int>(-1, i));
    }


    /// draw roof
    painter.drawLine(
                QPoint(floorWidth/2 + offsetX, 0 + offsetY),
                QPoint(0 + offsetX, floorHeight + offsetY));
    painter.drawLine(
                QPoint(floorWidth/2 + offsetX, 0 + offsetY),
                QPoint(floorWidth + offsetX, floorHeight + offsetY));
    painter.drawLine(
                QPoint(0 + offsetX, (floorHeight) + offsetY),
                QPoint(floorWidth + offsetX, (floorHeight) + offsetY));

    if (num >= 10)
        painter.drawText(
                QPoint(floorWidth/2 + offsetX - floorHeight/5 , floorHeight - floorHeight/8 + offsetY),
                QString::number(num));
    else
        painter.drawText(
                QPoint(floorWidth/2 + offsetX - floorHeight/6 , floorHeight - floorHeight/8 + offsetY),
                QString::number(num));


    ///draw floors
    for (int i = 1; i <= floors; i++) {
        /// walls
        painter.drawLine(
                    QPoint(0 + offsetX, ((i) * floorHeight) + offsetY),
                    QPoint(0 + offsetX, ((i+1) * floorHeight) + offsetY));
        painter.drawLine(
                    QPoint(floorWidth/2 + offsetX, ((i) * floorHeight) + offsetY),
                    QPoint(floorWidth/2 + offsetX, ((i+1) * floorHeight) + offsetY));
        painter.drawLine(
                    QPoint(floorWidth + offsetX, ((i) * floorHeight) + offsetY),
                    QPoint(floorWidth + offsetX, ((i+1) * floorHeight) + offsetY));
        /// floor
        painter.drawLine(
                    QPoint(0 + offsetX, ((i+1) * floorHeight) + offsetY),
                    QPoint(floorWidth + offsetX, ((i+1) * floorHeight) + offsetY));

        /// numbers
        if (pairs.count() == 0)
            break;
        int index = QRandomGenerator::global()->generate() % pairs.count();
        auto pair = pairs[index];
        pairs.erase(pairs.begin() + index);
        if (pair.first != -1){
            if (pair.first >= 10)
                painter.drawText(
                        QPoint(floorWidth/4 + offsetX - floorHeight/5 , ((i+1) * floorHeight) - floorHeight/8 + offsetY),
                        QString::number(pair.first));
            else
                painter.drawText(
                        QPoint(floorWidth/4 + offsetX - floorHeight/8 , ((i+1) * floorHeight) - floorHeight/8 + offsetY),
                        QString::number(pair.first));
        }
        if (pair.second != -1){
            if (pair.second >= 10)
                painter.drawText(
                        QPoint(floorWidth/4 + offsetX - floorHeight/5 , ((i+1) * floorHeight) - floorHeight/8 + offsetY),
                        QString::number(pair.second));
            else
            painter.drawText(
                        QPoint(floorWidth/2 + floorWidth/4 + offsetX - floorHeight/8 , ((i+1) * floorHeight) - floorHeight/8 + offsetY),
                        QString::number(pair.second));
        }
    }
}

void MainWindow::clicked()
{
    int floors = 4;
    int floorWidth = 40;

    int cols = 6;
    int rows = 4;

    int width = (floorWidth + floorWidth/2) * cols + 10 * 2;
    int height = ((floors + 3) * (floorWidth/2)) * rows;

    QPixmap pixmap(width, height);
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(), QBrush(Qt::white));

    int num = m_spinBox->value();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            drawHouse(painter,
                      num,
                      floors,
                      floorWidth,
                      10 + (j * (floorWidth + floorWidth/2)),
                      10 + ((floors+2) * (floorWidth/2) * i));
        }
        num += 1;
    }
    QString filename = QFileDialog::getSaveFileName(this, "Save as...");
    if (!filename.isEmpty()){
        if (!filename.endsWith(".png"))
            filename += ".png";
        pixmap.save(filename);
    }

}

