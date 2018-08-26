#include "phonerecords.h"

#include <QMessageBox>

#include "csv-parser/parser.hpp"

void PhoneRecords::load(QMainWindow* ptr, const QString& filename) {
  if (!filename.isEmpty()) {
    std::ifstream f(filename.toStdString());
    if (!f.is_open()) {
      QMessageBox::critical(ptr, "Error", "Could not open file");
      return;
    }

    records.clear();

    aria::csv::CsvParser csv(f);

    auto row = csv.begin();

    if ((*row)[0] == "Constituent ID")
      row++;

    for (; row != csv.end(); row++) {
      auto id = (*row)[0];
      auto name = (*row)[1];
      auto raw_number = (*row)[2];
      auto clean_number = "";

      records.emplace_back(Record{id, name, raw_number, clean_number});
    }
    // emit dataChanged ought to happen here
  }
}

int PhoneRecords::rowCount(const QModelIndex&) const {
  return static_cast<int>(records.size());
}

int PhoneRecords::columnCount(const QModelIndex&) const {
  return 5;
}

QVariant PhoneRecords::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();

  if (index.row() >= records.size())
    return QVariant();

  auto row = records[index.row()];
  switch (index.column()) {
    case 0:
      return QString::fromStdString(row.id);
    case 1:
      return QString::fromStdString(row.name);
    case 2:
      return QString::fromStdString(row.raw_number);
    case 3:
      return QString::fromStdString(row.clean_number);
    case 4:
      return TPSToStr(row.tps_match);

    default:
      return QVariant();
  }
}

QVariant PhoneRecords::headerData(int section,
                                  Qt::Orientation orientation,
                                  int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Vertical)
    return section + 1;

  switch (section) {
    case 0:
      return "Constituent ID";
    case 1:
      return "Constituent Name";
    case 2:
      return "RE phone number";
    case 3:
      return "Clean phone number";
    case 4:
      return "TPS match?";
    default:
      return "?";
  }
}

QString PhoneRecords::TPSToStr(PhoneRecords::TPSMatch m) const {
  switch (m) {
    case TPSMatch::NotChecked:
      return "Not checked";
    case TPSMatch::NoMatch:
      return "No match";
    case TPSMatch::Matched:
      return "Matched";
  }
}
