#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QRandomGenerator>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void drawHouse(QPainter &painter, int num, int floors, int width, int offsetX, int offsetY);

private:
    QSpinBox *m_spinBox;

public slots:
    void clicked();
};
#endif // MAINWINDOW_H
