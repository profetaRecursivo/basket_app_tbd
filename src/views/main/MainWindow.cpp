#include "MainWindow.h"
#include "../../models/Auth/UIGetter.h"
#include <QDebug>
#include <QMessageBox>
#include <cstdio>

MainWindow::MainWindow(int userId, QWidget *parent, PGconn *conn)
    : QMainWindow(parent), m_userId(userId), m_conn(conn) {  
  this->setAttribute(Qt::WA_DeleteOnClose);
  setWindowTitle("Basquet App TBD");
  resize(600, 400);

  QWidget *central = new QWidget(this);
  setCentralWidget(central);

  m_layout = new QVBoxLayout(central);
  m_layout->setSpacing(15);
  m_layout->setContentsMargins(30, 30, 30, 30);

  QLabel *title = new QLabel("🏀 Basquet App", this);
  QFont titleFont("JetBrains Mono", 18, QFont::Bold);
  title->setFont(titleFont);
  title->setAlignment(Qt::AlignCenter);
  m_layout->addWidget(title);

  m_layout->addSpacing(20);

  initializeUIRegistry();

  QVector<int> allowedUIIds = UIGetter::getUIIds(m_userId, m_conn);

  buildUI(allowedUIIds);

  m_layout->addStretch();
}

MainWindow::~MainWindow() {
  if (m_conn) {
    PQfinish(m_conn);
    m_conn = nullptr;
  }
}

void MainWindow::initializeUIRegistry() {
  m_uiRegistry[UI_DASHBOARD_ATLETA] = {
      "📊 Dashboard Individual",
      "background-color: #4CAF50; color: white; border-radius: 5px;",
      [this]() { onDashboardAtletaClicked(); }};

  m_uiRegistry[UI_DASHBOARD_EQUIPO] = {
      "👥 Dashboard por Equipo",
      "background-color: #2196F3; color: white; border-radius: 5px;",
      [this]() { onDashboardEquipoClicked(); }};

  m_uiRegistry[UI_REGISTRO_ATLETA] = {
      "✏️ Registrar Atletas",
      "background-color: #FF9800; color: white; border-radius: 5px;",
      [this]() { onRegistroAtletaClicked(); }};

  m_uiRegistry[UI_REGISTRO_ENTRENADOR] = {
      "👤 Registrar Entrenadores",
      "background-color: #9C27B0; color: white; border-radius: 5px;",
      [this]() { onRegistroEntrenadorClicked(); }};

  qDebug() << "UIRegistry inicializado con" << m_uiRegistry.size() << "UIs";
}

void MainWindow::buildUI(const QVector<int> &allowedUIIds) {
  if (allowedUIIds.isEmpty()) {
    QLabel *noAccess = new QLabel("⚠️ No tienes acceso a ninguna funcion", this);
    noAccess->setStyleSheet("color: red; font-size: 14px;");
    noAccess->setAlignment(Qt::AlignCenter);
    m_layout->addWidget(noAccess);
    return;
  }

  qDebug() << "Construyendo UI con" << allowedUIIds.size()
           << "elementos permitidos";

  for (int uiId : allowedUIIds) {
    const UIMetadata &meta = m_uiRegistry[uiId];

    QPushButton *btn = new QPushButton(meta.buttonText, this);
    btn->setMinimumHeight(50);
    btn->setStyleSheet(meta.styleSheet);

    QFont btnFont("Arial", 12);
    btn->setFont(btnFont);

    connect(btn, &QPushButton::clicked, this, meta.action);

    m_layout->addWidget(btn);

    qDebug() << "Boton creado para UI ID:" << uiId << "-" << meta.buttonText;
  }
}

void MainWindow::onDashboardAtletaClicked() {
  QMessageBox::information(
      this, "Dashboard Individual",
      "Aqui se mostraria el dashboard del atleta individual\n");
}

void MainWindow::onDashboardEquipoClicked() {
  QMessageBox::information(
      this, "Dashboard por Equipo",
      "Aqui se mostraria el dashboard completo del equipo\n\n"
      "- Estadisticas del equipo\n"
      "- Lista de atletas\n"
      "- Resultados de partidos");
}

void MainWindow::onRegistroAtletaClicked() {
  QMessageBox::information(this, "Registro de Atletas",
                           "Aqui se mostraria el formulario para:\n\n"
                           "- Crear nuevos atletas\n"
                           "- Editar datos de atletas\n"
                           "- Asignar a equipos");
}

void MainWindow::onRegistroEntrenadorClicked() {
  QMessageBox::information(this, "Registro de Entrenadores",
                           "Aqui se mostraria el formulario para:\n\n"
                           "- Crear nuevos entrenadores\n"
                           "- Editar datos de entrenadores\n"
                           "- Asignar a equipos");
}
