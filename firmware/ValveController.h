#include <Arduino.h>
#include "Controller.h"


#ifndef ValveController_h
#define ValveController_h

class ValveController : public Controller
{
  public:
    ValveController();
    virtual void init(String name, String id);
    virtual void handleInput(uint32_t val);
    virtual void setOn(bool notifyServer=false);
    virtual void setOff(bool notifyServer=false);

  protected:
    String lastKnownState = "unknown";
    uint32_t lastInputVal = HIGH;
};

#endif
