#include <QString>
#include <libpq-fe.h>

class AuthManager {
public:
    AuthManager();

    static int validate(const QString &user, const QString &password);
    static void insertPID(int id_userN, int PID, PGconn* conn);
};

