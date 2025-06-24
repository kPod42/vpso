// experimentrepository.cpp
#include "experimentrepository.h"
#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>

bool ExperimentRepository::insert(const Experiment &e)
{
    QSqlQuery q(DatabaseManager::db());
    q.prepare(R"(
        INSERT INTO experiments
        (R,x0,y0,C,orientation,N,S_est,error)
        VALUES (?,?,?,?,?,?,?,?)
    )");
    q.addBindValue(e.params.R);
    q.addBindValue(e.params.x0);
    q.addBindValue(e.params.y0);
    q.addBindValue(e.params.C);
    q.addBindValue(e.params.o == SegmentMonteCarlo::Vertical
                       ? "vertical" : "horizontal");
    q.addBindValue(e.params.N);
    q.addBindValue(e.S_est);
    q.addBindValue(e.error);
    return q.exec();
}
