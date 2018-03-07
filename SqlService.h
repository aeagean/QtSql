#ifndef SQLSERVICE_H
#define SQLSERVICE_H

#include <QSqlQuery>

class SqlService
{
public:
    SqlService();

    /* 打开sql文件，没有sql文件则创建一个对应数据库文件 */
    bool open(QString name);


    QSqlQuery& getSqlQuery();
    QSqlDatabase& getSqlDatabase();

private:
    QSqlQuery m_sqlQuery;
    QSqlDatabase m_sqlDatabase;
};

#endif // SQLSERVICE_H
