#pragma once

#include <QAbstractTableModel>
#include <QMainWindow>
#include <QStatusBar>

class PhoneRecords : public QAbstractTableModel {
public:
  void load(QMainWindow* window, QStatusBar* sb);
  virtual int rowCount(const QModelIndex& parent) const override;
  virtual int columnCount(const QModelIndex& parent) const override;
  virtual QVariant data(const QModelIndex& index, int role) const override;
  virtual QVariant headerData(int section,
                              Qt::Orientation orientation,
                              int role) const override;

private:
  enum class TPSMatch { NotChecked, NoMatch, Matched };
  QString TPSToStr(TPSMatch m) const;

  struct Record {
    std::string id;
    std::string name;
    std::string raw_number;
    std::string clean_number;
    TPSMatch tps_match{TPSMatch::NotChecked};
  };

  std::vector<Record> records;
};
