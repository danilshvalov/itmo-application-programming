#include "results_table.hpp"

#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

ResultsTable::ResultsTable(const std::vector<Point> &points,
                           const Coefficients &coefficients, QWidget *parent)
    : QWidget(parent) {
  QTableWidget *points_table = new QTableWidget(points.size(), 2, this);
  points_table->setFixedSize(250, 300);
  points_table->setHorizontalHeaderLabels({"x", "y"});

  for (size_t i = 0; i < points.size(); ++i) {
    const Point &p = points[i];
    QTableWidgetItem *x_item = new QTableWidgetItem(QString::number(p.x));
    QTableWidgetItem *y_item = new QTableWidgetItem(QString::number(p.y));
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    x_item->setFlags(flags);
    y_item->setFlags(flags);
    points_table->setItem(i, 0, x_item);
    points_table->setItem(i, 1, y_item);
  }

  QLabel *label_a = new QLabel(QString("a: %1").arg(coefficients.a));
  QLabel *label_b = new QLabel(QString("b: %1").arg(coefficients.b));
  QLabel *label_r = new QLabel(QString("R: %1").arg(coefficients.r));

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(points_table);
  layout->addWidget(label_a);
  layout->addWidget(label_b);
  layout->addWidget(label_r);

  setLayout(layout);
  setWindowTitle("Результаты");
  adjustSize();
  setFixedSize(width(), height());
}
