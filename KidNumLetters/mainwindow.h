#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QTextDocument>
#include <QFileDialog>
#include <QTextDocumentWriter>
#include <QTextTable>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QTextEdit *m_edit;

public slots:
    void clicked();
};
#endif // MAINWINDOW_H
