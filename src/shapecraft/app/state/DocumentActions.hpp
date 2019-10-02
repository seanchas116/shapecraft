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

    void copyNodes();
    void cutNodes();
    void deleteNodes();
    void pasteNodes();
    void selectAllNodes();

    // TODO

  private:
    SP<Document> _document;
};

} // namespace shapecraft
