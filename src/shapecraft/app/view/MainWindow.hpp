#pragma once
#include "shapecraft/Common.hpp"
#include <QMainWindow>

namespace shapecraft {

class WindowState;

class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    explicit MainWindow(const SP<WindowState> &state, QWidget *parent = nullptr);

  protected:
    void closeEvent(QCloseEvent *event) override;

  private:
    void actualSize();
    void zoomIn();
    void zoomOut();

    void open();

    void save();
    void saveAs();

    void updateWindowFilePath(const QString &filePath);

    SP<WindowState> _state;
    bool _discardConfirmed = false;
};

} // namespace shapecraft
