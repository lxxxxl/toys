#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QScreen>
#include <QScrollArea>
#include <QScroller>
#include <QTextDocument>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    this->speaker = new Speaker();
    this->FirstNum = 1;
    this->Operation = '+';
    this->SentenceDelay = 3000;
    connect(this->speaker, &Speaker::SpeakDone, this, &MainWindow::SpeakDone);

    ui->setupUi(this);

    // setup scrollable UI
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(ui->verticalLayoutWidget);
    //scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    scrollArea->setWidgetResizable(true);
    this->setCentralWidget(scrollArea);
    QScroller::grabGesture(scrollArea, QScroller::TouchGesture);

    GenerateNumberButtons();
    GenerateExpressionButtons();

}

/*
 * Generates buttons with numbers
 **/
void MainWindow::GenerateNumberButtons()
{


    QHBoxLayout *layout1 = new QHBoxLayout();
    ui->verticalLayout->addLayout(layout1);
    for (int i = 1; i <= 5; i++)
    {
        QPushButton *button = new QPushButton(QString::number(i));

        // set properties
        button->setProperty("num", i);
        button->setProperty("hilighted", false);

        // set style
        button->setFlat(true);
        QFont font = button->font();
        font.setPixelSize(this->OptimalFontSize()/2);
        button->setFont(font);


        // connect signal-slot
        connect(button, &QPushButton::released, this, &MainWindow::ButtonClickNum);


        // add button to UI
        layout1->addWidget(button);
        this->ButtonListNums.append(button);
    }

    QHBoxLayout *layout2 = new QHBoxLayout();
    ui->verticalLayout->addLayout(layout2);
    for (int i = 6; i <= 10; i++)
    {

        QPushButton *button = new QPushButton(QString::number(i));

        // set properties
        button->setProperty("num", i);
        button->setProperty("hilighted", false);

        // set style
        button->setFlat(true);
        QFont font = button->font();
        font.setPixelSize(this->OptimalFontSize()/2);
        button->setFont(font);

        // connect signal-slot
        connect(button, &QPushButton::released, this, &MainWindow::ButtonClickNum);


        // add button to UI
        layout2->addWidget(button);
        this->ButtonListNums.append(button);
    }


}

/*
 * Generates buttons with expressions
 **/
void MainWindow::GenerateExpressionButtons()
{
    for (int i = 1; i < 10; i++)
    {
        QPushButton *button;
        if (this->ButtonListExpressions.count() < i) {
             button = new QPushButton(this);

            // add button to UI
            ui->verticalLayout->addWidget(button);
            this->ButtonListExpressions.append(button);
        }
        else
            button = this->ButtonListExpressions.at(i - 1);


        // set text
        QString expression = QString("%1 %2 %3").arg(this->FirstNum).arg(this->Operation).arg(i);
        int result = this->EvaluateExpression(this->FirstNum, i, this->Operation);


        // set button text
        QPixmap pixmap = this->ButtonLabel(expression, QString::number(result), false);

        button->setIconSize(pixmap.size());
        button->setIcon(pixmap);

        // set properties
        button->setProperty("expression", expression);
        button->setProperty("result", result);
        button->setProperty("hilighted", false);

        // set style
        button->setFlat(true);

        // connect signal-slot
        connect(button, &QPushButton::released, this, &MainWindow::ButtonClickExpression);
    }
}

/*
 * Main window destructor
 **/
MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * Generates button label
 * @param expression for which this button is used
 * @param result of expression
 * @param highlight used when button was selected
 **/
QPixmap MainWindow::ButtonLabel(QString expression, QString result, bool highlight){
    QTextDocument doc;
    QString html;
    if (highlight)
        html = QString("%1 = <font color=red>%2</font>").arg(expression, result);
    else
        html = QString("%1 = %2").arg(expression, result);

    doc.setHtml(html);
    QFont font = doc.defaultFont();
    font.setPixelSize(this->OptimalFontSize());
    font.setBold(true);
    doc.setDefaultFont(font);
    doc.setTextWidth(doc.size().width());

    QPixmap pixmap(doc.size().width(), doc.size().height());
    pixmap.fill( Qt::transparent );
    QPainter painter(&pixmap);
    doc.drawContents(&painter);
    return pixmap;
}

/*
 * Evaluates arithmetic expression
 * @param firstNum first number in expression
 * @param secondNum second number in expression
 * @param operation arithmetic operation
 **/
int MainWindow::EvaluateExpression(int firstNum, int secondNum, char operation)
{
    switch (operation){
    case '+':
        return firstNum + secondNum;
    default:
        return 0;
    }
}

/*
 * Get optimal font size according to screen resolution
 **/
int MainWindow::OptimalFontSize()
{
    QScreen *screen = QApplication::screens().at(0);
    int width = screen->size().width();
    int height = screen->size().height();

    return qMin(width,height)/5;
}

/*
 * Slot for processing clicks on Expression buttons
 **/
void MainWindow::ButtonClickExpression()
{
    QPushButton *sender = (QPushButton*)QObject::sender();
    foreach (QPushButton* button, this->ButtonListExpressions)
    {
        if (sender != button)
            button->setEnabled(false);

    }

    // highlight result number
    QPixmap pixmap = this->ButtonLabel(sender->property("expression").toString(), QString::number(sender->property("result").toInt()), true);

    sender->setIconSize(pixmap.size());
    sender->setIcon(pixmap);
    sender->setProperty("hilighted", true);



    QString firstSentence = QString("Сколько будет %1").arg(sender->property("expression").toString());
    QString secondSEntence = QString::number(sender->property("result").toInt());
    this->speaker->SpeakWithDelay(firstSentence, secondSEntence, this->SentenceDelay);
}

/*
 * Slot for processing end of speech synthesis
 **/
void MainWindow::SpeakDone()
{

    foreach (QPushButton* button, this->ButtonListExpressions)
    {
        button->setEnabled(true);
        if (button->property("hilighted").toBool()){
            button->setProperty("highlited", false);
            QPixmap pixmap = this->ButtonLabel(button->property("expression").toString(), QString::number(button->property("result").toInt()), false);

            button->setIconSize(pixmap.size());
            button->setIcon(pixmap);
        }

    }
}

/*
 * Slot for processing clicks on Number buttons
 **/
void MainWindow::ButtonClickNum()
{
    QPushButton *sender = (QPushButton*)QObject::sender();
    foreach (QPushButton* button, this->ButtonListNums)
    {
        if (sender != button){
            button->setProperty("highlited", false);
            QFont font = button->font();
            font.setBold(false);
            button->setFont(font);
        }
    }

    sender->setProperty("highlited", true);
    QFont font = sender->font();
    font.setBold(true);
    sender->setFont(font);

    this->FirstNum = sender->property("num").toInt();
    GenerateExpressionButtons();
}
