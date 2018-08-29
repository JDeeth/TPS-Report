#include "tpslist.h"

#include <fstream>
#include <locale>
#include <regex>
#include <sstream>

#include <QFileDialog>
#include <QMessageBox>

void TPSList::load(QMainWindow* window, QStatusBar* sb) {
  QString filename = QFileDialog::getOpenFileName(
      window,
      "Load TPS list",
      "",
      "Text/CSV files (*.txt *.csv *.dat);;Any (*)");

  if (!filename.isEmpty()) {
    std::ifstream f(filename.toStdString());
    if (!f.is_open()) {
      QMessageBox::critical(window, "Error", "Could not open file");
      return;
    }

    list.clear();

    std::stringstream s;
    s.imbue(std::locale(""));

    size_t lines = 0;
    auto max_lines = list.max_size();
    std::string line;

    while (std::getline(f, line)) {
      if (lines >= max_lines) {
        s << "Cannot load more than " + std::to_string(max_lines) +
                 " records on this system.";
        QMessageBox::critical(window, "Error", QString::fromStdString(s.str()));
        break;
      }
      list.emplace_back(std::regex_replace(line, std::regex(" "), ""));

      ++lines;
      if (lines % 100000 == 0) {
        s << lines << " TPS phone numbers imported";
        sb->showMessage(QString::fromStdString(s.str()));
        s.str("");
      }
    }

    f.close();
  }
}

bool TPSList::matched(const std::string& str) const {
  for (const auto& tps : list)
    if (str == tps)
      return true;
  return false;
}

size_t TPSList::size() const {
  return list.size();
}
