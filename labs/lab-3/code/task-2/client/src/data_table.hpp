#pragma once

#include "models.hpp"

#include <QWidget>
#include <QTableWidget>
#include <QLocale>

class DataTable : public QWidget {
public:
  DataTable(QWidget *parent = nullptr);

private:
  void OnCreateRow();
  void OnDeleteRow();
  void OnCalculate();
  std::vector<Point> CollectPoints() const;

  QTableWidget* table_;
  QLocale locale_;
};
