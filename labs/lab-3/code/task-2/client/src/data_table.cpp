#include "data_table.hpp"

#include "requests.hpp"
#include "results_table.hpp"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

DataTable::DataTable(QWidget *parent) : QWidget(parent) {
  table_ = new QTableWidget(0, 2);
  table_->setFixedSize(250, 300);
  table_->setHorizontalHeaderLabels({"x", "y"});

  locale_ = QLocale(QLocale::English);

  QWidget *rowsButtons = new QWidget();

  QPushButton *createRowButton = new QPushButton();
  createRowButton->setFixedSize(100, 50);
  createRowButton->setText("+");
  QObject::connect(createRowButton, &QPushButton::clicked, this,
                   &DataTable::OnCreateRow);

  QPushButton *deleteRowButton = new QPushButton();
  deleteRowButton->setFixedSize(100, 50);
  deleteRowButton->setText("-");
  QObject::connect(deleteRowButton, &QPushButton::clicked, this,
                   &DataTable::OnDeleteRow);

  QHBoxLayout *rowsButtonsLayout = new QHBoxLayout();
  rowsButtonsLayout->addWidget(createRowButton);
  rowsButtonsLayout->addWidget(deleteRowButton);
  rowsButtons->setLayout(rowsButtonsLayout);

  QPushButton *calculateButton = new QPushButton();
  calculateButton->setFixedSize(214, 50);
  calculateButton->setText("Рассчитать");
  QObject::connect(calculateButton, &QPushButton::clicked, this,
                   &DataTable::OnCalculate);

  QWidget *buttons = new QWidget();
  QVBoxLayout *buttonsLayout = new QVBoxLayout(buttons);
  buttonsLayout->addStretch();
  buttonsLayout->setSpacing(0);
  buttonsLayout->addWidget(rowsButtons, 0, Qt::AlignCenter);
  buttonsLayout->addWidget(calculateButton, 0, Qt::AlignCenter);
  buttonsLayout->addStretch();

  buttons->setLayout(buttonsLayout);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(table_);
  layout->addWidget(buttons);
  setLayout(layout);
  setWindowTitle("Коэффициенты прямой");
  adjustSize();
  setFixedSize(width(), height());
}

void DataTable::OnCreateRow() {
  table_->setRowCount(table_->rowCount() + 1);
  for (size_t col = 0; col < table_->columnCount(); ++col) {
    QLineEdit *edit = new QLineEdit();
    QDoubleValidator *validator = new QDoubleValidator(edit);
    validator->setLocale(locale_);
    edit->setValidator(validator);
    table_->setCellWidget(table_->rowCount() - 1, col, edit);
  }
}

void DataTable::OnDeleteRow() { table_->setRowCount(table_->rowCount() - 1); }

void DataTable::OnCalculate() {
  if (table_->rowCount() < 2) {
    QMessageBox::critical(this, "Ошибка", "Необходимо ввести не менее 2 точек");
    return;
  }

  ResponseData response_data;
  try {
    response_data = SendRequest(RequestData{CollectPoints()});
  } catch (const std::exception &e) {
    QMessageBox::critical(this, "Ошибка", "Введены некорректные данные");
    return;
  }

  ResultsTable *table_ = new ResultsTable{
      response_data.calculated_points,
      response_data.coefficients,
  };
  table_->show();
}

std::vector<Point> DataTable::CollectPoints() const {
  std::vector<Point> points;
  points.reserve(table_->rowCount());
  for (size_t row = 0; row < table_->rowCount(); ++row) {
    auto x_cell = qobject_cast<QLineEdit *>(table_->cellWidget(row, 0));
    auto y_cell = qobject_cast<QLineEdit *>(table_->cellWidget(row, 1));
    points.push_back(Point{
        locale_.toDouble(x_cell->text()),
        locale_.toDouble(y_cell->text()),
    });
  }
  return points;
}
