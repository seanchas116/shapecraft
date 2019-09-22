#include "MainWindow.hpp"
#include "NodeListView.hpp"
#include "shapecraft/app/editor/EditorViewportContainer.hpp"
#include "shapecraft/app/state/WindowState.hpp"
#include <QSplitter>

namespace shapecraft {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setAttribute(Qt::WA_DeleteOnClose);

    auto state = std::make_shared<WindowState>();

    auto splitter = new QSplitter();

    auto nodeListView = new NodeListView(state);
    splitter->addWidget(nodeListView);

    auto viewportContainer = new EditorViewportContainer(state);
    splitter->addWidget(viewportContainer);

    setCentralWidget(splitter);
}

} // namespace shapecraft
