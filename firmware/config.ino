const int EEPROM_OFFSET = 0;

/** Load WLAN credentials from EEPROM */
void saveBoardConfig(ConfigStruct config)
{
    EEPROM.begin(sizeof(ConfigStruct));

    strcpy(config.configured, "OK");

    EEPROM.put(EEPROM_OFFSET, config);

    EEPROM.commit();
    EEPROM.end();
}

ConfigStruct loadBoardConfig()
{
    EEPROM.begin(sizeof(ConfigStruct));

    ConfigStruct config_read;
    EEPROM.get(EEPROM_OFFSET, config_read);
    EEPROM.end();

    if (String(config_read.configured) != String("OK"))
    {
        config_read = ConfigStruct();
    }

    return config_read;
}