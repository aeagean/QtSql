#ifndef SQLSERVICE_H
#define SQLSERVICE_H

#include <QSqlQuery>
#include <QVariantMap>

class SqlService
{
public:
    enum LoggerType {
        Error = 0,
        Warning,
        Info,
        Debug,
        Trace
    };

    enum ReplaceTableMode {
        Append = 0,
        OverWrite
    };

    SqlService();
    virtual ~SqlService();

    /* 打开sql文件，没有sql文件则创建一个对应数据库文件 */
    bool open(const QString &name, const QString &type = "QSQLITE");

    /* 创建一个table, 传入一个key, value，对应顺序的变量名字（key），类型（value） */
    bool createTable(const QString &table, const QMap<QString, QString> &titles);

    bool replaceTable(const QString &table, const QMap<QString, QString> &titles, ReplaceTableMode mode = OverWrite);

    bool insertRow(const QString &table, const QVariantMap &map);

    bool insertRow(const QString &table, const QList<QVariantMap> &maps);

    /* ALTER TABLE table_name ADD column_name datatype
     * 插入一列，arg(表格名字)，arg(变量)，arg(变量类型)
     */
    bool insertColumn(const QString &table, const QString &clumnKey, const QString &type);

    /* 更新表格(table)的一行(name字段)的数据(QVariantMap);
     * key,value是针对需要全部修改的，对应的列key,对应的行value
     * UPDATE 表名称 SET 列名称 = 新值 WHERE 列名称 = 某值 & update 表名 set 字段=新值,… where 条件
     */
    bool updateRow(const QString &table, const QString &clumnKey, const QString &rowValue, const QVariantMap &map);

    /* 删除一行，arg1(表名称)，arg2(列名称(比如id))，arg3(值)(列(id)所对应的值)
     * DELETE FROM 表名称 WHERE 列名称 = 值
     */
    bool deleteRow(const QString &table, const QString &columnKey, const QString &rowValue);


    /* ALTER TABLE table_name DROP COLUMN column_name
     * 某些数据库系统不允许这种在数据库表中删除列的方式 (DROP COLUMN column_name)。
     */
    bool deleteColumn(const QString &table, const QString &columnKey);

    /* select * from table order by target;
     * 升序排列
     */
    bool sort(const QString &table, const QString &columnKey);

    /* 获取数据库的查询结果，
     * 一般是调用exec命令后再调用该函数，
     * 不然就导致返回结果是上一次就结果,
     * arg1(从那一页获取)相当于seek偏移数据, 最小值为0,
     * arg2(一页获取多少条数据)， 最小值为1,
     *
     */
    QList<QVariantMap> getValues(const QString &table, int page, int pageNum);
    QList<QVariantMap> getAllValues(const QString &table);

    /* 获取个数 */
    int size(const QString &table);
    void close();

    const QString &lastError();

protected:
    QSqlQuery &getSqlQuery();
    QSqlDatabase &getSqlDatabase();
    bool isTableExist(const QString &table);

    bool prepare(const QString& query);
    bool exec(const QString& query);
    bool exec();

private:
    QMap<QString, QString> getTableInfo(const QString &table);
    void setLastError(const QString &lastError, LoggerType type = Warning);
    void testSupportDBType();

private:
    QSqlQuery m_sqlQuery;
    QSqlDatabase m_sqlDatabase;
    QString m_lastError;
};

#endif // SQLSERVICE_H
