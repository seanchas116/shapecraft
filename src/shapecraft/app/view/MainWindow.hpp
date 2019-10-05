#pragma once
#include "shapecraft/Common.hpp"
#include <QMainWindow>

class QSplitter;

namespace shapecraft {

class WindowState;

class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    explicit MainWindow(const SP<WindowState> &state, QWidget *parent = nullptr);

  protected:
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

  private:
    void actualSize();
    void zoomIn();
    void zoomOut();

    void open();

    void save();
    void saveAs();

    void updateWindowFilePath(const QString &filePath);

    QSplitter *_splitter = nullptr;
    SP<WindowState> _state;
    bool _discardConfirmed = false;
};

} // namespace shapecraft
