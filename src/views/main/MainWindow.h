// #pragma once

#include "../UIComponents.h"
#include <QLabel>
#include <QMainWindow>
#include <QMap>
#include <QPushButton>
#include <QVBoxLayout>
#include <functional>
#include <libpq-fe.h>
struct UIMetadata {
  QString buttonText;
  QString styleSheet;
  std::function<void()> action;
};

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(int userId, int backendPid, PGconn *conn = nullptr,
                      QWidget *parent = nullptr);
  ~MainWindow();

private:
  int m_userId;
  int m_backendPid;
  PGconn *m_conn;
  QVBoxLayout *m_layout;
  QMap<int, UIMetadata> m_uiRegistry;
  void initializeUIRegistry();
  void buildUI(const QVector<int> &allowedUIIds);

private slots:
  void onDashboardAtletaClicked();
  void onDashboardEquipoClicked();
  void onRegistroAtletaClicked();
  void onRegistroEntrenadorClicked();
};
