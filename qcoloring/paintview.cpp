#include "paintview.h"

PaintView::PaintView(QWidget *parent) : QGraphicsView(parent),
  scene(0)
{
    colorThreshold = 100;
    fillColor = QColor::fromRgb(0,0,0);
    imagePixmap = nullptr;

    // create Color picker button
    colorButton = new QPushButton();
    colorButton->setGeometry(QRect(0,0,50,50));
    updateButtonIcon(fillColor);
    scene.addWidget(colorButton);
    //connect(colorButton, SIGNAL(clicked()), this, SLOT(colorButtonClicked()));

    // create open file button
    openButton = new QPushButton();
    openButton->setGeometry(QRect(0,0,50,50));
    QIcon icon1(":/img/open.png");
    openButton->setIcon(icon1);
    openButton->setIconSize(QSize(48, 48));
    openButton->setFixedSize(QSize(48, 48));
    scene.addWidget(openButton);
    //connect(openButton, SIGNAL(clicked()), this, SLOT(openButtonClicked()));

    // create save file button
    saveButton = new QPushButton();
    saveButton->setGeometry(QRect(0,0,50,50));
    QIcon icon2(":/img/save.png");
    saveButton->setIcon(icon2);
    saveButton->setIconSize(QSize(48, 48));
    saveButton->setFixedSize(QSize(48, 48));
    scene.addWidget(saveButton);
    //connect(saveButton, SIGNAL(clicked()), this, SLOT(saveButtonClicked()));

    // setup window
    this->setScene(&scene);
    this->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    this->setFixedSize(QSize(600, 800));
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}


void PaintView::loadImage(QString filename)
{
    image = QImage(filename);
    // scale image to QGraphicsView's size
    if (image.width() > 600)
        image = image.scaledToWidth(600);
    if (image.height() > 800)
        image = image.scaledToHeight(800);

    if (imagePixmap)
        imagePixmap->setPixmap(QPixmap::fromImage(image));
    else
        imagePixmap = scene.addPixmap(QPixmap::fromImage(image));
    imagePixmap->setZValue(-1);

    setFixedSize(QSize(image.width(), image.height()));

    // set buttons position
    colorButton->move(this->width() - colorButton->width(), this->height() - colorButton->height());
    openButton->move(0, this->height() - openButton->height());
    saveButton->move(openButton->width(), this->height() - openButton->height());

}


void PaintView::mousePressEvent(QMouseEvent *event)
{
    const QPoint p = event->pos();

    // fix for double click on buttons
    if (colorButton->geometry().contains(p)){
        colorButtonClicked();
        return;
    }
    else if (openButton->geometry().contains(p)){
        openButtonClicked();
        return;
    }
    else if (saveButton->geometry().contains(p)){
        saveButtonClicked();
        return;
    }

    floodFill(p, getPixelColor(p), fillColor);

    // repaint image
    imagePixmap->setPixmap(QPixmap::fromImage(image));
}


void PaintView::updateButtonIcon(QColor color)
{
    QPixmap pixmap(40,40);
    QPainter painter(&pixmap);
    painter.setBrush(color);
    painter.setPen(color);
    painter.drawRect(0,0,40,40);
    painter.end();

    QIcon icon(pixmap);
    colorButton->setIcon(icon);
    colorButton->setIconSize(pixmap.rect().size());
}

void PaintView::colorButtonClicked()
{
    fillColor = QColorDialog::getColor(QColorDialog::DontUseNativeDialog);
    if (fillColor == QColor::Invalid)
        fillColor = QColor::fromRgb(0,0,0);

    updateButtonIcon(fillColor);
}

void PaintView::openButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(NULL,
            "Open image", "", "*.jpg *.png *.gif");

    if (!QFile::exists(fileName))
        return;

    loadImage(fileName);
}

void PaintView::saveButtonClicked()
{
    QString fileName = QFileDialog::getSaveFileName(NULL,
            "Open image", "", "*.jpg");
    image.save(fileName+".jpg");
}


QColor PaintView::getPixelColor(QPoint point)
{
    return image.pixel(point.x(), point.y());
}


void PaintView::setPixelColor(QPoint point, QColor color)
{
    image.setPixelColor(point.x(), point.y(), color);
}


bool PaintView::colorsEqual(QColor color1, QColor color2)
{
    qint32 diff = abs(color1.red() - color2.red()) +
                  abs(color1.blue() - color2.blue()) +
                  abs(color1.green() - color2.green());

    return diff < colorThreshold;
}


void PaintView::floodFill(QPoint point, QColor oldColor, QColor newColor)
{
    pixels.clear();
    pixels.push_back(point);

    if (colorsEqual(oldColor, newColor))
        return;

    while (pixels.count() > 0){
        point = pixels.last();
        pixels.pop_back();

        if (point.x() < 0 || point.x() > image.width() - 1 || point.y() < 0 || point.y() > image.height() - 1)
            continue;

        if (!colorsEqual(getPixelColor(point), oldColor))
            continue;

        setPixelColor(point, newColor);
        pixels.push_back(QPoint(point.x()+1, point.y()));
        pixels.push_back(QPoint(point.x()-1, point.y()));
        pixels.push_back(QPoint(point.x(), point.y()+1));
        pixels.push_back(QPoint(point.x(), point.y()-1));

    }
}
