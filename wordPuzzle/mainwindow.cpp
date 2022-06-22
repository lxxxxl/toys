#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QVBoxLayout *layout = new QVBoxLayout;
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);
    this->setFixedSize(QSize(600, 800));

    field = new GameField();
    layout->addWidget(field);
    connect(field, SIGNAL(wordSolved(QString)), this, SLOT(wordSolved(QString)));
    connect(field, SIGNAL(win()), this, SLOT(win()));

    generate();

}

void MainWindow::generate()
{
    for (QLabel* label: wordsLabels){
        label->deleteLater();
        wordsLabels.removeOne(label);
    }

    field->generate();

    foreach (QString s, field->words){
        QLabel *label = new QLabel();
        label->setText(s);
        centralWidget()->layout()->addWidget(label);
        wordsLabels.append(label);
    }


}

void MainWindow::wordSolved(QString word)
{
    foreach (QLabel *label, wordsLabels){
        if (label->text() == word){
            label->setStyleSheet("color : white;");
            break;
        }
    }
}

void MainWindow::win()
{
    QMessageBox::information(this, "Congratulations!", "You win!");
    generate();

}

MainWindow::~MainWindow()
{
    delete ui;
}

