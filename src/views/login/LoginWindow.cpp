#include "LoginWindow.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QFont>

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent),
    usernameLineEdit(new QLineEdit(this)),
    passwordLineEdit(new QLineEdit(this)),
    loginButton(new QPushButton(tr("Iniciar sesión"), this)),
    cancelButton(new QPushButton(tr("Cancelar"), this))
{
    setWindowTitle(tr("Login"));
    resize(600, 400);

    QFont titleFont("JetBrains Mono", 20, QFont::Bold);
    QFont textFont("JetBrains Mono", 12, QFont::Monospace);
    QLabel *title = new QLabel("Bienvenido", this);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);

    QLabel *userLabel = new QLabel(tr("Usuario:"), this);
    QLabel *passLabel = new QLabel(tr("Contraseña:"), this);
    userLabel->setFont(textFont);
    passLabel->setFont(textFont);
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(title);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(userLabel);
    mainLayout->addWidget(usernameLineEdit);
    mainLayout->addWidget(passLabel);
    mainLayout->addWidget(passwordLineEdit);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(loginButton);
    mainLayout->addWidget(cancelButton);

    QWidget *central = new QWidget(this);
    central->setLayout(mainLayout);
    setCentralWidget(central);

    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(cancelButton, &QPushButton::clicked, this, &LoginWindow::onCancelClicked);
}

LoginWindow::~LoginWindow() = default;

void LoginWindow::onLoginClicked() {
    const QString user = usernameLineEdit->text().trimmed();
    const QString pass = passwordLineEdit->text();

    if (user.isEmpty() or pass.isEmpty()) {
        QMessageBox::warning(this, tr("Faltan datos"), tr("Completa usuario y contraseña"));
        return;
    }
    emit loginRequested(user, pass);
}

void LoginWindow::onCancelClicked() {
    close();
}
