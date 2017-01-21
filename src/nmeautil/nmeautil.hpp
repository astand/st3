#ifndef __NMEA_UTIL_H
#define __NMEA_UTIL_H

// #include <stdint.h>
#include <stdio.h>
// #include "str0.h"
#include "navinote.hpp"

//#define _TEMP_NAVI_LENGHT ((uint32_t)32)

//#define _TEMP_ONESEC_INCLUDE   ((uint32_t)(4096/_TEMP_NAVI_LENGHT))

//#define _TEMP_USE_SECMEM_SIZE  ((uint32_t)(_TEMP_ONESEC_INCLUDE * _TEMP_NAVI_LENGHT))

/* -------------------------------------------------------------------------- *
 Object describes trek file params: such as start/end datetime,
 file size, distance traveled (need add absolute distance in last point)
 */
__packed class Matrix_item {

 public:
  uint16_t id;
  clnd0 dat0;
  clnd0 dat1;
  uint32_t size;
  uint32_t dist;
  uint32_t mileage;

  Matrix_item();
  void StartDate(const NaviNote* obj);
  void EndDate(const NaviNote* obj);
  void setParam(uint16_t id, uint32_t rawdatasize, uint32_t localkm, uint32_t odometrkm);
  static uint16_t Lenght();
  int32_t ToString(char* outprint);


 protected:

};









#endif
