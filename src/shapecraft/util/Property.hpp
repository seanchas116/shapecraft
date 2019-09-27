#pragma once

#include <QObject>

#define SHAPECRAFT_PROPERTY(TYPE, NAME, SETNAME, DEFAULTVALUE)         \
    Q_PROPERTY(TYPE NAME READ NAME WRITE SETNAME NOTIFY NAME##Changed) \
  public:                                                              \
    const TYPE &NAME() const { return _##NAME; }                       \
    void SETNAME(const TYPE &value) {                                  \
        if (_##NAME == value)                                          \
            return;                                                    \
        _##NAME = value;                                               \
        emit NAME##Changed(value);                                     \
    }                                                                  \
    Q_SIGNAL void NAME##Changed(const TYPE &value);                    \
                                                                       \
  private:                                                             \
    TYPE _##NAME = DEFAULTVALUE;                                       \
                                                                       \
  public:
