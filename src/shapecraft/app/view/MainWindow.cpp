#include "MainWindow.hpp"
#include "NodeListView.hpp"
#include "shapecraft/app/editor/EditorViewportContainer.hpp"
#include "shapecraft/app/state/File.hpp"
#include "shapecraft/app/state/WindowState.hpp"
#include "shapecraft/util/Debug.hpp"
#include <QFileDialog>
#include <QSplitter>

namespace shapecraft {

MainWindow::MainWindow(const SP<WindowState> &state, QWidget *parent) : QMainWindow(parent), _state(state) {
    setAttribute(Qt::WA_DeleteOnClose);

    auto splitter = new QSplitter();

    auto nodeListView = new NodeListView(_state);
    splitter->addWidget(nodeListView);

    auto viewportContainer = new EditorViewportContainer(_state);
    splitter->addWidget(viewportContainer);

    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    setCentralWidget(splitter);
}

void MainWindow::actualSize() {
    qWarning() << "TODO";
}

void MainWindow::zoomIn() {
    qWarning() << "TODO";
}

void MainWindow::zoomOut() {
    qWarning() << "TODO";
}

void MainWindow::open() {
    auto openDialog = new QFileDialog(this, Qt::Sheet);
    openDialog->setAcceptMode(QFileDialog::AcceptOpen);
    openDialog->setNameFilter("Grid document (*.grid)");
    openDialog->open();
    connect(openDialog, &QFileDialog::finished, this, [openDialog, this](int result) {
        openDialog->deleteLater();
        if (result == QDialog::Rejected) {
            return;
        }
        auto selectedFiles = openDialog->selectedFiles();
        if (selectedFiles.isEmpty()) {
            return;
        }

        if (_state->file()->isNew()) {
            _state->file()->openFilePath(selectedFiles[0]);
            return;
        }

        auto newState = std::make_shared<WindowState>();
        newState->file()->openFilePath(selectedFiles[0]);

        auto newWindow = new MainWindow(newState);
        newWindow->show();
    });
}

void MainWindow::save() {
    if (_state->file()->filePath().isEmpty()) {
        saveAs();
        return;
    }
    _state->file()->save();
}

void MainWindow::saveAs() {
    auto saveDialog = new QFileDialog(this, Qt::Sheet);
    saveDialog->setAcceptMode(QFileDialog::AcceptSave);
    saveDialog->setNameFilter("Grid document (*.grid)");
    saveDialog->open();
    connect(saveDialog, &QFileDialog::finished, this, [this, saveDialog](int result) {
        saveDialog->deleteLater();
        if (result == QDialog::Rejected) {
            return;
        }
        auto selectedFiles = saveDialog->selectedFiles();
        if (selectedFiles.isEmpty()) {
            return;
        }
        _state->file()->saveToFilePath(selectedFiles[0]);
    });
}

void MainWindow::updateWindowFilePath(const QString &filePath) {
    if (filePath.isEmpty()) {
        setWindowTitle(_state->file()->fileName());
    } else {
        setWindowTitle(QString());
        setWindowFilePath(filePath);
    }
}

} // namespace shapecraft
