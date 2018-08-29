#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDate>
#include <QtWidgets>

#include "attribute.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  setWindowTitle("TPS Report");

  ui->AttrCategory->setText("TPS Subscriber");
  ui->AttrDescription->setText("True");
  ui->AttrDate->setDate(QDate::currentDate());
  ui->AttrComment->setText("Matched by TPSReport on %RAW");

  reset_phonetableview();
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::reset_phonetableview() {
  // this is not the right way to do this
  ui->PhoneTable->setModel(nullptr);
  ui->PhoneTable->setModel(&phones);
  ui->PhoneTable->resizeColumnsToContents();
}

void MainWindow::on_LoadREPhones_clicked() {
  phones.load(this, ui->statusBar);
  phones.check(tps, ui->statusBar);
  reset_phonetableview();
}

void MainWindow::on_LoadTPS_clicked() {
  tps.load(this, ui->statusBar);
  phones.check(tps, ui->statusBar);
  reset_phonetableview();
}

void MainWindow::on_MakeImportData_clicked() {
  Attribute attr;
  attr.category = ui->AttrCategory->text();
  attr.desc = ui->AttrDescription->text();
  attr.date = ui->AttrDate->date().toString("yyyy-MM-dd");
  attr.comment = ui->AttrComment->text();
  phones.create_import(this, ui->statusBar, attr);
}
