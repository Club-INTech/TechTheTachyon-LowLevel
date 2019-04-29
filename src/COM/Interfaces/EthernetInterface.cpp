/**
*
*Classe gerant la communication avec le Haut Niveau(messages arrivant, sortant)
*
*ASCII:13 = CR, 10 = LF
*/

#include "EthernetInterface.h"
#include <utility/w5100.h>

void testInterrupt() {
    if(interfaceInstance) {
        char formatted[128];
        sprintf(formatted, "INT Wiz %i", W5100.readSnIR(interfaceInstance->client.getSocketNumber()));
        InterruptStackPrint::Instance().push(DEBUG_HEADER, formatted);
        // clear
        W5100.writeSnIR(interfaceInstance->client.getSocketNumber(), 0);
    }
}

EthernetInterface::EthernetInterface()
{
	resetCard();
    interfaceInstance = this;

	setIP();

    Serial.print("Ethernet Ready\nLocal ip: ");
    Serial.println(Ethernet.localIP());

    attachInterrupt(INT, testInterrupt, CHANGE);
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


    // Use No Delayed ACK (cf http://wizwiki.net/wiki/lib/exe/fetch.php?media=products:w5500:w5500_ds_v108e.pdf - 4.2 Socket Registers)
    uint8_t mr = W5100.readSnMR(0);
    W5100.writeSnMR(0, static_cast<uint8_t>(mr | (1 << 5)));

    for(int i = 0;i<MAX_SOCK_NUM;i++) {
        W5100.writeSnRX_SIZE(i, 0);
        W5100.writeSnTX_SIZE(i, 0);
    }
    W5100.writeSnRX_SIZE(0, 4);
    W5100.writeSnTX_SIZE(0, 4);

    // set pointeurs de RX et TX au début du bloc sur la socket #0
/*    W5100.writeSnRX_WR(0, 0);
    W5100.writeSnRX_RD(0, 0);
    W5100.writeSnTX_RD(0, 0);
    W5100.writeSnTX_WR(0, 0);
*/
    // clear buffers RX et TX de la socket #0
    // TODO

    Ethernet.setLocalIP(ip);
    Ethernet.setRetransmissionTimeout(50);
    Ethernet.setRetransmissionCount(2);

    while(!connect({192,168,1,2},13500))
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
        Serial.printf("CONNECTED ON SOCKET #%i TO : ", client.getSocketNumber());
        Serial.print(client.remoteIP());
        Serial.print(":");
        Serial.println(client.remotePort());
        Serial.print("Current ip is ");
        Ethernet.localIP().printTo(Serial);
        Serial.println();
        Serial.printf("Sent count is %i\n", sentCount);
    }
    else
    {
        Serial.print("CONNECTION FAILED TO : ");
        Serial.print(client.remoteIP());
        Serial.print(":");
        Serial.println(client.remotePort());
        Serial.print("Current ip is ");
        Ethernet.localIP().printTo(Serial);
        Serial.println();
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

    reconnectIfNeeded();

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
    reconnectIfNeeded();
    sentCount++;
    client.print(message);
    flushRoutine();
}

void EthernetInterface::printfln(const char* message) {
	reconnectIfNeeded();
	sentCount++;
    client.println(message);
    flushRoutine();
}

void EthernetInterface::reconnectIfNeeded() {
    //Serial.printf("Client status: %i\n", client.status());
    if( ! client.connected()) {
        Serial.println("Retry ethernet connection\n");
        while(!connect({192,168,1,2},13500)) {
            Serial.println("Retry ethernet connection...\n");
            delay(10);
        }
    }
}

void EthernetInterface::flushRoutine() {
    client.flush();
    Serial.printf("Available: %i", client.availableForWrite());
}
