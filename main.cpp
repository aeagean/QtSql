#include "SqlService.h"
#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SqlService sql;
    /* 创建数据库 */
    sql.open("aa.db");
    /* 创建数据库 */

    /* 初始化表格名字为bb ，列1有id,列2有type*/
    QMap<QString, QString> map;
    map["id"] = "int primary key";
    map["type"] = "varchar";
    sql.createTable("bb", map);
    /* 初始化表格名字为bb ，列1有id,列2有type*/

    /* 向aa.db数据库的表格bb插入一行 */
    QMap<QString, QVariant> mapContent;
    mapContent["id"] = 51;
    mapContent["type"] = "high";
    sql.insertRow("bb", mapContent);
    /* 向aa.db数据库的表格bb插入一行 */

    /* 向aa.db数据库的表格bb删除以id为51值得一行 */
    sql.deleteRow("bb", "id", "51");
    /* 向aa.db数据库的表格bb删除以id为51值得一行 */

    /* 插入一列，列名为list,列名的类型是varchar相当于c++的char* */
    sql.insertColumn("bb", "list", "varchar");
    /* 插入一列，列名为list,列名的类型是varchar相当于c++的char* */

    /* 删除一列，列名为list*/
    sql.deleteColumn("bb", "list");
    /* 删除一列，列名为list*/

    /* 排序表格bb,以id排序 */
    sql.sort("bb", "id");
    /* 排序表格bb,以id排序 */

    /* 向aa.db数据库的表格bb插入一行 */
    mapContent["id"] = 61;
    mapContent["type"] = "high";
    sql.insertRow("bb", mapContent);
    /* 向aa.db数据库的表格bb插入一行 */

    /* 更新表格bb中的行值 */
    QMap<QString, QVariant> testContent;
    testContent["id"] = 61;
    testContent["type"] = "low";

    sql.updateRow("bb", "id", "51", testContent);
    /* 更新表格bb中的行值 */

    /* 输出最后的错误，任何地方都可以调用 */
    qDebug()<<sql.lastError();
    /* 输出最后的错误，任何地方都可以调用 */
    qDebug()<<sql.size("bb")<<sql.getAllValues("bb");

    return a.exec();
}
