#include "History.hpp"
#include "Change.hpp"
#include <QUndoCommand>
#include <QUndoStack>

namespace shapecraft {

namespace {

static bool _isDuringUndoRedo = false;

class ChangeCommand : public QUndoCommand {
  public:
    ChangeCommand(int id, const QString &text, const SP<Change> &change) : _id(id), _changes({change}) {
        setText(text);
    }

    void redo() override {
        _isDuringUndoRedo = true;

        for (auto &&change : _changes) {
            change->apply();
        }

        _isDuringUndoRedo = false;
    }

    void undo() override {
        _isDuringUndoRedo = true;

        for (auto &&it = _changes.rbegin(); it != _changes.rend(); ++it) {
            (*it)->invert()->apply();
        }

        _isDuringUndoRedo = false;
    }

    int id() const override {
        return _id;
    }

    bool mergeWith(const QUndoCommand *other) override {
        auto otherChangeCommand = dynamic_cast<const ChangeCommand *>(other);
        if (!otherChangeCommand) {
            return false;
        }

        for (auto &change : otherChangeCommand->_changes) {
            if (_changes[_changes.size() - 1]->mergeWith(change)) {
                continue;
            }
            _changes.push_back(change);
        }
        return true;
    }

  private:
    int _id;
    std::vector<SP<Change>> _changes;
};

} // namespace

History::History() {
    _undoStack = new QUndoStack(this);
}

void History::beginChange(const QString &title) {
    _currentTitle = title;
    ++_currentID;
}

void History::addChange(const SP<Change> &change) {
    Q_ASSERT(!_isDuringUndoRedo);

    auto command = new ChangeCommand(_currentID, _currentTitle, change);
    _undoStack->push(command);
}

void History::clear() {
    _undoStack->clear();
    _currentID = 0;
    _currentTitle.clear();
}

} // namespace shapecraft
