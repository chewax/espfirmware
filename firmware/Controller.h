/*
  Board.h - Library for handling ESP8266 Board (Relay/DHT)
  Created by Daniel Waksman, August 17, 2018.
  Released into the public domain.
*/

#include <Arduino.h>
#include "SocketIO.h"
#include "SimpleTimer.h"

#ifndef Controller_h
#define Controller_h

class Controller
{
  public:
    Controller();

    String id;
    String name;

    virtual void init(String name, String id);
    virtual void defaultAction(bool notifyServer=false);
    virtual void setOn(bool notifyServer=false);
    virtual void setOff(bool notifyServer=false);
    virtual void sense();
    virtual void handleInput(uint32_t val);
    virtual void loop();

    void toggle();
    void toggle(bool notifyServer=false);
    void setIOHandler(SocketIO* io);
    uint32_t relayState;


  protected:
    uint32_t relayPin;
    uint32_t sensorPin;
    uint32_t inputPin;
    uint32_t switchState;
    SocketIO* io;
    uint64_t loopTimestamp = 0;
    uint64_t pollTimestamp = 0;
    uint32_t LOOP_INTERVAL = 50;
    uint32_t POLL_INTERVAL = 5000;
    bool poll_available = false;
    bool initialized = false;
    SimpleTimer timer;
};

#endif
