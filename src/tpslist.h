#pragma once

#include <string>
#include <vector>

#include <QMainWindow>
#include <QStatusBar>

class TPSList {
public:
  void load(QMainWindow* window, QStatusBar* statusbar);

private:
  std::vector<std::string> list;
};
