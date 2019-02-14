/*
  Board.cpp - Library for handling ESP8266 Board (Relay/DHT)
  Created by Daniel Waksman, August 17, 2018.
  Released into the public domain.
*/

#include <Arduino.h>
#include "Controller.h"
#include "SimpleTimer.h"

#define USE_SERIAL Serial

//Constructor
Controller::Controller()
{
}

void Controller::init(String name, String id)
{
    USE_SERIAL.println("[SETUP] INITIALIZING BOARD");

    this->id = id;
    this->name = name;
    this->switchState = LOW;
    this->relayState = LOW;
    

    inputPin = 2;
    sensorPin = 3;
    relayPin = 0;
    loopTimestamp = 0;

    //********** CHANGE PIN FUNCTION  TO GPIO **********
    pinMode(sensorPin, INPUT_PULLUP);  //former RX
    //**************************************************

    pinMode(relayPin, OUTPUT);
    pinMode(inputPin, INPUT_PULLUP);

    this->initialized = true;
    
}

//Performs default action
void Controller::defaultAction(bool notifyServer /* =false */)
{
    toggle(notifyServer);
}

//Performs ON action
void Controller::setOn(bool notifyServer /* =false */)
{
    digitalWrite(relayPin, HIGH);
    relayState = HIGH;

    if (notifyServer && io)
    {
        io->sendMessage("board:setOn");
    }
}

//Performs OFF action
void Controller::setOff(bool notifyServer /* =false */)
{
    digitalWrite(relayPin, LOW);
    relayState = LOW;

    if (notifyServer && io)
    {
        io->sendMessage("board:setOff");
    }
}

//Reads sensor information
//Althought it may have 2 sensors, sensed output is always a state calculated from both readings...so for the outer world it has 1 sensor.
void Controller::sense()
{


}

void Controller::handleInput(uint32_t val)
{

}

void Controller::loop()
{   
    timer.run();
    uint64_t now = millis();

    if ( (now - loopTimestamp) > LOOP_INTERVAL )
    {   
        loopTimestamp = now;
        uint32_t val = 0;
        val = digitalRead(inputPin);
        handleInput(val);
    }
}

void Controller::toggle(bool notifyServer /* =false */)
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

void Controller::setIOHandler(SocketIO *io)
{
    this->io = io;
    poll_available = true;
}