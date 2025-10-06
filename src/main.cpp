#include "models/Auth/AuthManager.h"
#include "views/login/LoginWindow.h"
#include "views/main/MainWindow.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[]) {
  QApplication core(argc, argv);
  AuthManager auth;
  LoginWindow *login = new LoginWindow();

  QObject::connect(
      login, &LoginWindow::loginRequested,
      [&](const QString &user, const QString &pass) {
        PGconn *conn = nullptr;
        int userId = auth.validate(user, pass, &conn);

        if (userId > 0) {
          MainWindow *mainWindow = new MainWindow(userId, nullptr, conn);
          mainWindow->show();
          login->close();
          login->deleteLater();
        } else if (userId == 0) {
          QMessageBox::warning(login, "Login",
                               "Usuario o contraseña incorrectos");
        } else {
          QMessageBox::warning(login, "Login", "No se pudo conectar a la BD");
        }
      });
  login->show();
  return core.exec();
}
