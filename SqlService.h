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
    bool insertRowTable(QString table, QMap<QString, QVariant> map);

    /* 删除一行，arg1(表名称)，arg2(列名称)，arg3(值)
     * DELETE FROM 表名称 WHERE 列名称 = 值
     */
    bool deleteRowTable(QString table, QString columnName, QString value);

    /* select * from table order by target;
     * 升序排列
     */
    bool sortTable(QString table, QString target);

    /* 获取数据库的查询结果，
     * 一般是调用exec命令后再调用该函数，
     * 不然就导致返回结果是上一次就结果,
     * arg1(从那一页获取),
     * arg2(一页获取多少条数据),
     *
     */
    QList<QMap<QString, QVariant>> getValues(int page, int pageNum);

    QString lastError();

    QSqlQuery& getSqlQuery();
    QSqlDatabase& getSqlDatabase();
    bool isTableExist(QString table);

    bool prepare(const QString& query);
    bool exec(const QString& query);
    bool exec();

private:
    QMap<QString, QString> getTableInfo(QString table);

private:
    QSqlQuery m_sqlQuery;
    QSqlDatabase m_sqlDatabase;
    QString m_lastError;
};

#endif // SQLSERVICE_H
