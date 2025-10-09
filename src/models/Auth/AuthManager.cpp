#include "AuthManager.h"
#include "DbManager.h"
#include <QDebug>
#include <libpq-fe.h>
AuthManager::AuthManager() {}
void AuthManager::insertPID(int id_userN, int PID, PGconn *conn) {
  const char *sql = "CALL public.insert_session($1, $2)";

  QByteArray bPid = QByteArray::number(PID);
  QByteArray bUserId = QByteArray::number(id_userN);

  const char *params[2] = {bPid.constData(), bUserId.constData()};

  PGresult *res =
      PQexecParams(conn, sql, 2, nullptr, params, nullptr, nullptr, 0);

  if (!res or PQresultStatus(res) != PGRES_COMMAND_OK) {
    qDebug() << "insertPID - error al insertar sesión:" << PQerrorMessage(conn);
  } else {
    qDebug() << "insertPID - sesión registrada correctamente";
  }

  if (res)
    PQclear(res);
}

QPair<int, int> AuthManager::validate(const QString &user,
                                      const QString &password,
                                      PGconn **outConn) {
  DBManager dbmanager;
  PGconn *conn = dbmanager.connect();
  if (!conn) {
    qDebug() << "AuthManager::validate - no se pudo conectar a la DB";
    return qMakePair(-1, 0);
  }

  int backendPid = PQbackendPID(conn);
  const char *sql = "SELECT get_user_id($1, $2)";
  QByteArray bUser = user.toUtf8();
  QByteArray bPass = password.toUtf8();
  const char *params[2] = {bUser.constData(), bPass.constData()};

  PGresult *res =
      PQexecParams(conn, sql, 2, nullptr, params, nullptr, nullptr, 0);

  QPair<int, int> result;

  if (res && PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
    if (!PQgetisnull(res, 0, 0)) {
      int id_userN = atoi(PQgetvalue(res, 0, 0));
      insertPID(id_userN, backendPid, conn);
      result = qMakePair(id_userN, backendPid);
      if (outConn) {
        *outConn = conn;
      }
    } else {
      result = qMakePair(0, 0);
      PQfinish(conn);
    }
  } else {
    qDebug() << "AuthManager::validate - error en consulta:"
             << PQerrorMessage(conn);
    result = qMakePair(-1, 0);
    PQfinish(conn);
  }

  if (res)
    PQclear(res);

  return result;
}

void AuthManager::closeSession(PGconn *conn, int backendPID) {
  if (!conn)
    return;

  QString query = QString("CALL close_user_session(%1);").arg(backendPID);

  PGresult *res = PQexec(conn, query.toUtf8().constData());
  PQclear(res);
}
