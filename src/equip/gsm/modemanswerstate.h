#pragma once

#include <stdint.h>

enum ResponseState { kOk, kAlter, kUnknown, kError, kNone };

bool inline is_bad_response(ResponseState st)
{
  return (st == kError || st == kAlter || st == kNone);
}

class ModemAnswerState {

  static const uint32_t kOk       = 1 << 0;
  static const uint32_t kAlter    = 1 << 1;
  static const uint32_t kUnknown  = 1 << 2;

  static const uint32_t kError    = 1 << 30;
  static const uint32_t kNone     = 1 << 29;

 public:
  ModemAnswerState() : state_(0) {}

  void clear() {
    state_ = 0;
  }
  void clear_err() {
    state_ = state_ & 0x00ffFFff;
  }

  void set_ok() {
    state_ |= kOk;
  }
  void set_alter()  {
    state_ |= kAlter;
  }
  void set_unknown()  {
    state_ |= kUnknown;
  }
  void set_err()  {
    state_ |= kError;
  }
  void set_none() {
    state_ |= kNone;
  }


  void unset_ok() {
    state_ &= ~kOk;
  }
  void unset_alter()  {
    state_ &= ~kAlter;
  }
  void unset_unknown()  {
    state_ &= ~kUnknown;
  }
  void unset_err()  {
    state_ &= ~kError;
  }
  void unset_none() {
    state_ &= ~kNone;
  }



  // void NoAnswer(bool state) { none = state; }
  // bool NoAnswer() { return none; }

  // void Alt_ErrorAnswer(bool state) { alt_error = state; }
  // bool Alt_ErrorAnswer() { return alt_error; }

  void Reset() {
    clear();
  }
  bool NotValid()  {
    return (state_ & (kError | kNone | kUnknown));
  }



 private:

  uint32_t state_;
};
