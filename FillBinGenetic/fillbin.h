#ifndef FILLBIN_H
#define FILLBIN_H

#include <QObject>
#include <QPoint>
#include <QRect>
#include <QRandomGenerator>
#include <QtDebug>
#include <QPixmap>

class FillBin
{
public:
    ~FillBin();
    FillBin *mate(FillBin *parent2);
    //void mutateRandomGen(int genCount);
    QPoint mutate(int index, QPoint point);
    void random();
    int fitness();
    void draw(QPixmap *pixmap);
    int generateInt(int max);
    QPoint getGen(int index);
    void setGen(int index, QPoint point);

    void setBinSize(int width, int height);
    int getBinWidth(){return m_binWidth;}
    int getBinHeight(){return m_binHeight;}
    bool objectIntersects(int index, QPoint point);

    bool m_fitnessCalculated = false;
    int m_fitness = 0;
    // array representing occupied cells
    // use height * width to simplify fitness calculation
    int *m_densityMap;
    // limitations
    int m_maxChorosomes = 10;
    int m_binWidth = 30;
    int m_binHeight = 35;

    QList<QPoint> m_chromosome{
        QPoint(0, 0),
        QPoint(0, 0),
        QPoint(0, 0),
        QPoint(0, 0),
        QPoint(0, 0),
        QPoint(0, 0),
        QPoint(0, 0),
        QPoint(0, 0),
        QPoint(0, 0),
        QPoint(0, 0)
    };

    // packages that should be placed to bin
    typedef struct{
        int width;
        int height;
    } Object;

    QList<Object> m_objects{
        Object{5, 5},
        Object{5, 10},
        Object{10, 5},
        Object{10, 15},
        Object{15, 10},
        Object{15, 15},
        Object{20, 10},
        Object{20, 5},
        Object{5, 20},
        Object{15, 5}
    };
};

#endif // FILLBIN_H
