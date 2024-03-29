#pragma once
#include "shapecraft/Common.hpp"
#include <QObject>

namespace shapecraft {

class File;
class Document;
class DocumentActions;

class WindowState final : public QObject {
    Q_OBJECT
  public:
    WindowState();

    const SP<File> &file() const { return _file; }
    const SP<Document> &document() const;
    const SP<DocumentActions> &documentActions() const { return _documentActions; }

  private:
    SP<File> _file;
    SP<DocumentActions> _documentActions;
};

} // namespace shapecraft
