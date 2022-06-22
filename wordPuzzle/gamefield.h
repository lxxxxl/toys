#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <QWidget>
#include <QGridLayout>
#include <QMouseEvent>
#include <QLabel>
#include <QRandomGenerator>
#include <QtSql>

class GameField : public QWidget
{
    Q_OBJECT
public:
    // constructor
    GameField();
    // set size of game field
    void setFieldSize(int width, int height);
    // generate gamefiled by filling with words and random letters
    void generate();
    // clear grid
    void clear();

    // event handlers for mouse press, release and move
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    // words on grid
    QList<QString> words;

private:
    // add word to grid. returns true on success
    bool addWord(QString word);
    // add single letter to grid
    void addLetter(QString letter, int x, int y);
    // update next allowed cell to select
    void updateNextAllowedCell();

    enum LabelState{
        NORMAL,
        SELECTED,
        INACTIVE
    };
    enum SelectionDirection{
        Up,
        Down,
        Left,
        Right
    };

    QGridLayout *grid;
    QList<QLabel*> pressedLetters;
    bool processInput;
    int nextAllowedRow;     // next allowed row in selection
    int nextAllowedColumn;     // next allowed column in selection
    SelectionDirection selectionDirection;
    QSize fieldSize;
    QList<QString> letters = {"А", "Б", "В", "Г", "Д", "Е", "Ё", "Ж", "З", "И", "Й", "К", "Л", "М", "Н", "О", "П", "Р", "С", "Т", "У", "Ф", "Х", "Ц", "Ч", "Ш", "Щ", "Ъ", "Ы", "Ь", "Э", "Ю", "Я"};




signals:
    // emits when user found word
    void wordSolved(QString word);
    // emits when user found all words
    void win();
};


#endif // GAMEFIELD_H
