#include <libpq-fe.h>

class DBManager
{
public:
    DBManager();
    static PGconn* connect();
};

