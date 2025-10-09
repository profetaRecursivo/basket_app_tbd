#include "RegisterAthleteWindow.h"
#include "../../models/Athlete/AthleteManager.h"
#include <QDate>
#include <QDebug>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QRegularExpression>
#include <QVBoxLayout>

RegisterAthleteWindow::RegisterAthleteWindow(PGconn *conn, int backendPid,
                                             QWidget *parent)
    : QDialog(parent), m_conn(conn), m_backendPid(backendPid) {
  setWindowTitle("Registro de Atleta");
  setMinimumWidth(500);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);

  QLabel *title = new QLabel("🏀 Registro de Nuevo Atleta", this);
  QFont titleFont("Arial", 16, QFont::Bold);
  title->setFont(titleFont);
  title->setAlignment(Qt::AlignCenter);
  mainLayout->addWidget(title);

  mainLayout->addSpacing(20);

  QFormLayout *formLayout = new QFormLayout();
  formLayout->setSpacing(15);

  m_nombresInput = new QLineEdit(this);
  m_nombresInput->setPlaceholderText("Ej: Juan Carlos");
  m_nombresInput->setMaxLength(30);
  formLayout->addRow("Nombres:", m_nombresInput);

  m_apellidosInput = new QLineEdit(this);
  m_apellidosInput->setPlaceholderText("Ej: Pérez García");
  m_apellidosInput->setMaxLength(30);
  formLayout->addRow("Apellidos:", m_apellidosInput);

  m_fechaNacimientoInput = new QDateEdit(this);
  m_fechaNacimientoInput->setCalendarPopup(true);
  m_fechaNacimientoInput->setDisplayFormat("dd/MM/yyyy");
  QDate maxDate = QDate::currentDate().addYears(-10);
  QDate minDate = QDate::currentDate().addYears(-80);
  m_fechaNacimientoInput->setDateRange(minDate, maxDate);
  m_fechaNacimientoInput->setDate(maxDate);
  formLayout->addRow("Fecha de Nacimiento:", m_fechaNacimientoInput);

  m_sexoComboBox = new QComboBox(this);
  m_sexoComboBox->addItem("Masculino", 'M');
  m_sexoComboBox->addItem("Femenino", 'F');
  formLayout->addRow("Sexo:", m_sexoComboBox);

  m_paisComboBox = new QComboBox(this);
  formLayout->addRow("País:", m_paisComboBox);

  m_alturaInput = new QDoubleSpinBox(this);
  m_alturaInput->setRange(0.50, 2.50);
  m_alturaInput->setSingleStep(0.01);
  m_alturaInput->setDecimals(2);
  m_alturaInput->setSuffix(" m");
  m_alturaInput->setValue(1.75);
  formLayout->addRow("Altura:", m_alturaInput);

  m_pesoInput = new QDoubleSpinBox(this);
  m_pesoInput->setRange(2.0, 250.0);
  m_pesoInput->setSingleStep(0.5);
  m_pesoInput->setDecimals(2);
  m_pesoInput->setSuffix(" kg");
  m_pesoInput->setValue(70.0);
  formLayout->addRow("Peso:", m_pesoInput);

  mainLayout->addLayout(formLayout);
  mainLayout->addSpacing(10);

  QLabel *infoLabel = new QLabel(
      "ℹ️ El usuario y contraseña serán generados automáticamente", this);
  infoLabel->setStyleSheet("color: #2196F3; font-style: italic;");
  infoLabel->setAlignment(Qt::AlignCenter);
  mainLayout->addWidget(infoLabel);

  mainLayout->addSpacing(20);

  QHBoxLayout *buttonLayout = new QHBoxLayout();

  m_btnCancelar = new QPushButton("Cancelar", this);
  m_btnCancelar->setStyleSheet(
      "background-color: #f44336; color: white; padding: 10px; "
      "border-radius: 5px; font-size: 14px;");
  buttonLayout->addWidget(m_btnCancelar);

  buttonLayout->addStretch();

  m_btnGuardar = new QPushButton("Guardar Atleta", this);
  m_btnGuardar->setStyleSheet(
      "background-color: #4CAF50; color: white; padding: 10px; "
      "border-radius: 5px; font-size: 14px;");
  buttonLayout->addWidget(m_btnGuardar);

  mainLayout->addLayout(buttonLayout);

  connect(m_btnGuardar, &QPushButton::clicked, this,
          &RegisterAthleteWindow::onGuardarClicked);
  connect(m_btnCancelar, &QPushButton::clicked, this,
          &RegisterAthleteWindow::onCancelarClicked);

  loadCountries();
}

