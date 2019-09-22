#pragma once
#include <QObject>
#include <unordered_set>

class QKeyEvent;

namespace shapecraft {

class KeyObserver final : public QObject {
    Q_OBJECT
  public:
    static KeyObserver *shared();

    ~KeyObserver() override;

    bool eventFilter(QObject *watched, QEvent *event) override;
    void keyPress(QKeyEvent *event);
    void keyRelease(QKeyEvent *event);

    const auto &pressedKeys() const { return _pressedKeys; }

  signals:
    void pressedKeysChanged(const std::unordered_set<int> &keys);

  private:
    KeyObserver();

    std::unordered_set<int> _pressedKeys;
};

} // namespace shapecraft
