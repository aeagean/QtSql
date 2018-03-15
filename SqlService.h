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
    bool insertRowTable(QString table, QVariantMap map);
    /* ALTER TABLE table_name ADD column_name datatype
     * 插入一列，arg(表格名字)，arg(变量)，arg(变量类型)
     */
    bool insertColumnTable(QString table, QString name, QString type);

    /* 更新表格(table)的一行(name字段)的数据(QVariantMap);
     * UPDATE 表名称 SET 列名称 = 新值 WHERE 列名称 = 某值
     */
    bool updateRowTable(QString table, QString name, QVariantMap map);

    /* 更新表格(table)的一行(name字段)的数据(QVariantMap);
     * key,value是针对需要全部修改的，对应的列key,对应的行value
     * UPDATE 表名称 SET 列名称 = 新值 WHERE 列名称 = 某值
     */
    bool updateRowTable(QString table, QString targetKey, QString targetValue, QVariantMap map);

    /* 删除一行，arg1(表名称)，arg2(列名称(比如id))，arg3(值)(列(id)所对应的值)
     * DELETE FROM 表名称 WHERE 列名称 = 值
     */
    bool deleteRowTable(QString table, QString columnName, QString value);


    /* ALTER TABLE table_name DROP COLUMN column_name
     * 某些数据库系统不允许这种在数据库表中删除列的方式 (DROP COLUMN column_name)。
     */
    bool deleteColumnTable(QString table, QString columnName);

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
    QList<QVariantMap> getValues(int page, int pageNum);

    /* 获取个数 */
    int size();

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
