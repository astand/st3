#include "trek-location-descriptor.h"

/* ------------------------------------------------------------------------- *
 * ------------------------------------------------------------------------- *
                                TrekLocationDescriptor ??? (description)
 * ------------------------------------------------------------------------- *
 * ------------------------------------------------------------------------- */
TrekLocationDescriptor::TrekLocationDescriptor()
{
  value = 0;
}
/* ------------------------------------------------------------------------- */
bool TrekLocationDescriptor::IsValid() const
{
  return (value & MATRIX_VALID_M);
}
/* ------------------------------------------------------------------------- */
uint32_t TrekLocationDescriptor::Sector() const
{
  return (value & MATRIX_SECTOR_M) >> MATRIX_SECTOR_P;
}
/* ------------------------------------------------------------------------- */
uint32_t TrekLocationDescriptor::Sector(uint32_t sec)
{
  value &= ~(MATRIX_SECTOR_M);
  value |= (sec << MATRIX_SECTOR_P);
  return value;
}
/* ------------------------------------------------------------------------- */
uint32_t TrekLocationDescriptor::CodeOffset() const
{
  return (value & MATRIX_BIAS_M);
}
/* ------------------------------------------------------------------------- */
uint32_t TrekLocationDescriptor::CodeOffset(uint32_t bias)
{
  value &= ~(MATRIX_BIAS_M);
  value |= bias | MATRIX_VALID_M;
  return value;
}
/* ------------------------------------------------------------------------- */
bool TrekLocationDescriptor::IsTrueOffset(uint32_t vic) const
{
  return (vic < CodeOffset()) ? (true) : (false);
}
/* ------------------------------------------------------------------------- */
uint32_t TrekLocationDescriptor::SetAll(uint32_t sec, uint32_t bias)
{
  value = 0;
  Sector(sec);
  CodeOffset(bias);
  return value;
}
/* ------------------------------------------------------------------------- */
bool TrekLocationDescriptor::GetAll(uint32_t& sec, uint32_t& bias)
{
  if (!IsValid())
    return false;

  sec = Sector();
  bias = CodeOffset();
  return true;
}
/* ------------------------------------------------------------------------- */
uint32_t TrekLocationDescriptor::AddressStart() const
{
  return (Sector() * 4096);
}

/* ------------------------------------------------------------------------- *
  @request_offset   requesting offset position in current treklist item
  this value must ZERO based rank, this will correct handle

  @return - return calculating absolute address for requesting position
  it MUST BE handle with AddressRollCorrection!!!
 * ------------------------------------------------------------------------- */
uint32_t TrekLocationDescriptor::AddressStart(uint32_t request_offset) const
{
  return GetNoteAddress(Sector(), request_offset);
}
/* ------------------------------------------------------------------------- */
uint32_t TrekLocationDescriptor::AddressEnd() const
{
  uint32_t loffset;
  loffset = CodeOffset();

  if (loffset != 0) loffset -= 1;

  return AddressStart(loffset);
}
