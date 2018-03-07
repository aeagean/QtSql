#ifndef SQLSERVICE_H
#define SQLSERVICE_H

#include <QSqlQuery>
#include <QVariantMap>

class SqlService
{
public:
    SqlService();

    /* 打开sql文件，没有sql文件则创建一个对应数据库文件 */
    bool open(QString name);
    /* 创建一个table, 传入一个key, value，对应顺序的变量名字（key），类型（value） */
    bool createTable(QString table, QMap<QString, QString> map);
    bool insertTable(QString table, QMap<QString, QVariant> map);

    QString lastError();

    QSqlQuery& getSqlQuery();
    QSqlDatabase& getSqlDatabase();
    bool isTableExist(QString table);


private:
    QMap<QString, QString> getTableInfo(QString table);

private:
    QSqlQuery m_sqlQuery;
    QSqlDatabase m_sqlDatabase;
    QString m_lastError;
    /* key -> table; value: map */
//    QMap<QString, QMap<QString, QString>> m_tableMap;
};

#endif // SQLSERVICE_H
