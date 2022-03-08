#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextDocument>
#include <QFileDialog>
#include <QTextDocumentWriter>
#include <QTextTable>
#include <QSpinBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSpinBox *m_spinBox;
    bool NextSet(int *a, int m, int n);

public slots:
    void clicked();
};
#endif // MAINWINDOW_H
