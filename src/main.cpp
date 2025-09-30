#include <QApplication>
#include "views/login/LoginWindow.h"
#include "models/Auth/AuthManager.h"
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication core(argc, argv);
    AuthManager auth;
    LoginWindow login;

    QObject::connect(&login, &LoginWindow::loginRequested,
                     [&](const QString &user, const QString &pass){
                         int ok = auth.validate(user, pass);
                         if (ok == 1) {
                             QMessageBox::information(&login, "Login", "Login exitoso");
                             login.close();
                         } else if(ok == 0){
                             QMessageBox::warning(&login, "Login", "Usuario o contraseña incorrectos");
                         } else {
                             QMessageBox::warning(&login, "Login", "No se conectó a la BD");
                         }
                     });
        login.show();
    return core.exec();
}
