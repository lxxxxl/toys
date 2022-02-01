#include "mainwindow.h"



QDebug operator<<(QDebug dbg, Package *package)
{
    dbg.nospace() << "Package{" << package->getWidth() << ',' << package->getHeight() << "}";
    return dbg.maybeSpace();
}

bool comparator(const QList<Package *> &v1, const QList<Package *> &v2)
{
    int sum1 = 0;
    int sum2 = 0;
    for (auto p : v1)
        sum1 += p->getWidth();
    for (auto p : v2)
        sum2 += p->getWidth();
    return sum1 > sum2;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_centralWidget = new QWidget(this);
    this->setCentralWidget(m_centralWidget);

    // generate packages
    m_maxPackages = 30;
    auto rand = QRandomGenerator();
    QList<Package*> packages;
    for (int i = 0; i < m_maxPackages; ++i) {
        Package *package = new Package(
                    (rand.generate() % 5)+1,
                    4);
        packages.append(package);
    }

    // perform container fill
    m_containerWidth = 10;
    m_containerHeight = 50;

    while (true){
        QList<Package *> line;
        int currentWidth = 0;

        if (packages.count() == 0)
            break;

        line.append(packages.at(0));
        packages.removeAt(0);
        currentWidth = line.at(0)->getWidth();

        bool repeatLoop;
        do{
            repeatLoop = false;
            for (auto it = packages.begin(); it != packages.end(); it++){
                if ((m_containerWidth - currentWidth) >= (*it)->getWidth()){
                    line.append(*it);
                    currentWidth += (*it)->getWidth();
                    packages.erase(it);
                    repeatLoop = true;
                    break;
                }
            }
        }while(repeatLoop);

        // got data for full line
        m_sortedPackages.append(line);
    }
    // sort lines descending
    std::sort(m_sortedPackages.begin(), m_sortedPackages.end(), comparator);
    qDebug() << m_sortedPackages;
}



void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.fillRect(m_centralWidget->rect(), QBrush(Qt::white));

    QRandomGenerator generator;

    int cellWidth = width() / m_containerWidth;
    int cellheight = height() / m_containerHeight;

    //painter.setPen(Qt::red);

    int currentY = height();
    for (auto list : m_sortedPackages){
        int currentX = 0;
        for (auto package : list){
            painter.setBrush(QBrush(QColor(
                                        generator.generate() % 255,
                                        generator.generate() % 255,
                                        generator.generate() % 255)));

            painter.drawRect(currentX,
                             currentY - package->getHeight()*cellheight,
                             package->getWidth() * cellWidth,
                             package->getHeight() * cellheight);
            currentX += package->getWidth() * cellWidth;
        }
        currentY -= cellheight * 4;
    }


}

MainWindow::~MainWindow()
{

}

