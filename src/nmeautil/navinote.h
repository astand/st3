#pragma once

#include <stdint.h>
#include "utility/etc/str0.h"
#include "clnd.hpp"
#include "speedsnaper.hpp"
#include "utility/abstract/IFlashStorable.h"

// this type returns by UpdatePosition function for determination
// what kind of data was updated
typedef enum
{
  kNoUpdate = 0,
  kPoint = 1 << 0,
  kSpeed = 1 << 1,
  kUnknownUpdate = 1 << 15,
  kValidUpdateMask = 0x000f
} GpsUpdate_t;

typedef enum
{
  kNoError = 0,
  kCsmError,
  kRmcParseError
} GpsUpdateError_t;

typedef enum
{
  kNoSensor = 0,
  kNotValidPosition,
  kValidPostion
} PositionerState_t;

typedef struct
{
  uint16_t kurs;
} GpsSupportData_t;

/* ------------------------------------------------------------------------- *
** single note (waypiont) of full trek file
** including latitude and long titude date time etc
** current Lenght = 24. Spare will use for ADC1 input soon
** next will deploy two more fields for other ADC2-ADC3 inputs
** ------------------------------------------------------------------------- */
__packed class NaviNote {
 public:
  clnd0 clnd;
  int16_t altitude;

  int32_t lafull;
  int32_t lofull;
  uint16_t spd;
  uint32_t accum_dist;
  uint16_t adcs[3];
};

const uint32_t kNaviNoteLength = sizeof(NaviNote);

class NaviWriter : public IFlashStorable {
 public:
  NaviWriter(NaviNote* note) : Note(note) {}
 public:
  virtual const uint8_t* DataPointer() {
    return (uint8_t*)(Note);
  }
  virtual uint32_t DataLenght() {
    return kNaviNoteLength;
  }

 private:
  NaviNote* const Note;
};

typedef struct
{
  NaviNote* pos;
  GpsSupportData_t sup;
  GpsUpdate_t update;
  GpsUpdateError_t error;
  PositionerState_t sensor;
} GpsPositionData_t;
