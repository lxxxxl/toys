#include "gamefield.h"

GameField::GameField()
{
    grid = new QGridLayout();
    this->setLayout(grid);
    fieldSize = QSize(10,10);
    processInput = false;
}

void GameField::setFieldSize(int width, int height)
{
    fieldSize = QSize(width, height);
}

void GameField::generate()
{
    clear();

    // add some words from words.db
    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName("words.db");
    if (!sdb.open())
        return;

    QSqlQuery query;
    query.exec("SELECT * FROM (SELECT word FROM words ORDER BY RANDOM() LIMIT 50) AS q1 ORDER BY length(word) DESC");
    while (query.next()) {
             QString word = query.value(0).toString().toUpper();
             if (addWord(word))
                 words.append(word);
    }

    // add random letters
    for (int col = 0; col < fieldSize.width(); col++) {
        for (int row = 0; row < fieldSize.height(); row++) {
            if (grid->itemAtPosition(row, col) == nullptr)
                addLetter(letters.at(QRandomGenerator::global()->generate() % letters.size()), row, col);
        }
    }
}

void GameField::clear()
{
    words.clear();
    pressedLetters.clear();
    nextAllowedRow = -1;
    nextAllowedColumn = -1;

    for (int i = 0; i < fieldSize.width(); i++) {
        for (int j = 0; j < fieldSize.height(); j++) {
            QLayoutItem *item = grid->itemAtPosition(i,j);
            if (item != nullptr)
                delete item->widget();
        }
    }
}

void GameField::updateNextAllowedCell()
{
    switch(selectionDirection){
    case Down:
        if (nextAllowedRow >= fieldSize.height())
            return;
        nextAllowedRow++;
        break;
    case Up:
        if (nextAllowedRow <= 0)
            return;
        nextAllowedRow--;
        break;
    case Right:
        if (nextAllowedColumn >= fieldSize.width())
            return;
        nextAllowedColumn++;
        break;
    case Left:
        if (nextAllowedColumn <= 0)
            return;
        nextAllowedColumn--;
        break;
    }
}

void GameField::mousePressEvent(QMouseEvent *event)
{
    processInput = true;
}

void GameField::mouseReleaseEvent(QMouseEvent *event)
{
    processInput = false;
    nextAllowedRow = -1;
    nextAllowedColumn = -1;
    bool found = false;
    QString word;
    foreach (QLabel *label, pressedLetters){
        word += label->text();
    }

    if (words.contains(word)){
        words.removeOne(word);
        emit wordSolved(word);
        found = true;
    }

    foreach (QLabel *label, pressedLetters){
        if (found){
            label->setProperty("state", INACTIVE);
            label->setStyleSheet("QLabel { background-color : grey; color : white; }");
        }
        else{
            label->setProperty("state", NORMAL);
            label->setStyleSheet("");
        }

    }

    pressedLetters.clear();

    if (words.count() == 0)
        emit win();
}

void GameField::mouseMoveEvent(QMouseEvent *event)
{
    if (!processInput)
        return;

    QWidget *widget = this->childAt(event->pos());
    if (widget == nullptr)
        return;

    // check linearity of selection
    int row_current = 0;
    int column_current = 0;
    int rowSpan_current = 0;
    int columnSpan_current = 0;
    grid->getItemPosition(grid->indexOf(widget), &row_current, &column_current, &rowSpan_current, &columnSpan_current);

    if ((nextAllowedRow == -1) && (nextAllowedColumn == -1)){
        if (pressedLetters.count() > 0 && pressedLetters.last() != widget)
        {
            int row_prev = 0;
            int column_prev = 0;
            int rowSpan_prev = 0;
            int columnSpan_prev = 0;
            grid->getItemPosition(grid->indexOf(pressedLetters.last()), &row_prev, &column_prev, &rowSpan_prev, &columnSpan_prev);

            if (row_current < row_prev)
                selectionDirection = Up;
            else if (row_current > row_prev)
                selectionDirection = Down;
            else if (column_current < column_prev)
                selectionDirection = Left;
            else if (column_current > column_prev)
                selectionDirection = Right;

            nextAllowedRow = row_current;
            nextAllowedColumn = column_current;
            updateNextAllowedCell();
        }
    }
    else{
        if ((row_current != nextAllowedRow) || (column_current) != nextAllowedColumn)
            return;
        updateNextAllowedCell();
    }


    // stop processing if selected inactive cell
    if (widget->property("state").toInt() == INACTIVE){
        mouseReleaseEvent(nullptr);
        return;
    }

    // do not select already selected cell
    if (widget->property("state").toInt() == SELECTED)
        return;

    widget->setStyleSheet("QLabel { background-color : red; color : blue; }");
    widget->setProperty("state", SELECTED);
    pressedLetters.append((QLabel*)widget);

}

bool GameField::addWord(QString word)
{

    // reverse word
    if ((QRandomGenerator::global()->generate() % 100) > 50)
        std::reverse(word.begin(), word.end());
    // horizontal/vertical word pracement
    bool axis = (QRandomGenerator::global()->generate() % 100) > 50;
    // attempt counter to prevent infinite loop
    int counter = 0;
    // start coords
    int row, col = 0;

    while (true){
cont_loop:
        // infinite looping check
        if (counter > 50)
            axis = !axis;
        if (counter > 100)
            return false;
        counter++;

        if (axis){
            // horizontal

            // if word is more or equal than grid size...
            if (fieldSize.width() <= word.size())
                return false;

            row = QRandomGenerator::global()->generate() % fieldSize.height();
            col = QRandomGenerator::global()->generate() % (fieldSize.width() - word.size());

            for(int i = 0; i < word.size(); i++)
                if (grid->itemAtPosition(row,col+i) != nullptr)
                    goto cont_loop;

            for(int i = 0; i < word.size(); i++)
                addLetter(word.at(i), row, col+i);
        }
        else{
            // vertical

            // if word is more or equal than grid size...
            if (fieldSize.height() <= word.size())
                return false;

            row = QRandomGenerator::global()->generate() % (fieldSize.height() - word.size());
            col = QRandomGenerator::global()->generate() % fieldSize.width();

            for(int i = 0; i < word.size(); i++)
                if (grid->itemAtPosition(row+i,col) != nullptr)
                    goto cont_loop;

            for(int i = 0; i < word.size(); i++)
                addLetter(word.at(i), row+i, col);
        }
        return true;
    }
}

void GameField::addLetter(QString letter, int row, int col)
{
    QLabel *label = new QLabel(letter);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label->setProperty("state", NORMAL);
    grid->addWidget(label, row, col);
}
