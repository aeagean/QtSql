#include "SqlService.h"
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlDriver>

SqlService::SqlService()
{

}

SqlService::~SqlService()
{
    m_sqlDatabase.close();
}

bool SqlService::open(const QString &name, const QString &type)
{
    if (QSqlDatabase::contains(name))
    {
        m_sqlDatabase = QSqlDatabase::database(name);
        this->setLastError(QString("Existing %1 database.").arg(name), Warning);
    }
    else {
        /* 添加数据库驱动 */
        m_sqlDatabase = QSqlDatabase::addDatabase(type, name);
        /* 数据库连接命名 */
        m_sqlDatabase.setDatabaseName(name);
    }

    /* 打开数据库 */
    if(!m_sqlDatabase.open()) {
        setLastError(m_sqlDatabase.lastError().text(), Error);
        return false;
    }
    else {
        this->setLastError(QString("The database was opened successfully."), Info);
    }

    /* 以下执行相关QSL语句 */
    m_sqlQuery = QSqlQuery(m_sqlDatabase);
    return true;
}

bool SqlService::createTable(const QString &table, const QMap<QString, QString> &map)
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
        setLastError(QString("Exist table<%1>!").arg(table), Warning);
        return false;
    }
}

bool SqlService::insertRowTable(const QString &table, const QVariantMap &map)
{
    QMap<QString, QString> tableContentMap;

    if (!isTableExist(table)) {
        setLastError(QString("Not find %1 table!").arg(table), Error);
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

/* ALTER TABLE table_name ADD column_name datatype */
bool SqlService::insertColumnTable(const QString &table, const QString &clumnKey, const QString &type)
{
    QString content = QString("alter table %1 add %2 %3").arg(table).arg(clumnKey).arg(type);
    return this->exec(content);
}

/* UPDATE 表名称 SET 列名称 = 新值 WHERE 列名称 = 某值 */
bool SqlService::updateRowTable(const QString &table, const QString &clumnKey, const QString &rowValue, const QVariantMap &map)
{
    QString content = QString("update %1 set ").arg(table);

    QMapIterator<QString, QVariant> i(map);
    while (i.hasNext()) {
        i.next();

        if (i.hasNext())
            content += QString("%1 = '%2', ").arg(i.key()).arg(i.value().toString());
        else
            content += QString("%1 = '%2' ").arg(i.key()).arg(i.value().toString());
    }

    content += QString("where %1 = %2").arg(clumnKey).arg(rowValue);

    return this->exec(content);
}

bool SqlService::deleteRowTable(const QString &table, const QString &columnKey, const QString &rowValue)
{
    QString deleteContent = QString("delete from %1 where %2 = %3").arg(table).arg(columnKey).arg(rowValue);
    return this->exec(deleteContent);
}

bool SqlService::deleteColumnTable(const QString &table, const QString &columnKey)
{
    /* ALTER TABLE table_name DROP COLUMN column_name
     * QSQLITE不支持
     */
    if (m_sqlDatabase.driverName() != "QSQLITE") {
        QString content = QString("alter table %1 drop column %2").arg(table).arg(columnKey);
        return this->exec(content);
    }
    else {
        return false;
    }
}

bool SqlService::sortTable(const QString &table, const QString &columnKey)
{
    /* select * from table order by target */
    QString sortContent = QString("select * from %1 order by %2").arg(table).arg(columnKey);
    return this->exec(sortContent);
}

QList<QVariantMap> SqlService::getValues(const QString &table, int page, int pageNum)
{
    m_sqlQuery.exec(QString("select * from %1").arg(table));

    QList<QVariantMap> list;
    if (!m_sqlQuery.seek(page)) {
        setLastError("[The number of pages is beyond the limit]", Error);
        return list;
    }

    do {
        QVariantMap map;
        for (int i = 0; i < m_sqlQuery.record().count(); i++)
            map.insert(m_sqlQuery.record().field(i).name(),
                       m_sqlQuery.record().field(i).value());
        list.append(map);

    }
    while(m_sqlQuery.next() && --pageNum);

    return list;
}

QList<QVariantMap> SqlService::getAllValues(const QString &table)
{
    return getValues(table, 0, size(table));
}

int SqlService::size(const QString &table)
{
    m_sqlQuery.exec(QString("select * from %1").arg(table));
    int size = -1;
    while (m_sqlQuery.next()) {
        /* 驱动支持返回记录数 */
        if (m_sqlQuery.driver()->hasFeature(QSqlDriver::QuerySize)) {
            size = m_sqlQuery.size();
            break;
        }
        else { /* 驱动不支持返回记录数，只能循环查找 */
            m_sqlQuery.last();
            size = m_sqlQuery.at() + 1;
        }
    }

    m_sqlQuery.first();
    return size;
}

QString SqlService::lastError()
{
    return m_lastError;
}

void SqlService::close()
{
    m_sqlDatabase.close();
}

QSqlQuery &SqlService::getSqlQuery()
{
    return m_sqlQuery;
}

QSqlDatabase &SqlService::getSqlDatabase()
{
    return m_sqlDatabase;
}

bool SqlService::isTableExist(const QString &table)
{
    return m_sqlDatabase.tables().contains(table);
}

bool SqlService::prepare(const QString &query)
{
    if (!m_sqlQuery.prepare(query)) {
        setLastError(m_sqlQuery.lastError().text(), Error);
        return false;
    }
    else {
        return true;
    }
}

bool SqlService::exec(const QString &query)
{
    if (!m_sqlQuery.exec(QString(query))) {
        setLastError(m_sqlQuery.lastError().text(), Error);
        return false;
    }
    else {
        return true;
    }
}

bool SqlService::exec()
{
    if (!m_sqlQuery.exec()) {
        setLastError(m_sqlQuery.lastError().text(), Error);
        return false;
    }
    else {
        return true;
    }
}

QMap<QString, QString> SqlService::getTableInfo(const QString &table)
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
        setLastError(m_sqlQuery.lastError().text(), Error);
        return tableMap;
    }
}

void SqlService::setLastError(const QString &lastError, SqlService::LoggerType type)
{
    if (lastError == "")
        return;

    m_lastError = lastError;

    if (type == Error)
        qDebug()<<"[ Error ]"<<m_lastError;
    else if (type == Warning)
        qDebug()<<"[Warning]"<<m_lastError;
    else if (type == Info)
        qDebug()<<"[  Info ]"<<m_lastError;
    else if (type == Warning)
        qDebug()<<"[Warning]"<<m_lastError;
    else if (type == Debug)
        qDebug()<<"[ Debug ]"<<m_lastError;
    else if (type == Trace)
        qDebug()<<"[ Trace ]"<<m_lastError;
}


