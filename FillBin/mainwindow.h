#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QRandomGenerator>
#include <QtDebug>
#include <package.h>
#include <node.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*);
    void drawNextFitDecrasingHigh();
    void drawLithmapPack();
    ~MainWindow();

    int m_maxPackages;
    int m_stepSize;
    QWidget *m_centralWidget;
    QList<Package*> m_sortedPackages;
    QRandomGenerator m_rand;
};
#endif // MAINWINDOW_H
