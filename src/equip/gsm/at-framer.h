#pragma once
#include "abstract/IStreamable.h"
#include "timers/timer.h"

class AtFramer {
 public:
  typedef char strbuf_t;
  typedef void (*waiter)();

 public:
  AtFramer(IStreamable& strm, int32_t rx_length, waiter wtr);

 public:
  int32_t SendCommand(const char* cmd, int32_t ms = 0, bool rx_flush = true);

  /// Read flow from pipe.
  /// @ms count of msecs for polling.
  /// When @ms equal to zero inner timer doesn't restrat.
  /// @return 0 if no valid frame in period or count of symbols in frame.
  int32_t ParseAnswer(int32_t ms);

 public:
  const strbuf_t* Answer() {
    return str_buf_;
  }

  int32_t Lenght();

 private:
  static const int32_t kMaxMsgLen = 512;
  const int32_t kLength;
  const waiter wait_f;

  Timers::Timer tim_;
  IStreamable& stream;

  uint32_t rec_len_;
  strbuf_t str_buf_[kMaxMsgLen];

 private:
  int32_t ReceieveAnswer();
  uint32_t StripFrame(strbuf_t);
  int32_t SendPacket(const char* str, int32_t leght);

};

