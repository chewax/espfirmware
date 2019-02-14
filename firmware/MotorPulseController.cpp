#include <Arduino.h>
#include "MotorPulseController.h"

#define USE_SERIAL Serial

MotorPulseController::MotorPulseController() : MotorController()
{
}

//Performs ON action
void MotorPulseController::setOn(bool notifyServer /* =false */)
{
    this->pulse(notifyServer);
}

void MotorPulseController::pulse(bool notifyServer /* =false */)
{
    MotorController::setOn(notifyServer);

    timer.setTimeout(AUTOOFF_DELAY, [this]() {
        this->setOff(true);
    });
}