void RegisterAthleteWindow::loadCountries() {
  m_paisComboBox->clear();
  m_paisComboBox->addItem("Seleccione un país...", -1);

  QVector<QPair<int, QString>> countries = AthleteManager::getCountries(m_conn);

  for (const auto &country : countries) {
    m_paisComboBox->addItem(country.second, country.first);
  }
}

bool RegisterAthleteWindow::validateInputs() {
  QString nombres = m_nombresInput->text().trimmed();
  if (nombres.isEmpty()) {
    QMessageBox::warning(this, "Error de validación",
                         "El campo 'Nombres' es obligatorio.");
    m_nombresInput->setFocus();
    return false;
  }

  QRegularExpression nameRegex("^[A-Za-zÁÉÍÓÚáéíóúÑñ ]+$");
  if (!nameRegex.match(nombres).hasMatch()) {
    QMessageBox::warning(
        this, "Error de validación",
        "El campo 'Nombres' solo puede contener letras y espacios.");
    m_nombresInput->setFocus();
    return false;
  }

  QString apellidos = m_apellidosInput->text().trimmed();
  if (apellidos.isEmpty()) {
    QMessageBox::warning(this, "Error de validación",
                         "El campo 'Apellidos' es obligatorio.");
    m_apellidosInput->setFocus();
    return false;
  }

  if (!nameRegex.match(apellidos).hasMatch()) {
    QMessageBox::warning(
        this, "Error de validación",
        "El campo 'Apellidos' solo puede contener letras y espacios.");
    m_apellidosInput->setFocus();
    return false;
  }

  int paisId = m_paisComboBox->currentData().toInt();
  if (paisId == -1) {
    QMessageBox::warning(this, "Error de validación",
                         "Debe seleccionar un país.");
    m_paisComboBox->setFocus();
    return false;
  }

  return true;
}

void RegisterAthleteWindow::onGuardarClicked() {
  if (!validateInputs()) {
    return;
  }

  QString nombres = m_nombresInput->text().trimmed();
  QString apellidos = m_apellidosInput->text().trimmed();
  QDate fechaNacimiento = m_fechaNacimientoInput->date();
  char sexo = m_sexoComboBox->currentData().toChar().toLatin1();
  int idPais = m_paisComboBox->currentData().toInt();
  double altura = m_alturaInput->value();
  double peso = m_pesoInput->value();

  QPair<QString, QString> credentials =
      AthleteManager::insertAthlete(nombres, apellidos, fechaNacimiento, sexo,
                                    idPais, altura, peso, m_conn, m_backendPid);

  if (!credentials.first.isEmpty()) {
    QString message =
        QString("🎉 ¡Atleta registrado exitosamente!\n\n"
                "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
                "📋 Datos del Atleta:\n"
                "   Nombre: %1 %2\n\n"
                "🔐 Credenciales de Acceso:\n"
                "   Usuario: %3\n"
                "   Contraseña: %4\n"
                "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n"
                "⚠️ IMPORTANTE: Guarde estas credenciales.\n"
                "El atleta debe usar estos datos para iniciar sesión.")
            .arg(nombres)
            .arg(apellidos)
            .arg(credentials.first)
            .arg(credentials.second);

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Registro Exitoso");
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();

    accept();
  } else {
    QMessageBox::critical(this, "Error",
                          "No se pudo registrar el atleta.\n\n"
                          "Posibles causas:\n"
                          "- Error de conexión a la base de datos\n"
                          "- Datos inválidos\n\n"
                          "Verifique los datos e intente nuevamente.");
  }
}

void RegisterAthleteWindow::onCancelarClicked() {
  reject();
}
