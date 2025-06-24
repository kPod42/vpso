// databasemanager.h
#pragma once
#include <QSqlDatabase>

class DatabaseManager
{
public:
    static QSqlDatabase db();               // единое соединение
    static bool initSchema();               // выполняет init.sql
private:
    DatabaseManager() = default;
};
