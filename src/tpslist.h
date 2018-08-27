#pragma once

#include <string>
#include <vector>

#include <QMainWindow>
#include <QStatusBar>

class TPSList {
public:
  void load(QMainWindow* window, QStatusBar* statusbar);
  bool matched(const std::string& str) const;
  size_t size() const;

private:
  std::vector<std::string> list;
};
