// experimentrepository.cpp
#include "experimentrepository.h"
#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>

bool ExperimentRepository::insert (const Experiment &e)
{
    QSqlQuery q(DatabaseManager::db());

    //               1  2  3  4  5     6    7      8      9
    q.prepare( "INSERT INTO experiments "
              "(R,x0,y0,C,orientation,N,S_est,error,ts) "
              "VALUES (?,?,?,?,?,?,?,?,?)" );

    q.addBindValue(e.params.R);                                      // 1
    q.addBindValue(e.params.x0);                                     // 2
    q.addBindValue(e.params.y0);                                     // 3
    q.addBindValue(e.params.C);                                      // 4
    q.addBindValue( e.params.o == SegmentMonteCarlo::Vertical        // 5
                       ? "vertical" : "horizontal");
    q.addBindValue(e.params.N);                                      // 6
    q.addBindValue(e.S_est);                                         // 7
    q.addBindValue(e.error);                                         // 8
    q.addBindValue(e.ts.toString(Qt::ISODate));                      // 9

    // быстрый лог: сколько реально привязано к запросу
    qDebug() << "placeholders =" << q.lastQuery()
             << "\nbound count =" << q.boundValues().size();

    if (!q.exec()) {
        qWarning() << "SQL insert error:" << q.lastError().text();
        return false;
    }
    return true;
}

