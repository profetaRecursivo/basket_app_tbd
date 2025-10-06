#pragma once
#include <QVector>
#include <libpq-fe.h>

class UIGetter {
public:
  UIGetter();
  static QVector<int> getUIIds(int userId);

private:
  static QVector<int> parseUIIds(PGresult *res);
};
