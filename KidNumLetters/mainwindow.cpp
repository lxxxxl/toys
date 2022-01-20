#include "mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
     QVBoxLayout *layout = new QVBoxLayout();

     m_edit = new QTextEdit();
     m_edit->setWordWrapMode(QTextOption::WordWrap);

     QPushButton *button = new QPushButton("Generate");
     connect(button, SIGNAL(clicked()), this, SLOT(clicked()));


     layout->addWidget(m_edit);
     layout->addWidget(button);


     QWidget *w = new QWidget(this);
     w->setLayout(layout);
     this->setCentralWidget(w);

}

void MainWindow::clicked()
{
    // TODO
    // Generate ODT document
}

MainWindow::~MainWindow()
{
}

