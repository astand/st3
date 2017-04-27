#include "a-sms-chain-handler.h"

class SmsDelegate : public ASmsChainHandler {
 public:
  typedef int32_t (*hand_t)(const SmsChunkDescriptor& smsdsc, char* ans);

 public:
  SmsDelegate() {}
  virtual int32_t HandleIt(const SmsChunkDescriptor& smsdsc, char* ans) {
    int32_t ret = handler_func(smsdsc, ans);

    if (ret == 0) {
      if (next_ != 0)
        return next_->HandleIt(smsdsc, ans);
    }

    return ret;
  }

  void SetProcessor(hand_t handler) {
    handler_func = handler;
  }
 protected:

 private:
  hand_t handler_func;
};

