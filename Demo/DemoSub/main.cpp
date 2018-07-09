#include "SqlService.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SqlService sql;
    sql.open("test.db");

    return a.exec();
}
