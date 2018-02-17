#include "m66-fail-detector.h"
#include <string.h>

static const int32_t kFailCnt = 3;

static const char* err_strs[kFailCnt] =
{
  "ERROR",
  "NO CARRIER",
  "+CMS ERROR:"
};


int32_t M66FailDetector::IsAnswerFail(const char* instr, int32_t length)
{
  int32_t compare_len = strlen(instr);

  for (int32_t i = 0; i < kFailCnt; i++)
    if (memcmp(instr, err_strs[i], compare_len) == 0)
      return i + 1;

  return 0;
}
