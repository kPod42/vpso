#include "data/databasemanager.h"
#include <QApplication>
#include "ui/mainwindow.h"
#include "qsqlerror.h"
#include "ui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("MyLab");   // ← любое
    QCoreApplication::setApplicationName("VPSO");

    if (!DatabaseManager::initSchema())
        qWarning() << "DB schema init failed:" << DatabaseManager::db().lastError();
    qDebug() << "calling initSchema...";
    bool okInit = DatabaseManager::initSchema();
    qDebug() << "init ok =" << okInit;
    qDebug() << "connectionName =" << DatabaseManager::db().connectionName();


    MainWindow w;
    w.show();

    qDebug() << "DB file:" << QStandardPaths::writableLocation(
                                  QStandardPaths::AppDataLocation)
                                  + "/experiments.db";

    return app.exec();
}
