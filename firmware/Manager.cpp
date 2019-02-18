/*
  board->cpp - Library for handling ESP8266 Board (Relay/DHT)
  Created by Daniel Waksman, August 17, 2018.
  Released into the public domain.
*/

#include <Arduino.h>
#include "Manager.h"
#include "Controller.h"
#include "MotorController.h"
#include "MotorPulseController.h"

#define USE_SERIAL Serial

//Constructor
Manager::Manager()
{
    USE_SERIAL.println("[SETUP] CREATING BOARD MANAGER");
}

void Manager::init(String name, String macAddress, String mode)
{
    USE_SERIAL.println("[SETUP] INITIALIZING BOARD MANAGER");

    this->macAddress = macAddress;
    this->mode = mode;


    if (mode == "motorpulse")
    {
        board = new MotorPulseController();
    }
    else if ( mode == "motor")
    {
        board = new MotorController();
    }
    else
    {
        board = new Controller();   
    }

    board->init(name, macAddress);
}

//Handles action that was parsed from the socket message
void Manager::handleAction(String action)
{
    if (action == "setOn") board->setOn();
    if (action == "setOff") board->setOff();
}

void Manager::loop ()
{   
    board->loop();
}

void Manager::setIOHandler(SocketIO* io)
{
    this->io = io;
    board->setIOHandler(io);
}