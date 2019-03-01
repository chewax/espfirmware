#include <Arduino.h>
#include "Controller.h"
#include "EmonLib.h"


#ifndef CurrentController_h
#define CurrentController_h

class CurrentController : public Controller
{
  public:
    CurrentController();
    virtual void init(String name, String id);
    virtual void handleInput(uint32_t val);
    virtual void loop();

  protected:
    EnergyMonitor emon1;
    void computeSensorData();
    void IRMS(int numberOfSamples);

    uint32_t lastInputVal = HIGH;
    uint64_t sampleTimestamp = 0;
    uint32_t SAMPLE_INTERVAL = 5000;

    float VCC = 3.3f; //supply voltage.
    float VPC = 0; //volts per count
    float ACVoltage = 230; //line voltage
    float sensitivity = 0.066f; //model sensitivity
    int alignment = 9999; //Means not configured

    long sampleSum = 0;
    uint32_t SAMPLING = 1480;
};

#endif
