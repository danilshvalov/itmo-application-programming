#pragma once

#include <QLineEdit>
#include <QLocale>
#include <QPushButton>
#include <QTableWidget>
#include <QWidget>

#include "server.hpp"

class MainWindow : public QWidget {
 public:
  MainWindow(QWidget* parent = nullptr);

 private:
  void OnStart();
  void OnStop();

  Server server_;
  QPushButton* startButton_;
  QPushButton* stopButton_;
  QLineEdit* addressEdit_;
  QLineEdit* portEdit_;
  std::string certFilename_;
  std::string keyFilename_;
  std::string dhFilename_;
  QLineEdit* passwordEdit_;
};
