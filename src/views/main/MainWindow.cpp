#include "MainWindow.h"
#include "../../models/Auth/UIGetter.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(int userId, QWidget *parent)
    : QMainWindow(parent), m_userId(userId) {
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

  // Inicializar el registro de UIs
  initializeUIRegistry();

  // Obtener los IDs de UI permitidos para este usuario
  QVector<int> allowedUIIds = UIGetter::getUIIds(m_userId);

  // Construir la interfaz con las UIs permitidas
  buildUI(allowedUIIds);

  m_layout->addStretch();
}

MainWindow::~MainWindow() {}

void MainWindow::initializeUIRegistry() {
  // Registramos cada UI con su id, texto, estilo y acción
  // El id_ui debe coincidir con los IDs en la tabla UI de la BD
  // IDs reales en BD: 14=DASHBOARD_ATLETA, 15=DASHBOARD_EQUIPO,
  //                   16=REGISTRO_ATLETA, 17=REGISTRO_ENTRENADOR

  m_uiRegistry[14] = {
      "📊 Dashboard Individual",
      "background-color: #4CAF50; color: white; border-radius: 5px;",
      [this]() { onDashboardAtletaClicked(); }};

  m_uiRegistry[15] = {
      "👥 Dashboard por Equipo",
      "background-color: #2196F3; color: white; border-radius: 5px;",
      [this]() { onDashboardEquipoClicked(); }};

  m_uiRegistry[16] = {
      "✏️ Registrar Atletas",
      "background-color: #FF9800; color: white; border-radius: 5px;",
      [this]() { onRegistroAtletaClicked(); }};

  m_uiRegistry[17] = {
      "👤 Registrar Entrenadores",
      "background-color: #9C27B0; color: white; border-radius: 5px;",
      [this]() { onRegistroEntrenadorClicked(); }};

  qDebug() << "UIRegistry inicializado con" << m_uiRegistry.size() << "UIs";
}

void MainWindow::buildUI(const QVector<int> &allowedUIIds) {
  if (allowedUIIds.isEmpty()) {
    QLabel *noAccess = new QLabel("⚠️ No tienes acceso a ninguna UI", this);
    noAccess->setStyleSheet("color: red; font-size: 14px;");
    noAccess->setAlignment(Qt::AlignCenter);
    m_layout->addWidget(noAccess);
    return;
  }

  qDebug() << "Construyendo UI con" << allowedUIIds.size()
           << "elementos permitidos";

  // Iteramos sobre los IDs permitidos y creamos los botones
  for (int uiId : allowedUIIds) {
    if (m_uiRegistry.contains(uiId)) {
      const UIMetadata &meta = m_uiRegistry[uiId];

      QPushButton *btn = new QPushButton(meta.buttonText, this);
      btn->setMinimumHeight(50);
      btn->setStyleSheet(meta.styleSheet);

      QFont btnFont("Arial", 12);
      btn->setFont(btnFont);

      // Conectamos el botón a su acción usando la lambda del metadata
      connect(btn, &QPushButton::clicked, this, meta.action);

      m_layout->addWidget(btn);

      qDebug() << "  ✓ Botón creado para UI ID:" << uiId << "-"
               << meta.buttonText;
    } else {
      qDebug() << "  ✗ Advertencia: UI ID" << uiId
               << "no está registrado en m_uiRegistry";
    }
  }
}

void MainWindow::onDashboardAtletaClicked() {
  QMessageBox::information(
      this, "Dashboard Individual",
      "Aquí se mostraría el dashboard del atleta individual\n\n"
      "- Estadísticas personales\n"
      "- Historial de partidos\n"
      "- Performance individual");
}

void MainWindow::onDashboardEquipoClicked() {
  QMessageBox::information(
      this, "Dashboard por Equipo",
      "Aquí se mostraría el dashboard completo del equipo\n\n"
      "- Estadísticas del equipo\n"
      "- Lista de atletas\n"
      "- Resultados de partidos");
}

void MainWindow::onRegistroAtletaClicked() {
  QMessageBox::information(this, "Registro de Atletas",
                           "Aquí se mostraría el formulario para:\n\n"
                           "- Crear nuevos atletas\n"
                           "- Editar datos de atletas\n"
                           "- Asignar a equipos");
}

void MainWindow::onRegistroEntrenadorClicked() {
  QMessageBox::information(this, "Registro de Entrenadores",
                           "Aquí se mostraría el formulario para:\n\n"
                           "- Crear nuevos entrenadores\n"
                           "- Editar datos de entrenadores\n"
                           "- Asignar a equipos");
}
