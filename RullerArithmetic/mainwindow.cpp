#include "mainwindow.h"

#include <QImage>
#include <QPainter>
#include <QPair>
#include <QRandomGenerator>
#include <QUuid>
#include <cmath>
#include <qapplication.h>

bool MainWindow::NextSet(int *a, int m, int n)
{
  int k = m;
  for (int i = k - 1; i >= 0; --i)
    if (a[i] < n - k + i + 1)
    {
      ++a[i];
      for (int j = i + 1; j < k; ++j)
        a[j] = a[j - 1] + 1;
      return true;
    }
  return false;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
     QVBoxLayout *layout = new QVBoxLayout();

     m_spinBox = new QSpinBox;
     m_spinBox->setMinimum(1);
     m_spinBox->setMaximum(100);
     m_spinBox->setValue(15);

     QPushButton *button = new QPushButton("Generate");
     connect(button, SIGNAL(clicked()), this, SLOT(clicked()));

     layout->addWidget(m_spinBox);
     layout->addWidget(button);

     QWidget *w = new QWidget(this);
     w->setLayout(layout);
     this->setCentralWidget(w);

}

void MainWindow::clicked()
{
    QList<QChar> chars;
    int _max = m_spinBox->value();

    // initialize ODF document
    QTextDocument doc;
    QTextCursor cursor(&doc);
    QTextTableFormat tableFormat;
    tableFormat.setCellPadding(10);
    tableFormat.setBorder(0);

    // insert table
    int columns = 4;
    int rows = 20;
    QTextTable *charTable = cursor.insertTable(rows, columns, tableFormat);

    // generate all possible questions
    QList<QPair<int, int>> questions;

    int *set = new int[2];
    set[0] = 1;
    set[1] = 2;

    while (NextSet(set, 2, _max)){
        QPair<int, int> pair(set[0], set[1]);
        questions.push_back(pair);
    }


    // get random questions
    QRandomGenerator randomGenerator;
    auto it = questions.begin();
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < columns; col++)
        {
            if (questions.size() == 0)
                break;
            int index = randomGenerator.generate() % questions.size();
            QPair<int, int> pair = questions[index];
            questions.removeAt(index);
            QString text;
            bool substr = (pair.first + pair.second > _max);
            if (!substr && randomGenerator.generate() % 100 > 80){
                // addition
                text = QString("%1 + %2 = ").arg(QString::number(pair.first)).arg(QString::number(pair.second));

            }
            else{
                // substraction
                text = QString("%1 - %2 = ").arg(QString::number(std::max(pair.first, pair.second)))
                        .arg(QString::number(std::min(pair.first, pair.second)));
            }
            charTable->cellAt(row, col).firstCursorPosition().insertText(text);
            it++;
        }
    }

    // add ruler
    int partlen = 30;
    int margin = 10;
    QPixmap pixmap(_max * partlen + margin * 2, margin * 3);

    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(), QBrush(Qt::white));
    painter.drawLine(margin, margin, partlen * _max + margin , margin);

    QFont font = painter.font() ;
    font.setPointSize(font.pointSize() * 1.5f);
    painter.setFont(font);
    for (int i = 0; i <= _max; i++){
        painter.drawRect(margin - 1 + partlen * i, margin - 1, 2, 2);
        QPoint point;
        if (i < 10)
            point = QPoint(margin - 5 + partlen * i, margin * 2 + 6);
        else
            point = QPoint(margin - 8 + partlen * i, margin * 2 + 6);
        // TODO fix quality
        //painter.drawText(point, QString::number(i));
    }

    cursor.movePosition(QTextCursor::End);
    cursor.insertText("\n\n");
    cursor.insertImage(pixmap.toImage());
    //pixmap.toImage().save("/mnt/ramdisk/ruler.png");

    // save ODF document
    QString filename = QFileDialog::getSaveFileName(this, "Save as...");
    if (!filename.endsWith(".odf"))
        filename += ".odf";
    QTextDocumentWriter writer(filename, "ODF");
    writer.write(&doc);
    QApplication::exit(0);
}

MainWindow::~MainWindow()
{
}


