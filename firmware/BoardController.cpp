/*
  Board.cpp - Library for handling ESP8266 Board (Relay/DHT)
  Created by Daniel Waksman, August 17, 2018.
  Released into the public domain.
*/

#include <Arduino.h>
#include "BoardController.h"

#define USE_SERIAL Serial

//Constructor
BoardController::BoardController()
{
}

void BoardController::init(String name, String id)
{
    USE_SERIAL.println("[SETUP] INITIALIZING BOARD");

    this->id = id;
    this->name = name;
    this->switchState = LOW;
    this->relayState = LOW;
    

    switchInputPin = 2;
    sensorPin_A = 3;
    sensorPin_B = 1;
    relayPin = 0;
    loopTimestamp = 0;

    //********** CHANGE PIN FUNCTION  TO GPIO **********
    pinMode(sensorPin_A, INPUT_PULLUP);  //former RX
    pinMode(sensorPin_B, INPUT_PULLUP);  //former TX
    //**************************************************

    pinMode(relayPin, OUTPUT);
    pinMode(switchInputPin, INPUT_PULLUP);

    this->initialized = true;
    
}

//Performs default action
void BoardController::defaultAction(bool notifyServer /* =false */)
{
    toggle(notifyServer);
}

//Performs ON action
void BoardController::setOn(bool notifyServer /* =false */)
{
    digitalWrite(relayPin, HIGH);
    relayState = HIGH;

    if (notifyServer)
    {
        io->sendMessage("board:setOn");
    }
}

//Performs OFF action
void BoardController::setOff(bool notifyServer /* =false */)
{
    digitalWrite(relayPin, LOW);
    relayState = LOW;

    if (notifyServer)
    {
        io->sendMessage("board:setOff");
    }
}

//Reads sensor information
//Althought it may have 2 sensors, sensed output is always a state calculated from both readings...so for the outer world it has 1 sensor.
void BoardController::sense()
{


}

void BoardController::handleInput(uint32_t val)
{

}

void BoardController::loop()
{   

    uint64_t now = millis();

    if ( (now - loopTimestamp) > LOOP_INTERVAL )
    {   
        loopTimestamp = now;
        uint32_t val = 0;
        val = digitalRead(switchInputPin);
        handleInput(val);
    }
}

void BoardController::toggle(bool notifyServer /* =false */)
{
    relayState = !relayState; //Toggle Relay
    digitalWrite(relayPin, relayState);

    if (notifyServer)
    {
        if (relayState == HIGH)
        {
            io->sendMessage("board:setOn");
        }
        else
        {
            io->sendMessage("board:setOff");
        }
    }
}

void BoardController::setIOHandler(SocketIO *io)
{
    this->io = io;
    poll_available = true;
}