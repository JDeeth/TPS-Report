#include "phonerecords.h"

#include <locale>
#include <regex>
#include <sstream>

#include <QFileDialog>
#include <QMessageBox>

#include "csv-parser/parser.hpp"

std::pair<std::string, bool> CleanNumber(std::string number) {
  // remove (0)
  number = std::regex_replace(number, std::regex("\\(0\\)"), "");

  // remove spaces and hyphens
  number = std::regex_replace(number, std::regex("[ -]"), "");

  // replace initial +44 with 0
  number = std::regex_replace(number, std::regex("^\\+44"), "0");

  // remove non-numeric characters
  number = std::regex_replace(number, std::regex("\\D"), "");

  // if first two digits are 00, discard
  if (std::regex_search(number, std::regex("^00")))
    return std::make_pair("Begins 00", false);

  // if first digit isn't 0, discard
  if (!std::regex_search(number, std::regex("^0")))
    return std::make_pair("Doesn't begin 0", false);

  // if not exactly 11 digits, discard
  if (number.length() != 11)
    return std::make_pair("Wrong length", false);

  return std::make_pair(number, true);
}

void PhoneRecords::load(QMainWindow* window, QStatusBar* sb) {
  QString filename = QFileDialog::getOpenFileName(
      window, "Load RE phones", "", "Text/CSV files (*.txt *.csv);;Any (*)");

  if (!filename.isEmpty()) {
    std::ifstream f(filename.toStdString());
    if (!f.is_open()) {
      QMessageBox::critical(window, "Error", "Could not open file");
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
      auto clean_number = CleanNumber(raw_number);

      records.emplace_back(Record(
          id, name, raw_number, clean_number.first, clean_number.second));
    }
    // emit dataChanged ought to happen here

    std::stringstream s;
    s.imbue(std::locale(""));
    s << records.size() << " RE phone numbers imported";

    sb->showMessage(QString::fromStdString(s.str()));

    f.close();
  }
}

void PhoneRecords::check(const TPSList& tps, QStatusBar* sb) {
  if (tps.size() == 0)
    return;

  int checked{0};
  int matched{0};

  std::stringstream s;
  s.imbue(std::locale(""));
  auto update = [&] {
    s.str("");
    s << checked << " numbers checked; " << matched << " numbers matched";
    sb->showMessage(QString::fromStdString(s.str()));
  };

  for (auto& record : records) {
    if (record.clean) {
      if (tps.matched(record.clean_number)) {
        record.tps_match = TPSMatch::Matched;
        ++matched;
        update();
      } else {
        record.tps_match = TPSMatch::NoMatch;
        if (checked % 100 == 0)
          update();
      }
      ++checked;
    }
  }
  update();
}

void PhoneRecords::create_import(QMainWindow* window,
                                 QStatusBar* sb,
                                 const Attribute& attr) {
  QString filename = QFileDialog::getSaveFileName(
      window,
      "Create import data",
      "",
      "Text/Import files (*.txt *.csv *.imp);;All files (*)");

  if (!filename.isEmpty()) {
    std::ofstream f(filename.toStdString());
    if (!f.is_open()) {
      QMessageBox::critical(window, "Error", "Could not create file");
    } else {
      f << R"("ConsID","CAttrCat","CAttrDesc","CAttrDate","CAttrCom")" << '\n';

      auto sep{R"(",")"};
      QString attr_data{sep + attr.category + sep + attr.desc + sep +
                        attr.date + sep + attr.comment + "\"\n"};

      int exported{0};
      for (auto r : records) {
        if (r.tps_match == TPSMatch::Matched) {
          f << "\"" << r.id << attr_data.toStdString();
          ++exported;
          sb->showMessage(QString::fromStdString(std::to_string(exported) +
                                                 " rows created"));
        }
      }

      f.close();
    }
  }
}

int PhoneRecords::rowCount(const QModelIndex&) const {
  return static_cast<int>(records.size());
}

int PhoneRecords::columnCount(const QModelIndex&) const {
  return 6;
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
      return row.clean ? "Yes" : "No";
    case 4:
      return QString::fromStdString(row.clean_number);
    case 5:
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
      return "Cn. ID";
    case 1:
      return "Constituent Name";
    case 2:
      return "RE phone number";
    case 3:
      return "Clean?";
    case 4:
      return "Cleaned number";
    case 5:
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

PhoneRecords::Record::Record(std::string id,
                             std::string name,
                             std::string raw_number,
                             std::string clean_number,
                             bool clean)
    : id{id},
      name{name},
      raw_number{raw_number},
      clean_number{clean_number},
      clean{clean} {}
