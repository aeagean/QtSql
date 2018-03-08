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
    mapContent["id"] = 51;
    mapContent["type"] = "high";
//    sql.insertRowTable("bb", mapContent);
//    sql.deleteRowTable("bb", "id", "4");
//    sql.insertColumnTable("bb", "list", "varchar");
    sql.deleteColumnTable("bb", "list");
    sql.sortTable("bb", "id");

//    QList<QMap<QString, QVariant>> valuesList = sql.getValues(0, 5);
//    qDebug()<<valuesList;
    qDebug()<<sql.lastError();
    return a.exec();
}
