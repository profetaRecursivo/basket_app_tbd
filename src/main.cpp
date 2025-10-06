#include "models/Auth/AuthManager.h"
#include "views/login/LoginWindow.h"
#include "views/main/MainWindow.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[]) {
  QApplication core(argc, argv);
  AuthManager auth;
  LoginWindow *login = new LoginWindow();

  QObject::connect(login, &LoginWindow::loginRequested,
                   [&](const QString &user, const QString &pass) {
                     int userId = auth.validate(user, pass);
                     if (userId > 0) {
                       MainWindow *mainWindow = new MainWindow(userId);
                       mainWindow->show();
                       login->close();
                       login->deleteLater();
                     } else if (userId == 0) {
                       QMessageBox::warning(login, "Login",
                                            "Usuario o contraseña incorrectos");
                     } else {
                       QMessageBox::warning(login, "Login",
                                            "No se conectó a la BD");
                     }
                   });
  login->show();
  return core.exec();
}
