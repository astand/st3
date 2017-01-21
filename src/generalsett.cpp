#include "generalsett.h"

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
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
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
TcpInstance_c::TcpInstance_c()
{
  add[0] = add[1] = add[2] = add[3] = 0;
  port = 10000;
  bigto = 18; /* 3 min */
  silentto = 6;
  reconnto = 12;
}
/* ------------------------------------------------------------------------- */
bool TcpInstance_c::TcpValid() const
{
  if (port == 0)
    return false;

  for (int32_t i = 0; i < 4; i++)
    if (add[i] == 0)
      return false;

  return true;
}
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
void TcpInstance_c::Default()
{
  add[0] = add[1] = add[2] = add[3] = 0;
  port = 10000;
  bigto = 18; /* 3 min */
  silentto = 6;
  reconnto = 12;
}

/* ------------------------------------------------------------------------- */
int32_t TcpInstance_c::Update(char* s)
{
  str0::str0_t* ps = s;
  str0::str0_t* padd;
  uint16_t presence = str0::Presence(s, '#');
  int32_t ret;

  if (!presence--)
    return -3; /* error no valid params */

  ps = str0::ToSym(s, '#', 1);

  for (int i = 0; i < 4; i++)
  {
    /* find all IP positions */
    padd = str0::ToSym(ps, '.', i);
    ret = str0::Atoi(padd);

    if (ret == 0 || ret > 255)
      return -1; /* error no valid value for IP */
    else
      add[i] = (uint8_t) ret;
  }

  if (!presence--)
    return 1;

  ps = str0::ToSym(s, '#', 2);
  ret = str0::Atoi(ps);

  if ((ret == 0) || ret > (0xFFFF))
    return -2; /* error no valid port */

  port = ret;
  return 2;
}
/* ------------------------------------------------------------------------- */
void TcpInstance_c::Print(char* s) const
{
  sprintf(s, "IP:%d.%d.%d.%d PORT:%d GLO:%d IDLE:%d RECONN:%d\n",
          add[0], add[1], add[2], add[3], port, bigto, silentto, reconnto);
}
/* ------------------------------------------------------------------------- */
void TcpInstance_c::PrintIP(char* s) const
{
  sprintf(s, "%d.%d.%d.%d", add[0], add[1], add[2], add[3]);
}
/* ------------------------------------------------------------------------- */
void TcpInstance_c::PrintPort(char* s) const
{
  sprintf(s, "%d", port);
}
/* ------------------------------------------------------------------------- */
int32_t TcpInstance_c::UpdateTim(char* s)
{
  str0::str0_t* ps = s;
  int32_t ret;
  uint16_t presence = str0::Presence(s, '#');

  if (presence != 3)
    return -1; /* error no valid params */

  ps = str0::ToSym(s, '#', 1);
  ret = str0::Atoi(ps);
  bigto = (IsTimValid(ret)) ? (ret) : (bigto);
  ps = str0::ToSym(s, '#', 2);
  ret = str0::Atoi(ps);
  silentto = (IsTimValid(ret)) ? (ret) : (silentto);
  ps = str0::ToSym(s, '#', 3);
  ret = str0::Atoi(ps);
  reconnto = (IsTimValid(ret)) ? (ret) : (reconnto);
  return 0;
}


/* ------------------------------------------------------------------------- */
void TcpInstance_c::PrintTim(char* s) const
{
  sprintf(s, "#%d #%d #%d", bigto, silentto, reconnto);
}
/* ------------------------------------------------------------------------- */
bool TcpInstance_c::IsTimValid(int32_t val) const
{
  return (val != 0) ? (true) : (false);
}

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
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

  for (int i = 0; i < TCP_NUMS; i++)
    IP[i].Default();

  SIGN = CURRENT_SIGN;
  sockbit = SocketBits_c();
}
/* ------------------------------------------------------------------------- */
uint32_t GeneralSett::SocketBits(char* s)
{
  uint32_t ret;
  ret = str0::Presence(s, '#');

  if (!ret)
    /* no valid */
    return 0;

  s = (char*)str0::ToSym(s, '#', 1);
  return sockbit.Socket(str0::Atoi(s));
}
