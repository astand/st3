#include "bin-pipe.h"

namespace Stuff
{

BinaryPipe::BinaryPipe(IStreamable& strm) : strm_(strm), crc_(), framer_(crc_)
{}


int32_t BinaryPipe::Read()
{
  /// This pipe don't perfoms byteable Read operations
  return -1;
}


int32_t BinaryPipe::Read(uint8_t* buff, int32_t start, int32_t count)
{
  int32_t ret;
  ret = framer_.UnPackFrame(strm_, buff + start, count);
  return ret;
}


int32_t BinaryPipe::Write(uint8_t byte)
{
  /// This pipe don't perfoms byteable Write operations
  return -1;
}


int32_t BinaryPipe::Write(const uint8_t* buff, int32_t start, int32_t count)
{
  int32_t ret;
  ret = framer_.PackFrame(buff + start, count, strm_);
  return ret;
}

};

