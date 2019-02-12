#include <Arduino.h>
#include "MotorController.h"

#define USE_SERIAL Serial

MotorController::MotorController() : BoardController()
{
}

void MotorController::init(String name, String id)
{
    BoardController::init(name, id);

    // attachInterrupt(digitalPinToInterrupt(sensorPin_A), magnet_detect_sensorA, RISING);//Initialize the intterrupt pin (Arduino digital pin 3)
    // attachInterrupt(digitalPinToInterrupt(sensorPin_B), magnet_detect_sensorB, RISING);//Initialize the intterrupt pin (Arduino digital pin 1)

    // using fn = void(*)();
    // fn doorOpenedCB = []() -> void { doorStateOpened(); };
    // fn doorClosedCB = []() -> void { doorStateClosed(); };
    // fn doorUnknownCB = []() -> void { doorStateUnknown(); };

    // attachInterrupt(digitalPinToInterrupt(sensorPin_A), doorOpenedCB, RISING);
    // attachInterrupt(digitalPinToInterrupt(sensorPin_B), doorClosedCB, RISING);
    // attachInterrupt(digitalPinToInterrupt(sensorPin_A), doorUnknownCB, FALLING);
    // attachInterrupt(digitalPinToInterrupt(sensorPin_B), doorUnknownCB, FALLING);
}

//Performs default action
void MotorController::defaultAction(bool notifyServer /* =false */)
{
    pulse(notifyServer);
}

//Performs ON action
void MotorController::setOn(bool notifyServer /* =false */)
{
    BoardController::setOn(notifyServer);
}

//Performs OFF action
void MotorController::setOff(bool notifyServer /* =false */)
{
    BoardController::setOff(notifyServer);
}


//Performs OFF action
void MotorController::sense()
{
    BoardController::sense();

    String result;

    result += "{\"";
    result += "state";
    result += "\":\"";
    result += lastKnownState;
    result += "\"}";

    io->sendJSON("board:sense", result);
}


//Reads sensor information
//Althought it may have 2 sensors, sensed output is always a state calculated from both readings...so for the outer world it has 1 sensor.
void MotorController::computeSensorData()
{
    int sensorA = digitalRead(sensorPin_A);
    int sensorB = digitalRead(sensorPin_B);;

    String currentState;

    if (sensorA && !sensorB)
        currentState = "opened";

    else if (sensorB && !sensorA)
        currentState = "closed";

    else
        currentState = "unknown";

    if (lastKnownState != currentState)
        onStateChange(currentState);

    lastKnownState = currentState;
}

void MotorController::onStateChange(String newState)
{
    if (newState == "opened" || newState == "closed")
        //Cuando llega a los topes desestimulo el relay
        setOff(true);

    String result;
    
    result += "{\"";
    result += "state";
    result += "\":\"";
    result += newState;
    result += "\"}";

    io->sendJSON("board:sense", result);
}

void MotorController::handleInput(uint32_t val)
{

    if (val == LOW && lastInputVal == HIGH) //If button set to GND. Toggle.
    {
        lastInputVal = LOW;
        toggle(true);
    }
    else if (val == HIGH && lastInputVal == LOW)
    {
        lastInputVal = HIGH;
    }

    computeSensorData();
}

void MotorController::pulse(bool notifyServer /* =false */)
{
    setOn(true);
    delay(500);
    setOff(true);
}