#pragma once

#include <QAbstractTableModel>
#include <QMainWindow>

#include "phonerecords.h"
#include "tpslist.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow* ui;

  PhoneRecords phones;
  TPSList tps;

  void reset_phonetableview();

private slots:
  void on_LoadREPhones_clicked();
  void on_LoadTPS_clicked();
  void on_MakeImportData_clicked();
};
