#include "MainWindow.hpp"
#include "shapecraft/app/editor/EditorViewportContainer.hpp"

namespace shapecraft {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setAttribute(Qt::WA_DeleteOnClose);

    auto viewportContainer = new EditorViewportContainer();
    setCentralWidget(viewportContainer);
}

} // namespace shapecraft
