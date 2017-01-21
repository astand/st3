#pragma once
#include "trekrep/ITrekList.h"

ITrekList* FileLink();
void JConfUpload(uint8_t* mem, uint32_t len);
void JConfSave(uint8_t* mem, uint32_t len);

void Start_GpsThread();
