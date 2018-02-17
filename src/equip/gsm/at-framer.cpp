#include <string.h>
#include "at-framer.h"

/// Low acceptable symbol
static const AtFramer::strbuf_t LOWCHAR = 0x20 - 1;
/// High acceptable symbol
static const AtFramer::strbuf_t HIGHCHAR = 0x7f;

static bool IsAtCharacter(AtFramer::strbuf_t sym)
{
  return (sym > LOWCHAR && sym < HIGHCHAR);
}


AtFramer::AtFramer(IStreamable& strm, int32_t rx_length, waiter wtr) :
  kLength(rx_length), wait_f(wtr), stream(strm)
{
}


int32_t AtFramer::Lenght()
{
  return rec_len_;
}


int32_t AtFramer::SendCommand(const char* cmd, int32_t ms, bool rx_flush)
{
  /// wait pre cmd delay if need
  if (ms > 0)
  {
    tim_.Start(ms);

    while (tim_.Elapsed() == false)
      wait_f();
  }

  /// flush income pipe if need
  if (rx_flush)
    stream.FFlushRead();

  return SendPacket((char*)cmd, strlen(cmd));
}


int32_t AtFramer::SendPacket(const char* str, int32_t count)
{
  int32_t res = 0;
  int32_t passed = 0;

  while (passed != count && (res >= 0))
  {
    res = stream.Write((uint8_t)str[passed]);
    passed++;
  }

  return passed;
}


int32_t AtFramer::ParseAnswer(int32_t ms)
{
  if (ms > 0)
    /// Restart wait timer
    tim_.Start(ms);

  /// flush container
  rec_len_ = 0;

  do
  {
    int32_t resrec = ReceieveAnswer();

    if (resrec > 0)
      return resrec;

    /// do async here
    wait_f();
  }
  while (tim_.Ticks());

  return 0;
}


int32_t AtFramer::ReceieveAnswer()
{
  int32_t ret;
  uint32_t at_answer;
  strbuf_t ch;
  ret = stream.Read();

  while (ret >= 0)
  {
    ch = static_cast<strbuf_t>(ret);
    at_answer = StripFrame(ch);

    if (at_answer > 0)
      return at_answer;

    ret = stream.Read();
  }

  return 0;
}


uint32_t AtFramer::StripFrame(strbuf_t ch)
{
  bool is_sym = IsAtCharacter(ch);

  if (rec_len_ >= (kMaxMsgLen - 1))
    /// avoiding overhead
    is_sym = false;

  if (!is_sym)
  {
    if (rec_len_ > 0)
    {
      /* end of chain */
      str_buf_[rec_len_++] = 0;
      return rec_len_ - 1;
    }

    /// search first sym here
  }
  else
  {
    /// fill message
    str_buf_[rec_len_++] = ch;
  }

  return 0;
}


