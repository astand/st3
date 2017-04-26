#include "serv-param.h"

static const int32_t param_lengths[] =
{
  SERVER_LENGTH, PORT_LENGTH, APN_LENGTH, USER_LENGTH, PSWRD_LENGTH
};

static char* param_pointers[kParamMaxPosition];


int32_t ServParam::UpdateParametr(ParamPosition pos, const char* param)
{
  if ((uint8_t)pos >= kParamMaxPosition)
    return -1;

  RefreshOutsidePointers(param_pointers);
  int32_t ret = ReWriteParam(param_pointers[pos], param, param_lengths[pos]);
  return ret;
}


int32_t ServParam::PrintParametr(ParamPosition pos, char* dst)
{
  if ((uint8_t)pos >= kParamMaxPosition)
    return -1;

  RefreshOutsidePointers(param_pointers);
  int32_t ret = ReWriteParam(dst, param_pointers[pos], param_lengths[pos]);
  return ret;
}


void ServParam::RefreshOutsidePointers(char* ptrs[])
{
  ptrs[0] = server_;
  ptrs[1] = port_;
  ptrs[2] = apn_;
  ptrs[3] = user_;
  ptrs[4] = pswrd_;
}


int32_t ServParam::ReWriteParam(char* dst, const char* src, int32_t max_len)
{
  if (src == 0)
  {
    dst[0] = '\0';
    return -1;
  }

  int32_t i;

  for (i = 0; (i < (max_len - 1) && (src[i] != '\0')); i++)
  {
    /// make copy param
    dst[i] = src[i];
  }

  /// this garantes that last symbol is EndOfString
  dst[i] = '\0';
  return i;
}

