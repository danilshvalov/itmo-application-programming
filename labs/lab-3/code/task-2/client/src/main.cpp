#include <QApplication>

#include "data_table.hpp"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  DataTable table;
  table.show();
  return app.exec();
}
