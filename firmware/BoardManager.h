/*
  Board.h - Library for handling ESP8266 Board (Relay/DHT)
  Created by Daniel Waksman, August 17, 2018.
  Released into the public domain.
*/

#include <Arduino.h>
#include "BoardController.h"
#include "SocketIO.h"

#ifndef BoardManager_h
#define BoardManager_h

class BoardManager
{
  public:
    BoardManager();
    void init(String name, String macAddress, String mode);
    void handleAction(String action);
    void loop();
    BoardController* board;
    String mode;
    void setIOHandler(SocketIO* io);

  protected:
    String macAddress;
    SocketIO* io;
};

#endif
