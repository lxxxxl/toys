#include "fillbin.h"

#include <QColor>
#include <QPainter>
#include <QPen>

FillBin::~FillBin()
{
    delete m_densityMap;
}

FillBin *FillBin::mate(FillBin *parent2)
{
    FillBin *child = new FillBin;
    child->setBinSize(getBinWidth(), getBinHeight());
    for (int i = 0; i < 10; i++)
        if (i % 2 == 0)
            if (child->objectIntersects(i, this->getGen(i)))
                child->setGen(i, mutate(i, this->getGen(i)));
            else
                child->setGen(i, this->getGen(i));
        else{
            if (child->objectIntersects(i, parent2->getGen(i)))
                child->setGen(i, mutate(i, parent2->getGen(i)));
            else
                child->setGen(i, parent2->getGen(i));
        }

    return child;
}

/*void FillBin::mutateRandomGen(int genCount)
{
    for (int i = 0; i < genCount; i++){
        int randomIndex = generateInt(15);
        if (randomIndex < m_chromosome.size()){
            mutate(randomIndex, 1);
        }
        randomIndex = generateInt(15);
        if (randomIndex < m_chromosome.size()){
            mutate(randomIndex, 2);
        }
    }
}*/

QPoint FillBin::mutate(int index, QPoint point)
{
    int mutateTries = 10;
    while (mutateTries){
        int newX = point.x() + (generateInt(2) - 1);
        if (newX + m_objects[index].width <= m_binWidth &&
                newX >= 0/* &&
                !objectIntersects(index, QPoint(newX, point.y()))*/){
            point.setX(newX);
            break;
        }
        else
            mutateTries--;
    }
    mutateTries = 10;
    while (mutateTries){
        int newY = point.y() + (generateInt(2) - 1);
        if (newY + m_objects[index].height <= m_binHeight &&
                newY >= 0/* &&
                !objectIntersects(index, QPoint(point.x(), newY))*/){
            point.setY(newY);
            break;
        }
        else
            mutateTries--;
    }
    return point;
}

void FillBin::random()
{
    for (int i = 0; i < 10; i++)
    {
        QPoint point(
                    generateInt(m_binWidth - m_objects[i].width),
                    generateInt(m_binHeight - m_objects[i].height)
                    );
        setGen(i, point);
    }
}

int FillBin::fitness()
{
    if (m_fitnessCalculated)
        return m_fitness;
    int maxHeight = m_chromosome[0].y() + m_objects[0].height;
    for (int i = 0; i < m_chromosome.size(); i++){
        // check if object goes outside of bin
        //if (((m_chromosome[i].x() + m_objects[i].width) > m_binWidth) || m_chromosome[i].x() < 0 || m_chromosome[i].y() < 0)
        //    m_fitness += 1;

        // remember max height of bin
        int height = m_chromosome[i].y() + m_objects[i].height;
        if (height > maxHeight)
            maxHeight = height;
    }

    // check density map
    int freeCellsCount = 0;
    for (int i = 0; i < m_binWidth * (maxHeight + 1); i++)
        if (m_densityMap[i] == 0)
            freeCellsCount++;
    m_fitness += freeCellsCount;
    m_fitnessCalculated = true;
    return m_fitness;
}

void FillBin::draw(QPixmap *pixmap)
{
    QPainter painter(pixmap);
    painter.fillRect(pixmap->rect(), QBrush(Qt::white));
    painter.setPen(Qt::NoPen);

    for (int i = 0; i < 10; i++)
    {
        painter.setBrush(QBrush(QColor(
                                    generateInt(255),
                                    generateInt(255),
                                    generateInt(255))));

        QRect rect = QRect(m_chromosome[i].x(),
                            m_chromosome[i].y(),
                            m_objects[i].width,
                            m_objects[i].height);

        painter.drawRect(rect);

    }
}

int FillBin::generateInt(int max)
{
    return QRandomGenerator::system()->generate() % max + 1;
}

void FillBin::setBinSize(int width, int height)
{
    m_binWidth = width;
    m_binHeight = height;
    m_densityMap = new int[width * height];
    memset(m_densityMap, 0, sizeof(width * height * sizeof(int)));
}

bool FillBin::objectIntersects(int index, QPoint point)
{
    for (int x = point.x(); x <point.x() + m_objects[index].width; x++)
        for (int y = point.y(); y < point.y() + m_objects[index].height; y++)
            if (m_densityMap[m_binWidth * y + x] == 1)
                return true;
    return false;
}

QPoint FillBin::getGen(int index)
{
    return m_chromosome[index];
}

void FillBin::setGen(int index, QPoint point)
{
    m_chromosome[index] = point;

    // fill density map for current gen
    for (int x = m_chromosome[index].x(); x < m_chromosome[index].x() + m_objects[index].width; x++)
        for (int y = m_chromosome[index].y(); y < m_chromosome[index].y() + m_objects[index].height; y++)
            if (m_densityMap[m_binWidth * y + x] != 1)
                m_densityMap[m_binWidth * y + x] = 1;
            else
                m_fitness++;
}

QDebug operator<<(QDebug dbg, FillBin::Object object)
{
    dbg.nospace() << "Object{" << object.width << ',' << object.height << "}";
    return dbg.maybeSpace();
}


