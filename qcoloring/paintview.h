#ifndef PAINTVIEW_H
#define PAINTVIEW_H

#include <QtWidgets/QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <QColorDialog>
#include <QFileDialog>

class PaintView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PaintView(QWidget *parent = nullptr);
    void loadImage(QString filename);

public slots:
    void colorButtonClicked();
    void openButtonClicked();
    void saveButtonClicked();

private:
    void mousePressEvent(QMouseEvent *event);

    // perform Flood Fill algo on selected region
    void floodFill(QPoint point, QColor oldColor, QColor newColor);
    // gets color of specified pixel
    QColor getPixelColor(QPoint point);
    // sets color of specified pixel
    void setPixelColor(QPoint point, QColor color);
    // check colors similiarity according to colorThreshold
    bool colorsEqual(QColor color1, QColor color2);
    // update icon on button
    void updateButtonIcon(QColor color);

    // loaded image
    QImage image;
    // loaded image proxy on scene
    QGraphicsPixmapItem *imagePixmap;
    // current graphics scene
    QGraphicsScene scene;
    // button to select color
    QPushButton *colorButton;
    // button to open image
    QPushButton *openButton;
    // button to save image
    QPushButton *saveButton;
    // list of pixels to change color
    QList<QPoint> pixels;
    // treshold to check colors similiarity
    qint8 colorThreshold;
    // color to fill region
    QColor fillColor;



signals:

};

#endif // PAINTVIEW_H
