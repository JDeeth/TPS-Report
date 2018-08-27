#include "tpslist.h"

#include <fstream>
#include <locale>
#include <regex>
#include <sstream>

#include <QFileDialog>
#include <QMessageBox>

void TPSList::load(QMainWindow* window, QStatusBar* sb) {
  QString filename = QFileDialog::getOpenFileName(
      window, "Load TPS list", "", "Text/CSV files (*.txt *.csv);;Any (*)");

  if (!filename.isEmpty()) {
    std::ifstream f(filename.toStdString());
    if (!f.is_open()) {
      QMessageBox::critical(window, "Error", "Could not open file");
      return;
    }

    auto sb_update = [sb](const std::string& msg) {
      sb->showMessage(QString::fromStdString(msg));
    };

    list.clear();

    std::stringstream s;
    s.imbue(std::locale(""));

    int lines = 0;
    std::string line;

    while (std::getline(f, line)) {
      ++lines;
      if (lines % 100000 == 0) {
        s << "Reading TPS row " << lines;
        sb_update(s.str());
        s.str("");
      }
      list.emplace_back(std::regex_replace(line, std::regex(" "), ""));
    }
    s << list.size() << " TPS phone numbers imported";
    sb_update(s.str());

    f.close();
  }
}
