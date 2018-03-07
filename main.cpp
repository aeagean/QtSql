#include "SqlService.h"
#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SqlService sql;
    sql.open("aa.db");
    QMap<QString, QString> map;
    map["id"] = "int primary key";
    map["type"] = "varchar";
    sql.createTable("bb", map);
    QMap<QString, QVariant> mapContent;
    mapContent["id"] = 2;
    mapContent["type"] = "high";
    sql.insertTable("bb", mapContent);

    qDebug()<<sql.lastError();
    return a.exec();
}
