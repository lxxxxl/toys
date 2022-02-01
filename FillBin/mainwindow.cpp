#include "mainwindow.h"

QDebug operator<<(QDebug dbg, Package *package)
{
    dbg.nospace() << "Package{" << package->width() << ',' << package->height() << "}";
    return dbg.maybeSpace();
}

bool comparator(Package *p1, Package *p2)
{
    return p1->height() > p2->height();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_centralWidget = new QWidget(this);
    this->setCentralWidget(m_centralWidget);

    m_rand = QRandomGenerator();

    // package sizes should be multiple of m_stepSize
    m_stepSize = 10;

    // generate packages
    m_maxPackages = 20;

    for (int i = 0; i < m_maxPackages; ++i) {
        Package *package = new Package(
                    ((m_rand.generate() % 5)+1) * m_stepSize,
                    ((m_rand.generate() % 5)+1) * m_stepSize);
        m_sortedPackages.append(package);
    }

    std::sort(m_sortedPackages.begin(), m_sortedPackages.end(), comparator);
}


void MainWindow::drawNextFitDecrasingHigh()
{
    QPainter painter(this);
    painter.fillRect(m_centralWidget->rect(), QBrush(Qt::white));

    int currentY = height();
    int currentX = 0;
    int maxHeight = m_sortedPackages[0]->height();
    for (auto package : m_sortedPackages){
        if ((currentX + package->width()) > width()){
            currentX = 0;
            currentY -= maxHeight;
            maxHeight = package->height();
        }
        if (maxHeight < package->height())
            maxHeight = package->height();

        painter.setBrush(QBrush(QColor(
                                    m_rand.generate() % 255,
                                    m_rand.generate() % 255,
                                    m_rand.generate() % 255)));

        painter.drawRect(currentX,
                         currentY - package->height(),
                         package->width(),
                         package->height());
        currentX += package->width();
    }
}

// https://blackpawn.com/texts/lightmaps/default.html
void MainWindow::drawLithmapPack()
{
    QPainter painter(this);
    painter.fillRect(m_centralWidget->rect(), QBrush(Qt::white));

    // init root node
    Node *rootNode = new Node;
    rootNode->setRect(QRect(0, 0, width(), height()));


    for (auto package : m_sortedPackages){
        Node *node = rootNode->insert(package);
        if (node == nullptr)
            break;

        painter.setBrush(QBrush(QColor(
                                    m_rand.generate() % 255,
                                    m_rand.generate() % 255,
                                    m_rand.generate() % 255)));

        painter.drawRect(node->getRect());
    }
    delete rootNode;
}

void MainWindow::paintEvent(QPaintEvent*)
{
    //drawNextFitDecrasingHigh();
    drawTextureAtlas();
}

MainWindow::~MainWindow()
{

}

