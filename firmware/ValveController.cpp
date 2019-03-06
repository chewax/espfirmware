#include <Arduino.h>
#include "ValveController.h"
#include "Utils.h"



ValveController::ValveController() : Controller()
{

}

//Performs ON action ON RELAY B
void ValveController::setOn(bool notifyServer /* =false */)
{
    digitalWrite(relayB, HIGH);
    relayState = HIGH;

    if (notifyServer && io)
    {
        io->sendMessage("board:setOn");
    }
}

//Performs OFF action ON RELAY B
void ValveController::setOff(bool notifyServer /* =false */)
{
    digitalWrite(relayB, LOW);
    relayState = LOW;

    if (notifyServer && io)
    {
        io->sendMessage("board:setOff");
    }
}


void ValveController::init(String name, String id)
{
    Controller::init(name, id);
}


void ValveController::handleInput(uint32_t val)
{

    if (val == LOW && lastInputVal == HIGH)
    {
        setOff(true);
        lastInputVal = LOW;
    }
    else if (val == HIGH && lastInputVal == LOW)
    {
        setOn(true);
        lastInputVal = HIGH;
    }

}

