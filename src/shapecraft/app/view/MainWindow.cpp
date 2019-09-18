#include "MainWindow.hpp"
#include "shapecraft/app/editor/EditorViewportContainer.hpp"
#include "shapecraft/app/state/WindowState.hpp"

namespace shapecraft {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setAttribute(Qt::WA_DeleteOnClose);

    auto state = std::make_shared<WindowState>();

    auto viewportContainer = new EditorViewportContainer(state);
    setCentralWidget(viewportContainer);
}

} // namespace shapecraft
