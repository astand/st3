#include "nmeautil.hpp"

#include "string.h"


/*
* */

/*
* */


/* ------------------------------------------------------------------------- *
 * ------------------------------------------------------------------------- *
                                MATRIX_ITEM
 * ------------------------------------------------------------------------- *
 * ------------------------------------------------------------------------- */
Matrix_item::Matrix_item()
{
}
/* ------------------------------------------------------------------------- */
void Matrix_item::StartDate(const NaviNote* obj)
{
  dat0 = obj->clnd;
}
/* ------------------------------------------------------------------------- */
void Matrix_item::EndDate(const NaviNote* obj)
{
  dat1 = obj->clnd;
}
/* ------------------------------------------------------------------------- */
void Matrix_item::setParam(uint16_t id, uint32_t rawdatasize, uint32_t localkm,
                           uint32_t odometrkm)
{
  this->id = id;
  size = rawdatasize;
  dist = localkm;
  mileage = odometrkm;
}
/* ------------------------------------------------------------------------- */
uint16_t Matrix_item::Lenght()
{
  return sizeof(Matrix_item);
}

/* ------------------------------------------------------------------------- */
int32_t Matrix_item::ToString(char* outprint)
{
  sprintf(outprint, "<%02d:%02d:%02d %02d%02d%02d - ",
          dat0.hr, dat0.min, dat0.sec, dat0.day, dat0.month, dat0.year);
  outprint += strlen(outprint);
  sprintf(outprint, "%02d:%02d:%02d %02d%02d%02d>",
          dat1.hr, dat1.min, dat1.sec, dat1.day, dat1.month, dat1.year);
  outprint += strlen(outprint);
  sprintf(outprint, "ID=%d, DIST=%d, SIZE=%d, MILE=%d\n", id, dist, size, mileage);
  return strlen(outprint);
}


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
