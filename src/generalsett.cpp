#include "generalsett.h"

/* ------------------------------------------------------------------------- */
uint32_t SocketBits_c::Socket()
{
  for (uint8_t i = 0; i < 5; i++)
    if (b & (1 << i)) return (i + 1);

  return 0;
}
/* ------------------------------------------------------------------------- */
uint32_t SocketBits_c::Socket(uint32_t num)
{
  if (num > 5)
    return Socket();

  b &= ~(BITS_MASK);

  if (num == 0)
    return Socket();

  num--;
  b |= (1 << num);
  return Socket();
}

/* ------------------------------------------------------------------------- */
GeneralSett::GeneralSett()
{
//	for (int i = 0; i < 64; i++)
//		ID[i] = 0;
  SIGN = CURRENT_SIGN;
}

/* ------------------------------------------------------------------------- */
bool GeneralSett::JCIsSign()
{
  return (SIGN == CURRENT_SIGN);
}
/* ------------------------------------------------------------------------- */
uint32_t GeneralSett::Lenght()
{
  return sizeof(GeneralSett);
}
/* ------------------------------------------------------------------------- */
void GeneralSett::JCDefault()
{
  for (int i = 0; i < ID_LENGHT; i++)
    ID[i] = 0;

  ID[0] = 'H';
  ID[1] = 'e';
  gprs[0].Default();
  gprs[1].Default();
  gprs[2].Default();
  SIGN = CURRENT_SIGN;
  sockbit = SocketBits_c();
}
/* ------------------------------------------------------------------------- */
uint32_t GeneralSett::SocketBits(char* s)
{
  return sockbit.Socket(str0::Atoi(s));
}
