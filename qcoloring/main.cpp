#include <QApplication>
#include <QFileDialog>

#include "paintview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString fileName = QFileDialog::getOpenFileName(NULL,
            "Open image", "", "*.jpg *.png *.gif");

    if (!QFile::exists(fileName))
        return 0;

    PaintView p;
    p.loadImage(fileName);
    p.show();
    return a.exec();
}
