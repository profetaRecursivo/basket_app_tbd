#include "AuthManager.h"
#include "DbManager.h"
#include <QDebug>
#include <libpq-fe.h>
AuthManager::AuthManager(){

}
void AuthManager::insertPID(int id_userN, int PID, PGconn* conn) {
    const char *sql = "INSERT INTO public.\"Sesion\" (\"PID\", active, \"id_userN\") "
                      "VALUES ($1, $2, $3)";

    QByteArray bPid = QByteArray::number(PID);
    QByteArray bActive = QByteArray::number(1);
    QByteArray bUserId = QByteArray::number(id_userN);

    const char *params[3] = { bPid.constData(), bActive.constData(), bUserId.constData() };

    PGresult *res = PQexecParams(conn, sql, 3, nullptr, params, nullptr, nullptr, 0);

    if (!res or PQresultStatus(res) != PGRES_COMMAND_OK) {
        qDebug() << "insertPID - =error al insertar sesión:" << PQerrorMessage(conn);
    } else {
        qDebug() << "insertPID - sesión registrada correctamente";
    }

    if (res) PQclear(res);
}


int AuthManager::validate(const QString &user, const QString &password) {
    DBManager dbmanager;
    PGconn* conn = dbmanager.connect();
    if (!conn) {
        qDebug() << "AuthManager::validate - no se pudo conectar a la DB";
        return -1;
    }

    int backendPid = PQbackendPID(conn);

    const char *sql = "SELECT \"id_userN\" FROM public.\"UserN\" WHERE \"user\" = $1 AND password = $2";
    QByteArray bUser = user.toUtf8();
    QByteArray bPass = password.toUtf8();
    const char *params[2] = { bUser.constData(), bPass.constData() };

    PGresult *res = PQexecParams(conn, sql, 2, nullptr, params, nullptr, nullptr, 0);

    int resultCode = 0;

    if (res and PQresultStatus(res) == PGRES_TUPLES_OK and PQntuples(res) > 0) {
        int id_userN = atoi(PQgetvalue(res, 0, 0));
        insertPID(id_userN, backendPid, conn);
        resultCode = 1;
    } else {
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            qDebug() << "AuthManager::validate - error en consulta:" << PQerrorMessage(conn);
        }
        resultCode = 0;
    }

    if (res) PQclear(res);
    PQfinish(conn);
    return resultCode;
}
