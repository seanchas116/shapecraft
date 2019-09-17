#pragma once
#include "lattice/Common.hpp"
#include <QObject>
#include <unordered_set>

namespace lattice {

class Document;
class Layer;

class DocumentActions : public QObject {
    Q_OBJECT
  public:
    DocumentActions(const SP<Document> &document);

    // TODO

  private:
    SP<Document> _document;
};

} // namespace lattice
