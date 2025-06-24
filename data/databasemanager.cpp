// databasemanager.cpp
#include "databasemanager.h"
#include <QFile>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QSqlError>

QSqlDatabase DatabaseManager::db()
{
    static QSqlDatabase db = []{
        const QString path =
            QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
            + "/experiments.db";
        QSqlDatabase d = QSqlDatabase::addDatabase("QSQLITE");
        d.setDatabaseName(path);
        d.open();
        return d;
    }();
    return db;
}

bool DatabaseManager::initSchema()
{
    QFile f(":/sql/init.sql");
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QSqlQuery q(db());
    for (const QString &stmt :
         QString::fromUtf8(f.readAll()).split(';', Qt::SkipEmptyParts))
        q.exec(stmt.trimmed());

    return !q.lastError ().isValid();
}
