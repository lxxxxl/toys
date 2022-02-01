#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QRandomGenerator>
#include <QtDebug>
#include <package.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*);
    ~MainWindow();

    int m_maxPackages;
    int m_containerWidth;
    int m_containerHeight;
    QWidget *m_centralWidget;
    QList<QList<Package*>> m_sortedPackages;
};
#endif // MAINWINDOW_H
