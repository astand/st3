#include "at-pipe.h"
#include <string.h>

int32_t AtPipe::AtSend(pipe_t str, int32_t predelay, bool rx_flush)
{
  int32_t sended_len = framer_.SendCommand(str, predelay, rx_flush);
  return sended_len;
}

/// @ok - valid matching string
/// @alt - string as alternative
/// @resp_time - timeout for polling framer to get valid frames
///
/// @return count of last received frame
int32_t AtPipe::AtRead(pipe_t ok, pipe_t alt, int32_t resp_time)
{
  static pipe_t answer = framer_.Answer();
  int32_t retparse = 0;
  int32_t unknown_len = 0;

  do
  {
    /// block calling
    retparse = framer_.ParseAnswer(resp_time);
    /// for disabling restart resp_time in atp_
    resp_time = 0;

    if (retparse == 0)
    {
      if (unknown_len > 0)
      {
        /// save unknown state and return len
        retparse = unknown_len;
        break;
      }

      /// silent in incoming pipe
      atstate_ = kNone;
      break;
    }
    else if (retparse > 0)
    {
      /// some valid parser received
      if ((ok != 0) && memcmp(answer, ok, strlen(ok)) == 0)
      {
        atstate_ = kOk;
      }
      else if ((alt != 0) && memcmp(answer, alt, strlen(alt)) == 0)
      {
        atstate_ = kAlter;
      }
      else if (failer_.IsAnswerFail(answer, retparse))
      {
        atstate_ = kError;
      }
      else
      {
        /// Unknown answer.
        atstate_ = kUnknown;

        if (ok == 0 && alt == 0)
        {
          /// Stop waiting
          break;
        }

        /// Save unknown length
        unknown_len = retparse;
        /// continue polling
        continue;
      }

      break;
    }
  }
  while (true);

  return retparse;
}

