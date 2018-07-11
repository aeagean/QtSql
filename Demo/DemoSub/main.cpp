#include "SqlService.h"
#include <QCoreApplication>

static const QString dbFile = "data.db";
static const QString testTable = "test";
const QMap<QString, QString> testTableTitles = {
    {"title1", "real"},
    {"title2", "real"},
    {"title3", "int"},
    {"title4", "varchar"},
    {"title5", "varchar"}
};

const QMap<QString, QString> newTestTableTitles = {
    {"title1", "real"},
    {"title2", "real"},
//    {"title3", "int"},
//    {"title4", "varchar"},
//    {"title5", "varchar"},
//    {"title6", "varchar"}
};


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SqlService sql;
    sql.open(dbFile);
    sql.createTable(testTable, testTableTitles);
    sql.replaceTable(testTable, newTestTableTitles, SqlService::OverWrite);
    return a.exec();
}
