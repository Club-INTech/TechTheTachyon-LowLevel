/**
*
*Classe gerant la communication avec le Haut Niveau(messages arrivant, sortant)
*
*ASCII:13 = CR, 10 = LF
*/

#include "EthernetInterface.h"

EthernetInterface::EthernetInterface()
{
	resetCard();

	setIP();

    Serial.print("Ethernet Ready\nLocal ip: ");
    Serial.println(Ethernet.localIP());
}

void EthernetInterface::resetCard() {

    Serial.println("Resetting WIZ850io");

    /**
     * Depuis https://cdn.sos.sk/productdata/43/24/8adad58a/wiz850io.pdf (datasheet)
     * This pin is to initialize WIZ850io.
     * Hold  at  least  500us  after  asserted  to  LOW
     * and keep HIGH until next Reset needed.User need
     * to wait for 50ms after this pin is changed to HIGH
     * to communicate with WIZ850io.
     */
    pinMode(RST, OUTPUT);
    digitalWrite(RST, LOW);
    delay(100);
    //delayMicroseconds(500);
    digitalWrite(RST, HIGH);
    delay(500);

    Ethernet.init(CS);
    Ethernet.begin(mac, ip, dns, gateway, subnet);
    Ethernet.setLocalIP(ip);

    while(!connect({192,168,0,2},13500))
    {
        delay(10);
        Serial.println("Retry ethernet connection ...");
    }
}
void EthernetInterface::setIP() {
	while(Ethernet.localIP() != ip && (ETHERNET_RW & com_options)) {
        Serial.print("ERR\tIP CONFIGURATON FAILED, expected was ");
        ip.printTo(Serial);
        Serial.print(" got ");
        Ethernet.localIP().printTo(Serial);
        Serial.println();
        resetCard();
    }
}

bool EthernetInterface::connect(IPAddress ip, int port)
{
    bool ret = client.connect(ip,port);
    if(ret)
    {
        Serial.print("CONNECTED TO : ");
        Serial.print(client.remoteIP());
        Serial.print(":");
        Serial.println(client.remotePort());
    }
    else
    {
        Serial.print("CONNECTION FAILED TO : ");
        Serial.print(client.remoteIP());
        Serial.print(":");
        Serial.println(client.remotePort());
    }
    return ret;
}

bool EthernetInterface::connected()
{
    return client.connected();
}

bool inline EthernetInterface::read_char(char & buffer)
{
    int readValue = client.read();
    buffer = (char)readValue;
    if(readValue == -1)                         // Si il y a une erreur de lecture
    {
        //resetCard();
        return(false);
    }
    else if(buffer == '\r' || buffer == '\n')   // On remplace les caractères de fin de ligne par des caractères nuls
    {
        buffer = '\0';
        return(false);
    }
	return(true);
}

inline bool EthernetInterface::read(char* order)
{

    if(Ethernet.localIP() != ip)                // Si on détecte qu'on a plus la bonne IP
    {
        setIP();                                // On essaye de la re-définir
    }
/*
    if( !client.connected() )
    {
        while(!connect({192,168,0,2},13500))
        {
            delay(10);
            Serial.println("Retry ethernet connection ...");
        }
    }
    */
    int length = client.available();
    if( length > 0 )
    {
        int i = 0;
        bool status = true;
        char tmp = 0x00;
        while( i < length && status )
        {
            status = read_char(tmp);
            order[i] = tmp;
            i++;
        }
        return strcmp(order,"");
    }
    else
    {
        return false;
    }
}

bool EthernetInterface::read(int32_t & value)
{
	char readValue[16];

	bool status = read(readValue);

	value = strtol(readValue, nullptr, DEC);

	return status;
}

bool EthernetInterface::read(int16_t & value)
{
	char readValue[16];

	bool status = read(readValue);

	value = strtol(readValue, nullptr, DEC);

	return status;
}

bool EthernetInterface::read(volatile int8_t & value)
{
	char readValue[16] = "";

	bool status = read(readValue);

	value = strtol(readValue, nullptr, DEC);

	return status;
}

bool EthernetInterface::read(float& value) {
	char readValue[16] = "";

	bool status = read(readValue);

	value = strtof(readValue, nullptr);

	return status;
}

void EthernetInterface::printf(const char *message) {
    if(client.connected())
    {
        client.print(message);
    }
}

void EthernetInterface::printfln(const char* message) {
	if(client.connected())
    {
	    client.println(message);
    }
}
