#include "core/loghandler.h"
#include "data/databasemanager.h"
#include <QApplication>
#include "ui/mainwindow.h"
#include "qsqlerror.h"
#include "ui/mainwindow.h"
#include "ui/splashdialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qInstallMessageHandler(logHandler);

    QCoreApplication::setOrganizationName("MyLab");   // ← любое
    QCoreApplication::setApplicationName("VPSO");

    if (!DatabaseManager::initSchema())
        qWarning() << "DB schema init failed:" << DatabaseManager::db().lastError();
    qDebug() << "calling initSchema...";
    bool okInit = DatabaseManager::initSchema();
    qDebug() << "init ok =" << okInit;
    qDebug() << "connectionName =" << DatabaseManager::db().connectionName();


    /* 1. показываем заставку  */
    SplashDialog splash(2000);    // держим 2 с
    splash.show();
    app.processEvents();            // показываем сразу
    QObject::connect(&splash, &SplashDialog::closed, [&]{
        MainWindow *w = new MainWindow; // создаём
        w->show();                      // и показываем
    });

    qDebug() << "DB file:" << QStandardPaths::writableLocation(
                                  QStandardPaths::AppDataLocation)
                                  + "/experiments.db";

    return app.exec();
}
