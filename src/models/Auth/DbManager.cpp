#include "DbManager.h"
#include <libpq-fe.h>
#include <qdebug.h>
DBManager::DBManager() {

}
PGconn* DBManager::connect() {
    const char* host = "localhost";
    const char* dbname = "basket";
    const char* user = "jschavarria";
    const char* pass = "segmenttree";

    QString connStr = QString("host=%1 port=%2 dbname=%3 user=%4 password=%5")
                          .arg(host).arg(9020).arg(dbname).arg(user).arg(pass);

    PGconn* conn = PQconnectdb(connStr.toUtf8().constData());

    if (!conn or PQstatus(conn) != CONNECTION_OK) {
        qDebug() << "Error conectando a la DB:" << (conn ? PQerrorMessage(conn) : "salio un nullptr");
        if (conn) PQfinish(conn);
        return nullptr;
    }

    qDebug() << "Conexión a la DB exitosa.";
    return conn;
}
