#pragma once
#include <QMainWindow>

namespace shapecraft {

class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    explicit MainWindow(QWidget *parent = nullptr);
};

} // namespace shapecraft
