#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDate>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->AttrName->setText(QString("TPSMatch"));
  auto date = QDate::currentDate().toString("yyyy-MM-dd");
  ui->AttrValue->setText(date);
  ui->AttrComment->setText("Matched in bulk by TPSReport on " + date);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::LoadRE() {}
