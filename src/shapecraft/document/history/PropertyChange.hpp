#pragma once
#include "Change.hpp"
#include <functional>
#include <unordered_map>

namespace shapecraft {

template <typename T>
class PropertyChange : public Change {
  public:
    PropertyChange(T oldValue, T newValue, std::function<void(const T &)> apply)
        : _oldValue(std::move(oldValue)),
          _newValue(std::move(newValue)),
          _apply(std::move(apply)) {
    }

    void apply() override {
        _apply(_newValue);
    }

    SP<Change> invert() const override {
        return std::make_shared<PropertyChange>(_newValue, _oldValue, _apply);
    }

    bool mergeWith(const SP<const Change> &other) override {
        if (auto change = std::dynamic_pointer_cast<const PropertyChange>(other); change) {
            _newValue = change->_newValue;
            return true;
        }
        return false;
    }

  private:
    T _oldValue;
    T _newValue;
    std::function<void(const T &)> _apply;
};

} // namespace shapecraft
