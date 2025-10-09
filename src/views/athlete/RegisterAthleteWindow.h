#pragma once
#include <QComboBox>
#include <QDateEdit>
#include <QDialog>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <libpq-fe.h>

class RegisterAthleteWindow : public QDialog {
  Q_OBJECT

public:
  explicit RegisterAthleteWindow(PGconn *conn, int backendPid,
                                 QWidget *parent = nullptr);

private:
  QLineEdit *m_nombresInput;
  QLineEdit *m_apellidosInput;
  QDateEdit *m_fechaNacimientoInput;
  QComboBox *m_sexoComboBox;
  QComboBox *m_paisComboBox;
  QDoubleSpinBox *m_alturaInput;
  QDoubleSpinBox *m_pesoInput;

  QPushButton *m_btnGuardar;
  QPushButton *m_btnCancelar;

  PGconn *m_conn;
  int m_backendPid;

  void loadCountries();
  bool validateInputs();

private slots:
  void onGuardarClicked();
  void onCancelarClicked();
};
