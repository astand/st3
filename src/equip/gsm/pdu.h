#pragma once

#include <stdint.h>

const char* GetPtrToPhone();
int32_t FromPduToAscii(char* ascii_text, const char* pdu_text, int32_t lenght);
int32_t FromAsciiToPdu(char* pdu_text, const char* ascii_text, int32_t lenght, uint8_t options);

