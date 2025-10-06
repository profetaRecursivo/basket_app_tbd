#pragma once

#include <QLabel>
#include <QMainWindow>
#include <QMap>
#include <QPushButton>
#include <QVBoxLayout>
#include <functional>

struct UIMetadata {
  QString buttonText;
  QString styleSheet;
  std::function<void()> action;
};

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(int userId, QWidget *parent = nullptr);
  ~MainWindow();

private:
  int m_userId;
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
