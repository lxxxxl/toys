#ifndef PACKAGE_H
#define PACKAGE_H

#include <QString>
#include <QObject>
#include <QtDebug>

class Package: public QObject
{
    Q_OBJECT
public:
    operator QString() const {return QString("%1 %2").arg(m_width).arg(m_height);}
    explicit Package(int width, int height){
        m_width = width;
        m_height = height;
    }


    int getWidth(){return m_width;}
    int getHeight(){return m_height;}



private:
    int m_width;
    int m_height;
};


#endif // PACKAGE_H
