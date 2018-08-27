#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDate>
#include <QtWidgets>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  ui->AttrName->setText(QString("TPSMatch"));
  auto date = QDate::currentDate().toString("yyyy-MM-dd");
  ui->AttrValue->setText(date);
  ui->AttrComment->setText("Matched in bulk by TPSReport on " + date);

  ui->PhoneTable->setModel(&phones);
  ui->PhoneTable->resizeColumnsToContents();
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::on_LoadREPhones_clicked() {
  phones.load(this, ui->statusBar);
  // this is not the right way to do this
  ui->PhoneTable->setModel(nullptr);
  ui->PhoneTable->setModel(&phones);
  ui->PhoneTable->resizeColumnsToContents();
}

void MainWindow::on_LoadTPS_clicked() {
  tps.load(this, ui->statusBar);
}

void MainWindow::on_MakeImportData_clicked() {
  ui->statusBar->showMessage("Create attribute import data");
}
