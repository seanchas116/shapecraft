#include "MainWindow.hpp"

namespace shapecraft {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setAttribute(Qt::WA_DeleteOnClose);
}

} // namespace shapecraft
