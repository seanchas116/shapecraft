#pragma once

#include "shapecraft/Common.hpp"
#include <QWidget>

class QTreeView;

namespace shapecraft {

class WindowState;
class Scene;

class NodeListView final : public QWidget {
    Q_OBJECT
  public:
    explicit NodeListView(const SP<WindowState> &state, QWidget *parent = nullptr);

  private:
    void handleCurrentPageChanged(const SP<Scene> &scene);
    void showContextMenu(const QPoint &pos);

    SP<WindowState> _state;
    QTreeView *_treeView = nullptr;
};

} // namespace shapecraft
