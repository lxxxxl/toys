#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "speaker.h"

#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int EvaluateExpression(int firstNum, int secondNum, char operation);
    void GenerateExpressionButtons();
    void GenerateNumberButtons();
    int OptimalFontSize();
    void ButtonClickExpression();
    void ButtonClickNum();
    void SpeakDone();
    QPixmap ButtonLabel(QString expression, QString result, bool highlight);


    int SentenceDelay;
    int FirstNum;
    char Operation;
    QList<QPushButton*> ButtonListExpressions;
    QList<QPushButton*> ButtonListNums;
    Speaker *speaker;


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
