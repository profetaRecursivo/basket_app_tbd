#include "AthleteManager.h"
#include <QDebug>

QVector<QPair<int, QString>> AthleteManager::getCountries(PGconn *conn) {
  QVector<QPair<int, QString>> countries;

  if (!conn) {
    qDebug() << "AthleteManager::getCountries - conexión nula";
    return countries;
  }

  const char *sql = "SELECT * FROM get_all_countries();";
  PGresult *res = PQexec(conn, sql);

  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    qDebug() << "AthleteManager::getCountries - error:" << PQerrorMessage(conn);
    PQclear(res);
    return countries;
  }

  int rows = PQntuples(res);
  for (int i = 0; i < rows; i++) {
    int id = atoi(PQgetvalue(res, i, 0));
    QString nombre = QString::fromUtf8(PQgetvalue(res, i, 1));
    countries.append(qMakePair(id, nombre));
  }

  PQclear(res);
  qDebug() << "AthleteManager::getCountries - cargados" << rows << "países";
  return countries;
}

QPair<QString, QString>
AthleteManager::insertAthlete(const QString &nombres, const QString &apellidos,
                              const QDate &fechaNacimiento, char sexo,
                              int idPais, double altura, double peso,
                              PGconn *conn, int backendPid) {

  QPair<QString, QString> credentials;

  if (!conn) {
    qDebug() << "AthleteManager::insertAthlete - conexión nula";
    return credentials;
  }

  QByteArray bNombres = nombres.toUtf8();
  QByteArray bApellidos = apellidos.toUtf8();
  QByteArray bFecha = fechaNacimiento.toString("yyyy-MM-dd").toUtf8();
  QByteArray bSexo = QByteArray(1, sexo);
  QByteArray bIdPais = QByteArray::number(idPais);
  QByteArray bAltura = QByteArray::number(altura, 'f', 2);
  QByteArray bPeso = QByteArray::number(peso, 'f', 2);

  const char *params[7] = {bNombres.constData(), bApellidos.constData(),
                           bFecha.constData(),   bSexo.constData(),
                           bIdPais.constData(),  bAltura.constData(),
                           bPeso.constData()};

  const char *sql = "SELECT * FROM insert_athlete($1, $2, $3, $4, $5, $6, $7);";

  PGresult *res =
      PQexecParams(conn, sql, 7, nullptr, params, nullptr, nullptr, 0);

  if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
    qDebug() << "AthleteManager::insertAthlete - error:"
             << PQerrorMessage(conn);
    PQclear(res);
    return credentials;
  }

  QString generatedUser = QString::fromUtf8(PQgetvalue(res, 0, 0));
  QString generatedPassword = QString::fromUtf8(PQgetvalue(res, 0, 1));

  credentials.first = generatedUser;
  credentials.second = generatedPassword;

  PQclear(res);

  qDebug() << "AthleteManager::insertAthlete - atleta registrado exitosamente"
           << "Usuario:" << generatedUser;

  return credentials;
}
