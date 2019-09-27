#pragma once
#include "history/PropertyChange.hpp"
#include "shapecraft/Common.hpp"
#include <QObject>
#include <QPointF>
#include <QRectF>
#include <functional>
#include <nlohmann/json.hpp>
#include <optional>
#include <vector>

class QMimeData;

#define SHAPECRAFT_UNDOABLE_PROPERTY(TYPE, NAME, SETNAME, DEFAULTVALUE)                                            \
  private:                                                                                                         \
    Q_PROPERTY(TYPE NAME READ NAME WRITE SETNAME NOTIFY NAME##Changed)                                             \
    TYPE _##NAME = DEFAULTVALUE;                                                                                   \
                                                                                                                   \
  public:                                                                                                          \
    const TYPE &NAME() const { return _##NAME; }                                                                   \
    void SETNAME(const TYPE &value) {                                                                              \
        if (_##NAME == value) {                                                                                    \
            return;                                                                                                \
        }                                                                                                          \
        addChange(std::make_shared<PropertyChange<TYPE>>(_##NAME, value, [self = sharedFrom(this)](auto &&value) { \
            self->_##NAME = value;                                                                                 \
            emit self->nameChanged(value);                                                                         \
        }));                                                                                                       \
    }                                                                                                              \
    Q_SIGNAL void NAME##Changed(const TYPE &value);

namespace shapecraft {

class Change;
class History;

class Node : public QObject, public std::enable_shared_from_this<Node> {
    Q_OBJECT
  public:
    Node(const SP<History> &history);

    SHAPECRAFT_UNDOABLE_PROPERTY(QString, name, setName, "")

    Opt<SP<Node>> parentNode() const;
    Opt<SP<Node>> nextNode() const;

    const std::vector<SP<Node>> &childNodes() const { return _childNodes; }
    void setChildNodes(const std::vector<SP<Node>> &nodes);
    virtual bool canHaveChildren() const = 0;
    virtual bool canInsertNode(const SP<Node> &node) const = 0;
    void appendChildNode(const SP<Node> &node);
    void insertNodeBefore(const SP<Node> &node, const Opt<SP<const Node>> &reference);
    void removeChildNode(const SP<Node> &node);

    int index() const;
    std::vector<int> indexPath() const;

    std::vector<SP<Node>> ancestors() const;
    void forEachDescendant(const std::function<void(const SP<Node> &)> &callback) const;

    virtual SP<Node> newInstance(const SP<History> &history) const = 0;
    SP<Node> clone(const SP<History> &history) const;

    virtual void toJSON(nlohmann::json &json) const;
    virtual void fromJSON(const nlohmann::json &json);

    nlohmann::json toJSONRecursive() const;
    static SP<Node> fromJSONRecursive(const nlohmann::json &json, const SP<History> &history);

    virtual QString type() const = 0;
    static void addPrototype(const SP<Node> &prototype);
    auto &&prototypes() const { return _prototypes; }

    auto &&history() const { return _history; }

  signals:
    void childNodesAboutToBeInserted(int first, int last, const std::vector<SP<Node>> &nodes);
    void childNodesInserted(int first, int last);
    void childNodesAboutToBeRemoved(int first, int last);
    void childNodesRemoved(int first, int last, const std::vector<SP<Node>> &nodes);

    void appearanceChanged();

  protected:
    void addChange(const SP<Change> &change);
    template <typename T>
    SP<T> sharedFrom(T *ptr) {
        return std::static_pointer_cast<T>(static_cast<Node *>(ptr)->shared_from_this());
    }

  private:
    class ChildInsertChange;
    class ChildRemoveChange;

    void insertNodeBeforeInternal(const SP<Node> &node, const Opt<SP<const Node>> &reference);
    void removeChildNodeInternal(const SP<Node> &node);

    SP<History> _history;
    WP<Node> _parentNode;
    std::vector<SP<Node>> _childNodes;

    static std::unordered_map<QString, SP<Node>> _prototypes;
};

} // namespace shapecraft
