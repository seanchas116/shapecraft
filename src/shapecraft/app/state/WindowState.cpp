#include "WindowState.hpp"
#include "DocumentActions.hpp"
#include "File.hpp"
#include "shapecraft/document/Document.hpp"
#include <QtDebug>

namespace shapecraft {

WindowState::WindowState()
    : _file(std::make_shared<File>()),
      _documentActions(std::make_shared<DocumentActions>(_file->document())) {
}

const SP<Document> &WindowState::document() const {
    return _file->document();
}

} // namespace shapecraft
