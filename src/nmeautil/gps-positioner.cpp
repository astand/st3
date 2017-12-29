#include "gps-positioner.h"
#include "etc/str0.h"

static const int8_t P_RMC_VALID	= 2;
static const int8_t P_RMC_TIME = 1;
static const int8_t P_RMC_DATE = 9;
// static const int8_t	P_RMC_SPEED = 7;
static const int8_t	P_RMC_LAT = 3;
static const int8_t	P_RMC_LATNS = 4;
static const int8_t	P_RMC_LONG = 5;
static const int8_t	P_RMC_LONGEW = 6;
// static const int8_t	P_VTG_VALID  = 9;
static const int8_t	P_VTG_SPD = 7;
static const int8_t	P_VTG_COERSE = 1;
static const int8_t P_GGA_ALT = 9;

static const uint32_t kRMC_Prefix = 0x2C434D52;
static const uint32_t kVTG_Prefix = 0x2C475456;
static const uint32_t kGGA_Prefix = 0x2C414747;
// static const uint32_t kGLL_Prefix = 0x2C4C4C47;


static int32_t SetL(const char* b, uint8_t l)
{
  uint32_t grd = str0::Atoi(b, l);
  uint32_t gmin = str0::Atoi((b + l), 2);
  uint32_t gsec = str0::Atoi((b + l + 3), 4);
  uint32_t gdectail = ((((10000 * gmin) + gsec) * 10 ) / 6);
  return grd * 1000000 + gdectail;
}


GpsPositioner::GpsPositioner(NmeaStringReader& provider) : reader_(provider)
{
}

GpsUpdate_t GpsPositioner::UpdatePosition(GpsPositionData_t* note)
{
  posdat_->update = kNoUpdate;

  if (reader_.TryReadNmea(gpstr, kStrLen) > 0)
  {
    posdat_ = note;
    UnpackNmeaString();
  }

  return kNoUpdate;
}

void GpsPositioner::UnpackNmeaString()
{
  static uint32_t* pmark = (uint32_t*)(gpstr + 2);

  if (*pmark == kRMC_Prefix)
  {
    UnpackRMC();
  }
  else if (*pmark == kVTG_Prefix)
  {
    UnpackVTG();
  }
  else if (*pmark == kGGA_Prefix)
  {
    // altitude actually
    UnpackGGA();
  }
  else
    posdat_->update = kUnknownUpdate;
}

void GpsPositioner::UnpackRMC()
{
  char* nmst = (char*)gpstr;
  str0::str0_t* chunk = nmst;
  bool rmcvalid = (*(str0::ToSym(nmst, P_RMC_VALID)) == 'A') ? (true) : (false);
  posdat_->sensor = kNotValidPosition;

  if (!rmcvalid)
    return;

  posdat_->sensor = kValidPostion;
  chunk = str0::ToSym(nmst, P_RMC_DATE);
  posdat_->pos->clnd.day = str0::Atoi(chunk, 2);
  posdat_->pos->clnd.month = str0::Atoi(chunk + 2, 2);
  posdat_->pos->clnd.year = str0::Atoi(chunk + 4, 2);
  chunk = str0::ToSym(nmst, P_RMC_TIME);
  posdat_->pos->clnd.hr = str0::Atoi(chunk, 2);
  posdat_->pos->clnd.min = str0::Atoi(chunk + 2, 2);
  posdat_->pos->clnd.sec = str0::Atoi(chunk + 4, 2);

  if (str0::LenToSym(str0::ToSym(nmst, P_RMC_LAT)) == 8 &&
      (str0::LenToSym(str0::ToSym(nmst, P_RMC_TIME)) == 10))
  {
    posdat_->pos->lafull = SetL(str0::ToSym(nmst, P_RMC_LAT), 2);

    if (*str0::ToSym(nmst, P_RMC_LATNS) != 'N')
      posdat_->pos->lafull *= -1;

    posdat_->pos->lofull = SetL(str0::ToSym(nmst, P_RMC_LONG), 3);

    if (*str0::ToSym(nmst, P_RMC_LONGEW) != 'E')
      posdat_->pos->lofull *= -1;

    posdat_->update = kPoint;
  }
  else
  {
    posdat_->error = kRmcParseError;
  }
}


void GpsPositioner::UnpackVTG()
{
  char* nmst = (char*)gpstr;
  /* get ground speed in 10 meters/h (xxx.xx kmh * 100) */
  posdat_->pos->spd = str0::Atod((str0::ToSym(nmst, P_VTG_SPD)), 3, 2);
  posdat_->sup.kurs = str0::Atod((str0::ToSym(nmst, P_VTG_COERSE)), 3, 2);
  posdat_->update = kSpeed;
}


void GpsPositioner::UnpackGGA()
{
  str0::str0_t* nmst = (str0::str0_t*)gpstr;
  nmst = str0::ToSym(nmst, P_GGA_ALT);

  if (*nmst == ',')
    return;

  if (*nmst == '-')
  {
    posdat_->pos->altitude = str0::Atod(nmst + 1, 4, 1);
    posdat_->pos->altitude *= -1;
  }
  else
    posdat_->pos->altitude = str0::Atod(nmst, 4, 1);

  posdat_->update = kPoint;
}
