#pragma once
#include <QMainWindow>

class QLineEdit;
class QPushButton;

class LoginWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

signals:
    void loginRequested(const QString &user, const QString &pass);

private slots:
    void onLoginClicked();
    void onCancelClicked();

private:
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;
    QPushButton *cancelButton;
};
