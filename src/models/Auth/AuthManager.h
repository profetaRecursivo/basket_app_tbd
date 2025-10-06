#pragma once
#include <QString>
#include <libpq-fe.h>

class AuthManager {
public:
  AuthManager();

  // Valida credenciales y opcionalmente retorna la conexión activa
  // outConn: si no es nullptr, se retorna la conexión para reutilizar
  // Retorna: user_id si éxito, 0 si credenciales incorrectas, -1 si error
  static int validate(const QString &user, const QString &password,
                      PGconn **outConn = nullptr);
  static void insertPID(int id_userN, int PID, PGconn *conn);
};
