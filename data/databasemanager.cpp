// databasemanager.cpp
#include "databasemanager.h"
#include <QFile>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QSqlError>
#include <QDir>            // вместо "qdir.h"

QSqlDatabase DatabaseManager::db()
{
    static QSqlDatabase db = []{
        const QString baseDir =
            QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir().mkpath(baseDir);

        QSqlDatabase d = QSqlDatabase::addDatabase("QSQLITE", "main"); // имя!
        d.setDatabaseName(baseDir + "/experiments.db");
        if (!d.open())
            qWarning() << "DB open error:" << d.lastError();
        return d;
    }();
    return db;
}

bool DatabaseManager::initSchema()
{
    QFile f(":/databaseScripts/sql/init.sql");
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "init.sql NOT found in resources!";
        return false;
    }

    QSqlDatabase d = DatabaseManager::db();
    if (!d.isOpen()) {
        qCritical() << "DB is NOT open:" << d.lastError();
        return false;
    }
    qDebug() << "DB path:" << d.databaseName();

    QStringList stmts = QString::fromUtf8(f.readAll())
                            .split(';', Qt::SkipEmptyParts);

    QSqlQuery q(d);
    bool ok = true;               //  ←  ОБЯЗАТЕЛЬНО!

    for (QString s : stmts) {
        s = s.trimmed();
        if (s.isEmpty())
            continue;

        /* убираем строки-комментарии "-- …" */
        while (s.startsWith("--")) {
            int nl = s.indexOf('\n');
            if (nl < 0) { s.clear(); break; }
            s = s.mid(nl + 1).trimmed();
        }
        if (s.isEmpty())
            continue;

        if (!q.exec(s)) {
            qWarning() << "SQL error:" << q.lastError().text()
            << "\nCmd:" << s;
            ok = false;           // фиксируем ошибку
        }
    }

    // финальная проверка: реально ли таблица создана?
    QSqlQuery chk("PRAGMA table_info(experiments);", d);
    qDebug() << "---- table_info(experiments) ----";
    while (chk.next())
        qDebug() << chk.value(1).toString();

    return true;
}

