#include <Arduino.h>
#include "BoardController.h"

#ifndef MotorController_h
#define MotorController_h

class MotorController : public BoardController
{
  public:
    MotorController();
    virtual void init(String name, String id);
    virtual void defaultAction(bool notifyServer=false);
    virtual void setOn(bool notifyServer=false);
    virtual void setOff(bool notifyServer=false);
    virtual void sense();
    virtual void handleInput(uint32_t val);

  protected:
    uint32_t lastInputVal = HIGH;
    void onStateChange(String newState);
    String lastKnownState = "unknown";
    void pulse(bool notifyServer = false);
    void computeSensorData();
};

#endif
