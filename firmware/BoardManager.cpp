/*
  board->cpp - Library for handling ESP8266 Board (Relay/DHT)
  Created by Daniel Waksman, August 17, 2018.
  Released into the public domain.
*/

#include <Arduino.h>
#include "BoardManager.h"
#include "BoardController.h"
#include "MotorController.h"

#define USE_SERIAL Serial

//Constructor
BoardManager::BoardManager()
{
    USE_SERIAL.println("[SETUP] CREATING BOARD MANAGER");
}

void BoardManager::init(String name, String macAddress, String mode)
{
    USE_SERIAL.println("[SETUP] INITIALIZING BOARD MANAGER");

    this->macAddress = macAddress;
    this->mode = mode;


    if (mode == "motorpulse")
    {
        board = new MotorController();
    }
    else if ( mode == "switch")
    {
        /* code */
    }
    else
    {
        board = new BoardController();
        
    }

    board->init(name, macAddress);
}

//Handles action that was parsed from the socket message
void BoardManager::handleAction(String action)
{
    if (action == "setOn") board->setOn();
    if (action == "setOff") board->setOff();
}

void BoardManager::loop ()
{   
    board->loop();
}

void BoardManager::setIOHandler(SocketIO* io)
{
    this->io = io;
    board->setIOHandler(io);
}