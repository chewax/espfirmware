//Initializes Serial
void initializeSerial()
{
    // USE_SERIAL.begin(921600);
    USE_SERIAL.begin(115200);
    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    // for (uint8_t t = 4; t > 0; t--)
    // {
    //     USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
    //     USE_SERIAL.flush();
    //     delay(1000);
    // }

    USE_SERIAL.println('[SETUP] SERIAL INITIALIZED');
}