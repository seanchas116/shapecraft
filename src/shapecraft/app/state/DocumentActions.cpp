#include "DocumentActions.hpp"
#include "shapecraft/document/Document.hpp"
#include "shapecraft/document/Scene.hpp"
#include "shapecraft/document/history/History.hpp"
#include <QApplication>
#include <QClipboard>
#include <QtDebug>

namespace shapecraft {

DocumentActions::DocumentActions(const SP<Document> &document) : _document(document) {
    connect(document.get(), &Document::selectedNodesChanged, this, &DocumentActions::updateIsNodeSelected);
}

void DocumentActions::copyNodes() {
    auto clipboard = QApplication::clipboard();
    clipboard->setMimeData(Node::toNodesMimeData(_document->currentScene()->normalizedSelectedNodes()));
}

void DocumentActions::cutNodes() {
    _document->history()->beginChange(tr("Cut Shapes"));
    auto clipboard = QApplication::clipboard();
    clipboard->setMimeData(Node::toNodesMimeData(_document->currentScene()->normalizedSelectedNodes()));
    _document->currentScene()->deleteSelectedNodes();
}

void DocumentActions::deleteNodes() {
    _document->history()->beginChange(tr("Delete Shapes"));
    _document->currentScene()->deleteSelectedNodes();
}

void DocumentActions::pasteNodes() {
    auto clipboard = QApplication::clipboard();
    auto mimeData = clipboard->mimeData();

    try {
        auto nodes = Node::fromNodesMimeData(mimeData, _document->history());
        if (nodes.empty()) {
            return;
        }
        _document->history()->beginChange(tr("Paste Shapes"));
        for (auto &&node : nodes) {
            _document->currentScene()->insertNodeToCurrentPosition(node);
        }
        std::unordered_set<SP<Node>> nodeSet(nodes.begin(), nodes.end());
        _document->currentScene()->setSelectedNodes(nodeSet);
    } catch (const std::exception &error) {
        qWarning() << "Error reading clipboard:" << error.what();
    }
}

void DocumentActions::selectAllNodes() {
    std::unordered_set<SP<Node>> allNodes;
    _document->currentScene()->forEachDescendant([&](auto &node) {
        if (node == _document->currentScene()) {
            return;
        }
        allNodes.insert(node);
    });
    _document->currentScene()->setSelectedNodes(std::move(allNodes));
}

void DocumentActions::updateIsNodeSelected() {
    bool isNodeSelected = !_document->currentScene()->selectedNodes().empty();
    if (_isNodeSelected == isNodeSelected) {
        return;
    }
    _isNodeSelected = isNodeSelected;
    emit isNodeSelectedChanged(isNodeSelected);
}

} // namespace shapecraft
