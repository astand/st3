#pragma once

#include <stdint.h>
#include "bridge/abstract/ISwitchable.h"

class MultiLed : public ISwitchable {
 public:
  MultiLed() {}
  // this calls perform switching object's state
  virtual void on() {
    int32_t i = 0;

    while (switchs[i] != 0 && i < 10) {
      switchs[i++]->on();
    }
  }
  virtual void off() {
    int32_t i = 0;

    while (switchs[i] != 0 && i < 10) {
      switchs[i++]->off();
    }
  }
  virtual bool ison() const {
    return false;
  }

// perform on/off action depends on condition.
// return current state
  virtual bool Set(bool) {
    return false;
  }

  virtual bool Toggle() {
    int32_t i = 0;

    while (switchs[i] != 0 && i < 10) {
      switchs[i++]->Toggle();
    }
    
    return true;
  }

 public:
  void Add(ISwitchable* next) {
    if (count < 10) {
      switchs[count++] = next;
    }
  }
 private:
  ISwitchable* switchs[10];
  int32_t count;
};

