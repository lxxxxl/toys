#ifndef PACKAGE_H
#define PACKAGE_H

#include <QString>
#include <QObject>
#include <QtDebug>
#include <QRect>

class Package: public QObject
{
    Q_OBJECT
public:
    operator QString() const {return QString("%1 %2").arg(m_width).arg(m_height);}
    explicit Package(int width, int height){
        m_width = width;
        m_height = height;
    }

    int width(){return m_width;}
    int height(){return m_height;}

    //QRect getRect(){return QRect(0, 0, m_width, m_height);}




private:
    int m_width;
    int m_height;
};


#endif // PACKAGE_H
