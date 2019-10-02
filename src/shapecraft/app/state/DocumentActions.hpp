#pragma once
#include "shapecraft/Common.hpp"
#include <QObject>
#include <unordered_set>

namespace shapecraft {

class Document;

class DocumentActions : public QObject {
    Q_OBJECT
  public:
    DocumentActions(const SP<Document> &document);

    bool isNodeSelected() const { return _isNodeSelected; }

    void copyNodes();
    void cutNodes();
    void deleteNodes();
    void pasteNodes();
    void selectAllNodes();

    void addBox();
    void addCylinder();
    void addSphere();
    void addBottle();

  signals:
    void isNodeSelectedChanged(bool selected);

  private:
    void updateIsNodeSelected();

    SP<Document> _document;
    bool _isNodeSelected = false;
};

} // namespace shapecraft
