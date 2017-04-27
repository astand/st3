#include "serv-param.h"
#include <cstring>
#include <cstdlib>

static const char ClearSym = '!';
static const char NullSym = '\0';


static const int32_t param_lengths[] =
{
  SERVER_LENGTH, PORT_LENGTH, APN_LENGTH, USER_LENGTH, PSWRD_LENGTH
};

/// this pointer's array uses for indexed access to each parametr
/// it must be updated before each access by RefreshOutsidePointers
/// for correct behaviour when a few ServParam objects exists.
static char* param_pointers[kParamMaxPosition];


int32_t ServParam::UpdateParametr(ParamPosition pos, const char* param, int32_t len)
{
  if ((uint8_t)pos >= kParamMaxPosition)
    return -1;

  int32_t max_parse_len = param_lengths[pos];
  int32_t actual_param_len = len;

  if (len == 0)
  {
    actual_param_len = std::strlen(param);
  }

  actual_param_len += 1;

  if (actual_param_len < max_parse_len)
    max_parse_len = actual_param_len;

  RefreshOutsidePointers(param_pointers);

  if (param[0] == ClearSym && len == 1)
    param = 0;

  int32_t ret = ReWriteParam(param_pointers[pos], param, max_parse_len);
  return ret;
}


int32_t ServParam::PrintParametr(ParamPosition pos, char* dst) const
{
  if ((uint8_t)pos >= kParamMaxPosition)
    return -1;

  RefreshOutsidePointers(param_pointers);
  int32_t cpylen = std::strlen(param_pointers[pos]);

  if (cpylen > param_lengths[pos] - 1)
    cpylen = param_lengths[pos] - 1;

  std::memcpy((void*)dst, param_pointers[pos], cpylen);
  dst[cpylen] = NullSym;
  return cpylen;
}


const char* ServParam::GetParametr(ParamPosition pos) const
{
  RefreshOutsidePointers(param_pointers);
  return param_pointers[pos];
}


/// This trick makes possible operate with const pointer to instance
/// and get values of params by PrintParametr member
void ServParam::RefreshOutsidePointers(char* ptrs[]) const
{
  ptrs[0] = const_cast<char*>(server_);
  ptrs[1] = const_cast<char*>(port_);
  ptrs[2] = const_cast<char*>(apn_);
  ptrs[3] = const_cast<char*>(user_);
  ptrs[4] = const_cast<char*>(pswrd_);
}


int32_t ServParam::ReWriteParam(char* dst, const char* src, int32_t max_len)
{
  if (src == 0)
  {
    dst[0] = NullSym;
    return -1;
  }

  int32_t i;

  for (i = 0; (i < (max_len - 1) && (src[i] != NullSym)); i++)
  {
    /// make copy param
    dst[i] = src[i];
  }

  /// this garantes that last symbol is EndOfString
  dst[i] = NullSym;
  return i;
}


void ServParam::ResetAll()
{
  server_[0] = NullSym;
  port_[0] = NullSym;
  apn_[0] = NullSym;
  user_[0] = NullSym;
  pswrd_[0] = NullSym;
}

