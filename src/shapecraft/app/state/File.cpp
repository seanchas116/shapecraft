#include "File.hpp"
#include "shapecraft/document/Document.hpp"
#include "shapecraft/document/Scene.hpp"
#include "shapecraft/document/history/History.hpp"
#include <QFile>
#include <QFileInfo>
#include <QUndoStack>
#include <QtDebug>

namespace shapecraft {

File::File() : _document(std::make_shared<Document>()) {
    auto scene = std::make_shared<Scene>(_document->history());
    _document->appendChildNode(scene);
    _document->setCurrentScene(scene);
    _document->history()->clear();

    connect(_document->history()->undoStack(), &QUndoStack::indexChanged, this, [this] {
        setModified(true);
        _isNew = false;
    });
}

bool File::save() {
    if (_filePath.isEmpty()) {
        return false;
    }
    return saveToFilePath(_filePath);
}

bool File::saveToFilePath(const QString &path) {
    auto data = _document->toJSONRecursive().dump(2);
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    file.write(data.data(), data.size());

    setFilePath(path);
    setModified(false);
    return true;
}

bool File::openFilePath(const QString &path) {
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    auto jsonString = file.readAll().toStdString();
    try {
        auto json = nlohmann::json::parse(jsonString);
        _document->loadJSONRecursive(json);
        _document->setCurrentScene(_document->scenes().at(0));
        _document->history()->clear();
        setFilePath(path);
        setModified(false);
    } catch (const std::exception &ex) {
        qWarning() << ex.what();
        return false;
    }
    return true;
}

QString File::fileName() const {
    if (_filePath.isEmpty()) {
        return tr("Untitled");
    }
    QFileInfo info(_filePath);
    return info.fileName();
}

void File::setFilePath(const QString &filePath) {
    if (_filePath == filePath) {
        return;
    }
    _filePath = filePath;
    emit filePathChanged(_filePath);
}

void File::setModified(bool modified) {
    if (_isModified == modified) {
        return;
    }
    _isModified = modified;
    emit modifiedChanged(_isModified);
}

} // namespace shapecraft
