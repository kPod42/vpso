#include "ui\mainwindow.h"
#include "data\databasemanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

    if (!DatabaseManager::initSchema())
        qFatal("Failed to init DB schema");

}
