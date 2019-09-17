#include "DocumentActions.hpp"
#include "lattice/document/Document.hpp"
#include <QApplication>
#include <QClipboard>
#include <QtDebug>

namespace lattice {

DocumentActions::DocumentActions(const SP<Document> &document) : _document(document) {
}

} // namespace lattice
