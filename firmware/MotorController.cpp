#include <Arduino.h>
#include "MotorController.h"
#include "Utils.h"



MotorController::MotorController() : Controller()
{
}

void MotorController::init(String name, String id)
{

    Controller::init(name, id);
}


//Performs OFF action
void MotorController::sense()
{
    Controller::sense();

    String result;

    result += "{\"";
    result += "state";
    result += "\":\"";
    result += lastKnownState;
    result += "\"}";

    if (io)
        io->sendJSON("board:sense", result);
}


// Reads sensor information
// Althought it may have 2 sensors, sensed output is always a state calculated from both readings...so for the outer world it has 1 sensor.
void MotorController::computeSensorData()
{
    int hallState = digitalRead(sensorPin);

    String currentState;

    if (hallState)
        currentState = "closed";
    else
        currentState = "unknown";

    if (lastKnownState != currentState)
        onStateChange(currentState);

    lastKnownState = currentState;
}

void MotorController::onStateChange(String newState)
{
    if (newState == "closed")
        //Cuando cierra desestimulo el relay
        setOff(true);

    if (newState == "unknown")
        //Set delay to turn off
        timer.setTimeout(AUTOOFF_DELAY, [this]() {
            this->setOff(true);
        });

    String result;
    
    result += "{\"";
    result += "state";
    result += "\":\"";
    result += newState;
    result += "\"}";

    if (io)
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
