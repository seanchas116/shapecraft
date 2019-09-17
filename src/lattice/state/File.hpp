#pragma once
#include "lattice/Common.hpp"
#include <QObject>

namespace lattice {

class Document;

class File : public QObject {
    Q_OBJECT
  public:
    File();

    bool save();
    bool saveAs();
    bool saveToFilePath(const QString &path);

    bool openFilePath(const QString &path);

    auto &&document() const { return _document; }

    QString fileName() const;
    QString filePath() const { return _filePath; }
    bool isModified() const { return _isModified; }

    bool isNew() const { return _isNew; }

  signals:
    void filePathChanged(const QString &filePath);
    void modifiedChanged(bool modified);

  private:
    void setFilePath(const QString &filePath);
    void setModified(bool modified);

    SP<Document> _document;
    QString _filePath;
    bool _isModified = false;
    bool _isNew = true;
};

} // namespace lattice
