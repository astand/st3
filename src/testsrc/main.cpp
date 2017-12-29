#include <stdint.h>

#include "nmeautil/nmea-test-generator.h"
#include "nmeautil/nmea-string-reader.h"
#include "nmeautil/gps-positioner.h"


SinteticStream strm;
NmeaStringReader reader(strm);
GpsPositioner gpspos(reader);
NaviNote note;
GpsPositionData_t data;

uint8_t buff[128] = {0};
int main()
{
  data.pos = &note;

  while (1)
  {
    gpspos.UpdatePosition(&data);

    if (data.update != kNoUpdate)
    {
      __nop();
    }
  }
}
