#include <QCoreApplication>
#include "gamemanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    GameManager GameManager;
    return a.exec();
}
