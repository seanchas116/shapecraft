#include "NodeListView.hpp"
#include "NodeItemModel.hpp"
#include "shapecraft/app/state/WindowState.hpp"
#include "shapecraft/document/Document.hpp"
#include "shapecraft/document/Scene.hpp"
#include <QMenu>
#include <QTreeView>
#include <QVBoxLayout>

namespace shapecraft {

NodeListView::NodeListView(const SP<WindowState> &state, QWidget *parent)
    : QWidget(parent),
      _state(state) {
    _treeView = new QTreeView();
    _treeView->setHeaderHidden(true);
    _treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    _treeView->setDragDropMode(QAbstractItemView::DragDrop);
    _treeView->setDefaultDropAction(Qt::MoveAction);
    _treeView->setDropIndicatorShown(true);
    _treeView->setAcceptDrops(true);

    _treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_treeView, &QWidget::customContextMenuRequested, this, &NodeListView::showContextMenu);

    auto layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->addWidget(_treeView);
    setLayout(layout);

    connect(state->document().get(), &Document::currentSceneChanged, this, &NodeListView::handleCurrentPageChanged);
    handleCurrentPageChanged(state->document()->currentScene());
}

void NodeListView::handleCurrentPageChanged(const SP<Scene> &scene) {
    auto model = new NodeItemModel(scene);
    auto selectionModel = new QItemSelectionModel(model); // TODO: connect selection

    connect(scene.get(), &Scene::selectedNodesChanged, selectionModel, [scene = scene.get(), selectionModel, model]() {
        QItemSelection selection;
        for (auto node : scene->selectedNodes()) {
            auto index = model->indexForNode(node);
            selection.select(index, index);
        }
        selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);
    });

    connect(selectionModel, &QItemSelectionModel::selectionChanged, scene.get(), [scene = scene.get(), selectionModel, model] {
        std::unordered_set<SP<Node>> nodes;
        for (auto index : selectionModel->selectedIndexes()) {
            nodes.insert(model->nodeForIndex(index));
        }
        scene->setSelectedNodes(std::move(nodes));
    });

    _treeView->setModel(model);
    _treeView->setSelectionModel(selectionModel);
}

void NodeListView::showContextMenu(const QPoint &pos) {
    Q_UNUSED(pos)
    /*
    auto index = _treeView->indexAt(pos);
    QMenu contextMenu;
    if (index.isValid()) {
        contextMenu.addAction(tr("Cut"), _state->documentActions().get(), &DocumentActions::cutLayers, QKeySequence::Cut);
        contextMenu.addAction(tr("Copy"), _state->documentActions().get(), &DocumentActions::copyLayers, QKeySequence::Copy);
    }
    contextMenu.addAction(tr("Paste"), _state->documentActions().get(), &DocumentActions::pasteLayers, QKeySequence::Paste);
    if (index.isValid()) {
        contextMenu.addAction(tr("Delete"), _state->documentActions().get(), &DocumentActions::deleteLayers, QKeySequence::Delete);
    }
    contextMenu.addSeparator();
    contextMenu.addAction(tr("Select All"), _state->documentActions().get(), &DocumentActions::selectAllLayers, QKeySequence::SelectAll);
    contextMenu.exec(_treeView->mapToGlobal(pos));
    */
}

} // namespace shapecraft
