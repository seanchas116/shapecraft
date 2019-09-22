#pragma once
#include <QObject>
#include <unordered_set>

class QKeyEvent;

namespace shapecraft {

class KeyObserver final : public QObject {
    Q_OBJECT
  public:
    KeyObserver(QObject *parent = nullptr);
    ~KeyObserver() override;

    bool eventFilter(QObject *watched, QEvent *event) override;
    void keyPress(QKeyEvent *event);
    void keyRelease(QKeyEvent *event);

    const auto &pressedKeys() const { return _pressedKeys; }

  signals:
    void pressedKeysChanged(const std::unordered_set<int> &keys);

  private:
    std::unordered_set<int> _pressedKeys;
};

} // namespace shapecraft
