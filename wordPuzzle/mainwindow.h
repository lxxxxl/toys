#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include "gamefield.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void generate();
    ~MainWindow();

public slots:
    void wordSolved(QString word);
    void win();


private:
    Ui::MainWindow *ui;
    QList<QLabel*> wordsLabels;
    GameField *field;

};
#endif // MAINWINDOW_H
