#include "SqlService.h"

SqlService::SqlService()
{

}

bool SqlService::open(QString name)
{
    if (QSqlDatabase::contains(name))
    {
        m_sqlDatabase = QSqlDatabase::database(name);
    }
    else {
        m_sqlDatabase = QSqlDatabase::addDatabase("QSQLITE", name);//添加数据库驱动
        m_sqlDatabase.setDatabaseName(name); //数据库连接命名
    }

    if(!m_sqlDatabase.open()) { //打开数据库
         qDebug()<<"open failed!"<<m_sqlDatabase.lastError();
         return false;
    }

    m_sqlQuery = QSqlQuery(m_sqlDatabase); //以下执行相关QSL语句
}

QSqlQuery &SqlService::getSqlQuery()
{
    return &m_sqlQuery;
}

QSqlDatabase &SqlService::getSqlDatabase()
{
    return &m_sqlDatabase;
}
