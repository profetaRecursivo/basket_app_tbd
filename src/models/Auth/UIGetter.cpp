#include "UIGetter.h"
#include "DbManager.h"
#include <QDebug>

UIGetter::UIGetter() {}

QVector<int> UIGetter::getUIIds(int userId, PGconn *conn) {
  QVector<int> uiIds;

  if (!conn) {
    qDebug() << "UIGetter::getUIIds - Error: conexion nula";
    return uiIds;
  }

  const char *sql = "SELECT id_ui FROM get_ui_ids_by_user_id($1)";
  QByteArray bUserId = QByteArray::number(userId);
  const char *paramValues[1] = {bUserId.constData()};

  PGresult *res =
      PQexecParams(conn, sql, 1, nullptr, paramValues, nullptr, nullptr, 0);

  if (!res or PQresultStatus(res) != PGRES_TUPLES_OK) {
    qDebug() << "UIGetter::getUIIds - Error en consulta:"
             << PQerrorMessage(conn);
    if (res)
      PQclear(res);
    return uiIds;
  }

  uiIds = parseUIIds(res);

  qDebug() << "UIGetter::getUIIds - Usuario" << userId << "tiene acceso a"
           << uiIds.size() << "UIs";
  for (int id : uiIds) {
    qDebug() << "  - UI ID:" << id;
  }

  PQclear(res);

  return uiIds;
}

QVector<int> UIGetter::parseUIIds(PGresult *res) {
  QVector<int> uiIds;
  int rows = PQntuples(res);

  for (int i = 0; i < rows; ++i) {
    int id_ui = atoi(PQgetvalue(res, i, 0));
    uiIds.append(id_ui);
  }

  return uiIds;
}
