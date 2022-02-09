//#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include "fillbin.h"

#define BIN_WIDTH 35
#define BIN_HEIGHT 45
#define MAX_ITERATIONS 10000
#define POPULATION_SIZE 20

bool comparator(FillBin *p1, FillBin *p2){
    return p1->fitness() < p2->fitness();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // create population with random species
    QList<FillBin*> population;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        FillBin *fillBin = new FillBin;
        fillBin->setBinSize(BIN_WIDTH, BIN_HEIGHT);
        fillBin->random();
        population.push_back(fillBin);
    }

    int epoch = 0;
    int bestResult = INT_MAX;
    // perform genetic algo
    while (epoch < MAX_ITERATIONS){
        std::sort(population.begin(), population.end(), comparator);
        int currentBestResult = population[0]->fitness();
        if (bestResult != currentBestResult){
            qDebug() << epoch << ": " << currentBestResult;
            bestResult = currentBestResult;
        }

        if (currentBestResult == 0)
            break;

        // remove worst species
        for (auto it = population.begin()+POPULATION_SIZE/2-1; it != population.end()-1; ){
            delete *it;
            it = population.erase(it);
        }

        // make offsprings
        for (int i = 0; i < POPULATION_SIZE/2; i+=2){
            FillBin *child1 = population[i]->mate(population[i+1]);
            population.push_back(child1);
            FillBin *child2 = population[i]->mate(population[POPULATION_SIZE/2-i]);
            population.push_back(child2);
        }
        epoch++;
    }

    if (population[0]->fitness() > 0)
        qDebug() << "Solution not found";
    QPixmap pixmap(BIN_WIDTH, BIN_HEIGHT);
    population[0]->draw(&pixmap);
    QLabel label;
    label.setPixmap(pixmap.scaled(QSize(BIN_WIDTH * 10, BIN_HEIGHT * 10)));
    label.show();
    //QString filename = QString::asprintf("/mnt/ramdisk/%08d.png", epoch);
    //pixmap.save(filename);

    return a.exec();
}
