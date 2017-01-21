#include "clnd.hpp"

/* ------------------------------------------------------------------------- */
bool operator>(const clnd0& left, const clnd0& right)
{
  for (int i = 0 ; i < 6; i++)
    if (((clnd0::clnd0_t*) & (left))[i] > ((clnd0::clnd0_t*) & (right))[i])
      return true;

  return false;
}

/* ------------------------------------------------------------------------- */
clnd0::clnd0()
{
  year = month = day = hr = min = sec = 0;
}

/* ------------------------------------------------------------------------- */
clnd0& clnd0::operator=(const clnd0& right)
{
  if (this == &right)
    return *this;

  for (int i = 0 ; i < 6; i++)
    (((clnd0::clnd0_t*)(this))[i] = ((clnd0::clnd0_t*) & (right))[i]);

  return *this;
}

/* ------------------------------------------------------------------------- */
int32_t clnd0::Lenght()
{
  return (sizeof(clnd0_t) * 6);
}
