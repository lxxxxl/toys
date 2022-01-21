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
    QList<QChar> chars;
    QString data = m_edit->toPlainText();

    // extract unique chars
    for (int i = 0; i < data.length(); i++){
        QChar c = data.at(i);
        if (c == ' ' || c == '\n' || c == '\r')
            continue;
        if (chars.contains(c.toUpper()))
            continue;
        chars.append(c.toUpper());
    }
    std::sort(chars.begin(), chars.end());

    // initialize ODF document
    QTextDocument doc;
    QTextCursor cursor(&doc);
    QTextTableFormat tableFormat;
    QTextCharFormat charFormat;
    tableFormat.setCellPadding(5);
    tableFormat.setBorder(0);
    charFormat.setFontWeight(QFont::Bold);

    // insert character table
    int columns = 20;
    int rows = (chars.count() / columns) * 2 +
                ((chars.count() % columns) == 0 ? 0 : 2);

    QTextTable *charTable = cursor.insertTable(rows, columns, tableFormat);

    for (int row = 0; row < rows; row+=2)
    {
        for (int col = 0; col < columns; col++)
        {
            int number = col + ((row/2) * columns);
            if (number >= chars.count())
                break;
            // TODO set bold
            QTextBlockFormat blockFormat = charTable->cellAt(row, col).firstCursorPosition().blockFormat();
            blockFormat.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            charTable->cellAt(row, col).firstCursorPosition().setBlockFormat(blockFormat);
            charTable->cellAt(row+1, col).firstCursorPosition().setBlockFormat(blockFormat);
            charTable->cellAt(row, col).firstCursorPosition().insertText(QString::number(number+1));
            charTable->cellAt(row+1, col).firstCursorPosition().insertText(chars.at(number));
        }
    }

    // insert puzzle
    tableFormat.setBorder(2);
    // create separate tables for each string
    QStringList list = data.split("\n");
    for (QString str: list){
        // create new table
        int columns = 20;
        int rows = (str.length() / columns) * 2 +
                    ((str.length() % columns) == 0 ? 0 : 2);
        cursor.movePosition(QTextCursor::End);
        cursor.insertText("\n\n");
        QTextTable *puzzleTable = cursor.insertTable(rows, columns, tableFormat);

        for (int row = 0; row < rows; row+=2)
        {
            for (int col = 0; col < columns; col++)
            {
                // TODO word wrap
                QString cellvalue = "";
                int pos = col + ((row/2) * columns);
                if (pos >= str.length())
                    break;
                QChar c = str.at(pos);
                for (int i = 0; i < chars.count(); i++){
                    if (chars.at(i) == c.toUpper())
                        cellvalue = QString::number(i+1);
                }
                QTextBlockFormat blockFormat = puzzleTable->cellAt(row, col).firstCursorPosition().blockFormat();
                blockFormat.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                puzzleTable->cellAt(row, col).firstCursorPosition().setBlockFormat(blockFormat);
                puzzleTable->cellAt(row, col).firstCursorPosition().insertText(cellvalue);
            }
        }
    }


    // save ODF document
    QString filename = QFileDialog::getSaveFileName(this, "Save as...");
    QTextDocumentWriter writer(filename + ".odf", "ODF");
    writer.write(&doc);

}

MainWindow::~MainWindow()
{
}

