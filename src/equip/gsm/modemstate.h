#pragma once

#include <stdint.h>

enum GprsState
{
  kIdle = 0,
  kNeedRegister,
  kNeedDeactive,
  kActive
};

enum ModemState
{
  kNoecho = 0,
  kNoSim,
  kNoReg,
  kRegOk,
  kConnected,
  kOff
};

class ModemOperateState {
 public:


  ModemOperateState() : _opstate(kNoecho) { }

  void Change(ModemState newstat) {
    _opstate = newstat;
  }

  ModemState GetActual() {
    return _opstate;
  }

  void RegisterState(bool condition) {
    if (_opstate != kConnected)
      _opstate = (condition) ? (kRegOk) : (kNoReg);
  }

 private:
  ModemState _opstate;

};
