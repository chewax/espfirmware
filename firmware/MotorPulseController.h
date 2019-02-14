#include <Arduino.h>
#include "MotorController.h"


#ifndef MotorPulseController_h
#define MotorPulseController_h

class MotorPulseController : public MotorController
{
  public:
    MotorPulseController();
    virtual void setOn(bool notifyServer=false);
    virtual void pulse(bool notifyServer=false);
};

#endif
