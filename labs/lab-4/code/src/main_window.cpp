#include "main_window.hpp"

#include <QFileDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
  startButton_ = new QPushButton();
  startButton_->setFixedSize(100, 50);
  startButton_->setText("Запустить");
  QObject::connect(startButton_, &QPushButton::clicked, this,
                   &MainWindow::OnStart);

  stopButton_ = new QPushButton();
  stopButton_->setFixedSize(100, 50);
  stopButton_->setText("Остановить");
  QObject::connect(stopButton_, &QPushButton::clicked, this,
                   &MainWindow::OnStop);
  stopButton_->setDisabled(true);

  QWidget* buttons = new QWidget();
  QHBoxLayout* buttonsLayout = new QHBoxLayout(buttons);
  buttonsLayout->addStretch();
  buttonsLayout->setSpacing(0);
  buttonsLayout->addWidget(startButton_);
  buttonsLayout->addWidget(stopButton_);
  buttonsLayout->addStretch();
  buttons->setLayout(buttonsLayout);

  addressEdit_ = new QLineEdit();
  portEdit_ = new QLineEdit();
  passwordEdit_ = new QLineEdit();

  QPushButton* certFileButton = new QPushButton(this);
  certFileButton->setText("Выбрать файл");
  QObject::connect(certFileButton, &QPushButton::clicked, [=] {
    certFilename_ = QFileDialog::getOpenFileName().toStdString();
    if (!certFilename_.empty()) {
      QString filename =
          QFileInfo(QString::fromStdString(certFilename_)).fileName();
      certFileButton->setText(filename);
    }
  });

  QPushButton* keyFileButton = new QPushButton(this);
  keyFileButton->setText("Выбрать файл");
  QObject::connect(keyFileButton, &QPushButton::clicked, [=] {
    keyFilename_ = QFileDialog::getOpenFileName().toStdString();
    if (!keyFilename_.empty()) {
      QString filename =
          QFileInfo(QString::fromStdString(keyFilename_)).fileName();
      keyFileButton->setText(filename);
    }
  });

  QPushButton* dhFileButton = new QPushButton(this);
  dhFileButton->setText("Выбрать файл");
  QObject::connect(dhFileButton, &QPushButton::clicked, [=] {
    dhFilename_ = QFileDialog::getOpenFileName().toStdString();
    if (!dhFilename_.empty()) {
      QString filename =
          QFileInfo(QString::fromStdString(dhFilename_)).fileName();
      dhFileButton->setText(filename);
    }
  });

  QWidget* form = new QWidget();
  QFormLayout* formLayout = new QFormLayout(this);
  formLayout->addRow(tr("&Address:"), addressEdit_);
  formLayout->addRow(tr("&Port:"), portEdit_);
  formLayout->addRow(tr("&Certificate file:"), certFileButton);
  formLayout->addRow(tr("&Key file:"), keyFileButton);
  formLayout->addRow(tr("&DH file:"), dhFileButton);
  formLayout->addRow(tr("&Key file password:"), passwordEdit_);
  form->setLayout(formLayout);

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->addWidget(form);
  layout->addWidget(buttons);
  setLayout(layout);
  setWindowTitle("HTTPS Server");
  adjustSize();
  setFixedSize(width(), height());
}

void MainWindow::OnStart() {
  const auto& address =
      boost::asio::ip::make_address(addressEdit_->text().toStdString());
  uint16_t port = portEdit_->text().toInt();
  server_.Start(address, port, certFilename_, keyFilename_, dhFilename_,
                passwordEdit_->text().toStdString());
  startButton_->setDisabled(true);
  stopButton_->setDisabled(false);
}

void MainWindow::OnStop() {
  server_.Stop();
  startButton_->setDisabled(false);
  stopButton_->setDisabled(true);
}
