void connectToWifi()
{
    WiFi.softAPdisconnect(true);
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(config.ssid, config.password);

    USE_SERIAL.print("[WIFI] Connecting to: ");
    USE_SERIAL.print(config.ssid);

    while (WiFi.status() != WL_CONNECTED)
    { // Wait for the Wi-Fi to connect
        delay(1000);
        USE_SERIAL.print('.');
    }

    USE_SERIAL.println(".");
    USE_SERIAL.println("[WIFI] Connection established!");
    USE_SERIAL.print("[WIFI] IP address:\t");
    USE_SERIAL.println(WiFi.localIP());
}

//InitializesWiFi
void initializeWiFi()
{

    USE_SERIAL.println("...");
    // connect = strlen(ssid) > 0; // Request WLAN connect if there is a SSID
    if (strlen(config.ssid) > 0)
    {
        USE_SERIAL.println("[WIFI] INITIALIZING STATION...");
        connectToWifi();
    }
    else
    {
        USE_SERIAL.println("[WIFI] <NO CONFIG> STARTING AP...");
        startAP();
    }
}

void startAP()
{
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_AP);
    WiFi.hostname("espconfig");

    if (config.name)
    {
        strcat(softAP_ssid, config.name);
    }
    else
    {
        strcat(softAP_ssid, WiFi.macAddress().c_str());
    }

    WiFi.softAPConfig(apIP, apIP, netMsk);
    WiFi.softAP(softAP_ssid, softAP_password);

    delay(500); // Without delay I've seen the IP address blank

    USE_SERIAL.println("[WIFI] SoftAP Configured.");
    USE_SERIAL.print("SSID: ");
    USE_SERIAL.println(softAP_ssid);
    USE_SERIAL.print("IP address: ");
    USE_SERIAL.println(WiFi.softAPIP());

    // if DNSServer is started with "*" for domain name, it will reply with
    // provided IP to all DNS request
    dnsServer.start(DNS_PORT, "*", apIP);
}
