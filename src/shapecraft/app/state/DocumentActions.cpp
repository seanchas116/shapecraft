#include "DocumentActions.hpp"
#include "shapecraft/document/Document.hpp"
#include <QApplication>
#include <QClipboard>
#include <QtDebug>

namespace shapecraft {

DocumentActions::DocumentActions(const SP<Document> &document) : _document(document) {
}

} // namespace shapecraft
