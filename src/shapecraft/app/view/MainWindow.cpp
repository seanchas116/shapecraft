#include "MainWindow.hpp"
#include "NodeListView.hpp"
#include "shapecraft/app/editor/EditorViewportContainer.hpp"
#include "shapecraft/app/state/DocumentActions.hpp"
#include "shapecraft/app/state/File.hpp"
#include "shapecraft/app/state/WindowState.hpp"
#include "shapecraft/document/Document.hpp"
#include "shapecraft/document/history/History.hpp"
#include "shapecraft/util/Debug.hpp"
#include <QFileDialog>
#include <QMenuBar>
#include <QSettings>
#include <QSplitter>
#include <QUndoStack>

namespace shapecraft {

MainWindow::MainWindow(const SP<WindowState> &state, QWidget *parent) : QMainWindow(parent), _state(state) {
    setAttribute(Qt::WA_DeleteOnClose);

    {
        auto splitter = new QSplitter();

        auto nodeListView = new NodeListView(_state);
        splitter->addWidget(nodeListView);

        auto viewportContainer = new EditorViewportContainer(_state);
        splitter->addWidget(viewportContainer);

        splitter->setStretchFactor(0, 0);
        splitter->setStretchFactor(1, 1);

        setCentralWidget(splitter);
    }

    {
        auto menuBar = new QMenuBar();

        {
            auto fileMenu = menuBar->addMenu(tr("File"));
            fileMenu->addAction(tr("Open..."), this, &MainWindow::open, QKeySequence::Open);
            fileMenu->addSeparator();
            fileMenu->addAction(tr("Close"), this, &QMainWindow::close, QKeySequence::Close);
            fileMenu->addAction(tr("Save"), this, &MainWindow::save, QKeySequence::Save);
            fileMenu->addAction(tr("Save As..."), this, &MainWindow::saveAs, QKeySequence::SaveAs);
        }

        {
            auto editMenu = menuBar->addMenu(tr("Edit"));

            auto undoStack = _state->document()->history()->undoStack();

            auto undoAction = undoStack->createUndoAction(this);
            undoAction->setShortcut(QKeySequence::Undo);
            editMenu->addAction(undoAction);

            auto redoAction = undoStack->createRedoAction(this);
            redoAction->setShortcut(QKeySequence::Redo);
            editMenu->addAction(redoAction);

            editMenu->addSeparator();

            auto cutAction = editMenu->addAction(tr("Cut"), _state->documentActions().get(), &DocumentActions::cutNodes, QKeySequence::Cut);
            auto copyAction = editMenu->addAction(tr("Copy"), _state->documentActions().get(), &DocumentActions::copyNodes, QKeySequence::Copy);
            editMenu->addAction(tr("Paste"), _state->documentActions().get(), &DocumentActions::pasteNodes, QKeySequence::Paste);
            auto deleteAction = editMenu->addAction(tr("Delete"), _state->documentActions().get(), &DocumentActions::deleteNodes, QKeySequence::Delete);
            editMenu->addAction(tr("Select All"), _state->documentActions().get(), &DocumentActions::selectAllNodes, QKeySequence::SelectAll);

            for (auto action : {cutAction, copyAction, deleteAction}) {
                connect(_state->documentActions().get(), &DocumentActions::isNodeSelectedChanged, action, &QAction::setEnabled);
                action->setEnabled(_state->documentActions()->isNodeSelected());
            }
        }

        {
            auto viewMenu = menuBar->addMenu(tr("View"));
            viewMenu->addAction(tr("Actual Size"), this, &MainWindow::actualSize, QKeySequence("Ctrl+0"));
            viewMenu->addAction(tr("Zoom In"), this, &MainWindow::zoomIn, QKeySequence::ZoomIn);
            viewMenu->addAction(tr("Zoom Out"), this, &MainWindow::zoomOut, QKeySequence::ZoomOut);
        }

        setMenuBar(menuBar);
    }

    connect(_state->file().get(), &File::filePathChanged, this, &MainWindow::updateWindowFilePath);
    updateWindowFilePath(_state->file()->filePath());

    connect(_state->file().get(), &File::modifiedChanged, this, &QMainWindow::setWindowModified);
    setWindowModified(_state->file()->isModified());

    resize(800, 600);

    QSettings settings;
    restoreState(settings.value("windowState").toByteArray());
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
