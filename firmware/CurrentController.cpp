#include <Arduino.h>
#include "CurrentController.h"
#include "Utils.h"
#include "EmonLib.h"

#define MAX_COUNTS 1024

CurrentController::CurrentController() : Controller()
{


}

void CurrentController::init(String name, String id)
{
    Controller::init(name, id);
    VPC = VCC / MAX_COUNTS;

    Utils::logger("Wait for current sensor alignment", "SETUP");
    ConfigStruct conf = Utils::loadConfig();
    Utils::logger(String("Config stored value: ") + String(conf.current_alignment), "SETUP");

    //Attempt to align. If already aligned then continue.
    if (conf.current_alignment == 9999) {

        for (unsigned int n = 0; n < SAMPLING; n++)
        {
            sampleSum += sq( analogRead(A0) );
        }

        int mean = sampleSum / SAMPLING;
        int counts = sqrt(mean);

        alignment = 512 - counts;
        conf.current_alignment = alignment;
        
        Utils::logger(String("Calculated alignment value: ") + String(alignment), "SETUP");
        Utils::logger("Updating config...", "SETUP");

        Utils::saveConfig(conf);
    }
    else
    {   
        Utils::logger("Using stored value", "SETUP");
        alignment = conf.current_alignment;
    }

    Utils::logger("Updating config...", "SETUP");
    Utils::saveConfig(conf);
    Utils::debug(String("Final alignment is ") + String(alignment));
}

//Intensity RMS
void CurrentController::IRMS(int numberOfSamples)
{
    for (unsigned int n = 0; n < numberOfSamples; n++)
    {
        sampleSum += sq( analogRead(A0) - 512 ); 
    }

    int mean = sampleSum / numberOfSamples;
    int counts = sqrt(mean);

    counts += alignment;  // +alignment

    Utils::debug(String(counts));

    float mv = counts * VPC;
    float amps = mv / sensitivity;
    float watts = amps * ACVoltage;

    String result;

    result += Utils::asProperty("type", "current", true);
    result += Utils::asProperty("method", "RMS", true);
    result += Utils::asProperty("amps", String(amps, 3), true);
    result += Utils::asProperty("watts", String(watts, 3), true);

    result = Utils::asJSONObj(result);

    if (io)
        io->sendJSON("board:sense", result);
    
    sampleSum = 0;
}

// Reads sensor information
// Althought it may have 2 sensors, sensed output is always a state calculated from both readings...so for the outer world it has 1 sensor.
void CurrentController::computeSensorData()
{
    //USING RMS Sampling
    if ( millis() - sampleTimestamp > SAMPLE_INTERVAL ) 
    {
        IRMS(SAMPLING);
        sampleTimestamp = millis();
    }
}

void CurrentController::handleInput(uint32_t val)
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

void CurrentController::loop()
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

    computeSensorData();
}
