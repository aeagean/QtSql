#include "SqlService.h"
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlField>

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
        /* 添加数据库驱动 */
        m_sqlDatabase = QSqlDatabase::addDatabase("QSQLITE", name);
        /* 数据库连接命名 */
        m_sqlDatabase.setDatabaseName(name);
    }

    /* 打开数据库 */
    if(!m_sqlDatabase.open()) {
         m_lastError = m_sqlDatabase.lastError().text();
         return false;
    }

    /* 以下执行相关QSL语句 */
    m_sqlQuery = QSqlQuery(m_sqlDatabase);
    return true;
}

bool SqlService::createTable(QString table, QMap<QString, QString> map)
{
    if(!isTableExist(table)) {
        QString tableList = QString("create table %1 (").arg(table);
        QMapIterator<QString, QString> i(map);
        while (i.hasNext()) {
            i.next();
            tableList.append(QString("%1 %2").arg(i.key()).arg(i.value()));
            if (i.hasNext())
                tableList.append(", ");
        }
        tableList.append(")");
        return this->exec(tableList);

    }
    else {
        m_lastError = QString("Exist table<%1>!").arg(table);
        return false;
    }
}

bool SqlService::insertRowTable(QString table, QMap<QString, QVariant> map)
{
    QMap<QString, QString> tableContentMap;

    if (!isTableExist(table)) {
        m_lastError = QString("Not find %1 table!").arg(table);
        return false;
    }
    else {
        tableContentMap = getTableInfo(table);
        if (tableContentMap.isEmpty())
            return false;
    }

    QString insertTableContent = QString("insert into %1 (").arg(table);
    QString values = QString("values (");

    QMapIterator<QString, QString> i(tableContentMap);
    while (i.hasNext()) {
        i.next();
        insertTableContent.append(QString("%1").arg(i.key()));
        values.append("?");
        if (i.hasNext()) {
            insertTableContent.append(", ");
            values.append(", ");
        }
    }
    insertTableContent.append(") ");
    values.append(")");

    insertTableContent.append(values);

    if (!this->prepare(insertTableContent)) {
        return false;
    }

    QMapIterator<QString, QString> ii(tableContentMap);
    while (ii.hasNext()) {
        ii.next();
        m_sqlQuery.addBindValue(map[ii.key()]);
    }

    return this->exec();
}

bool SqlService::deleteRowTable(QString table, QString columnName, QString value)
{
    QString deleteContent = QString("delete from %1 where %2 = %3").arg(table).arg(columnName).arg(value);
    return this->exec(deleteContent);
}

bool SqlService::sortTable(QString table, QString target)
{
    /* select * from table order by target */
    QString sortContent = QString("select * from %1 order by %2").arg(table).arg(target);
    return this->exec(sortContent);
}

QList<QMap<QString, QVariant>> SqlService::getValues(int page, int pageNum)
{
    QList<QMap<QString, QVariant>> list;
    if (!m_sqlQuery.seek(page)) {
        m_lastError = "getValues error![The number of pages is beyond the limit]";
        return list;
    }

    do {
        QMap<QString, QVariant> map;
        for (int i = 0; i < m_sqlQuery.record().count(); i++)
            map.insert(m_sqlQuery.record().field(i).name(),
                       m_sqlQuery.record().field(i).value());
        list.append(map);

    }
    while(m_sqlQuery.next() && --pageNum);

    return list;
}

QString SqlService::lastError()
{
    return m_lastError;
}

QSqlQuery &SqlService::getSqlQuery()
{
    return m_sqlQuery;
}

QSqlDatabase &SqlService::getSqlDatabase()
{
    return m_sqlDatabase;
}

bool SqlService::isTableExist(QString table)
{
    return m_sqlDatabase.tables().contains(table);
}

bool SqlService::prepare(const QString &query)
{
    if (!m_sqlQuery.prepare(query)) {
        m_lastError = m_sqlQuery.lastError().text();
        return false;
    }
    else {
        return true;
    }
}

bool SqlService::exec(const QString &query)
{
    if (!m_sqlQuery.exec(QString(query))) {
        m_lastError = m_sqlQuery.lastError().text();
        return false;
    }
    else {
        return true;
    }
}

bool SqlService::exec()
{
    if (!m_sqlQuery.exec()) {
        m_lastError = m_sqlQuery.lastError().text();
        return false;
    }
    else {
        return true;
    }
}

QMap<QString, QString> SqlService::getTableInfo(QString table)
{
    QMap<QString, QString> tableMap;

    QString str = "PRAGMA table_info(" + table + ")";
    m_sqlQuery.prepare(str);

    if (m_sqlQuery.exec())
    {
        while (m_sqlQuery.next())
        {
            /* value(0) = 字段名; value(1) = 字段类型 */
            tableMap[m_sqlQuery.value(1).toString()] = m_sqlQuery.value(2).toString();
        }
        return tableMap;
    }
    else
    {
        m_lastError = m_sqlQuery.lastError().text();
        return tableMap;
    }
}

