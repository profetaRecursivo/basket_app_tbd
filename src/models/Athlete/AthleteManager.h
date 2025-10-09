#pragma once
#include <QDate>
#include <QPair>
#include <QString>
#include <QVector>
#include <libpq-fe.h>

class AthleteManager {
public:
  static QVector<QPair<int, QString>> getCountries(PGconn *conn);

  static QPair<QString, QString>
  insertAthlete(const QString &nombres, const QString &apellidos,
                const QDate &fechaNacimiento, char sexo, int idPais,
                double altura, double peso, PGconn *conn, int backendPid);
};