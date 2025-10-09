#pragma once
#include <QPair>
#include <QString>
#include <libpq-fe.h>

class AuthManager {
public:
  AuthManager();
  static QPair<int, int> validate(const QString &user, const QString &password,
                                  PGconn **outConn = nullptr);
  static void insertPID(int id_userN, int PID, PGconn *conn);
  
  static void closeSession(PGconn *conn, int backendPID);
};
