#pragma once

#include <stdint.h>

#define MAX_LENGTH      (256)

#define SERVER_LENGTH   (64)
#define PORT_LENGTH     (10)
#define APN_LENGTH      (48)
#define USER_LENGTH     (32)
#define PSWRD_LENGTH    (32)

#define SUM_LENGTH      (SERVER_LENGTH + PORT_LENGTH + APN_LENGTH + USER_LENGTH + PSWRD_LENGTH)

#define RESERVED_LENGTH (MAX_LENGTH - SUM_LENGTH)

typedef enum
{
  kServer = 0,
  kPort = 1,
  kAPN = 2,
  kUser = 3,
  kPswrd = 4,
  kParamMaxPosition
} ParamPosition;

class ServParam {
 public:
  ServParam() {}
  int32_t UpdateParametr(ParamPosition pos, const char* param, int32_t len = 0);
  int32_t PrintParametr(ParamPosition pos, char* dst) const;
  const char* GetParametr(ParamPosition pos) const;
  void ResetAll();

 protected:
  /// server may be presented as IP or as web address
  char server_[SERVER_LENGTH];
  char port_[PORT_LENGTH];
  char apn_[APN_LENGTH];
  char user_[USER_LENGTH];
  char pswrd_[PSWRD_LENGTH];
  char reserved[RESERVED_LENGTH];

 private:
  void RefreshOutsidePointers(char* pointers[]) const;
  int32_t ReWriteParam(char* dst, const char* src, int32_t max_len);
};

