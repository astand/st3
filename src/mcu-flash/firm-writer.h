#pragma once

#include <stdint.h>
#include "utility/abstract/io/ISectorWriter.h"

class FirmWriter : public ISectorWriter {
 public:
  FirmWriter();

 public:
  // Erase sector
  virtual bool Erase();
  // check if driver can
  virtual bool IsBusy();
  virtual int32_t Program(const uint8_t* mem, uint32_t length);
  virtual int32_t Program(const uint8_t* mem, uint32_t offset, uint32_t length);
  virtual int32_t ProgramTo(const uint8_t* mem, uint32_t addr, uint32_t length);

 private:
  bool flashfailed;
  uint32_t memaddr;

 private:
  int32_t ProgramAndCheck(uint32_t addr, uint32_t data);
};
