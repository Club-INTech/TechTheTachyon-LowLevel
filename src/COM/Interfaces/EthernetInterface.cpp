/**
*
*Classe gerant la communication avec le Haut Niveau(messages arrivant, sortant)
*
*ASCII:13 = CR, 10 = LF
*/

#include "EthernetInterface.h"

constexpr uint8_t bufferSize = 16;

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
//    W5100Class::SSIZE = bufferSize*1024;
//    W5100Class::SMASK = W5100Class::SSIZE-1;
	resetCard();
    interfaceInstance = this;

	setIP();

    Serial.print("Ethernet Ready\nLocal ip: ");
    Serial.println(Ethernet.localIP());

//    attachInterrupt(INT, testInterrupt, CHANGE);
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
    Serial.println("Wiz has been reset");

    W5100.init();
    wizMemoryDump();
    Ethernet.begin(mac, ip, dns, gateway, subnet);

    Serial.printf("Wiz is %i\n", (int)Ethernet.hardwareStatus());
    Serial.printf("MR is %i\n", W5100.readMR());

    // Use No Delayed ACK (cf http://wizwiki.net/wiki/lib/exe/fetch.php?media=products:w5500:w5500_ds_v108e.pdf - 4.2 Socket Registers)
    uint8_t mr = W5100.readSnMR(0);
    W5100.writeSnMR(0, static_cast<uint8_t>(mr | (1 << 5)));

//    W5100Class::SSIZE = 16*1024;
//    W5100Class::SMASK = W5100Class::SSIZE-1;
    for(int i = 0;i<MAX_SOCK_NUM;i++) {
        W5100.writeSnRX_SIZE(i, 0);
        W5100.writeSnTX_SIZE(i, 0);
    }
    W5100.writeSnRX_SIZE(0, bufferSize);
    W5100.writeSnTX_SIZE(0, bufferSize);

    // don't force ARP
    W5100.writeMR(W5100.readMR() & ~(1 << 1));

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
    Serial.printf("Available: %i\n", client.availableForWrite());
    Serial.printf("TX WritPtr: %i ; TX ReadPtr: %i\n", W5100.readSnTX_WR(0), W5100.readSnTX_RD(0));
    Serial.printf("SSIZE: %i ; SMASK: %i\n", W5100Class::SSIZE, W5100Class::SMASK);
    Serial.printf("Free Space TX: %i, Free Space RX: %i\n", W5100.readSnTX_FSR(0), W5100.readSnRX_RSR(0));
    wizMemoryDump();
    while(Ethernet.localIP() != ip && (ETHERNET_RW & com_options)) {
        Serial.print("ERR\tIP CONFIGURATON FAILED, expected was ");
        ip.printTo(Serial);
        Serial.print(" got ");
        Ethernet.localIP().printTo(Serial);
        Serial.println();
//        resetCard();
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

        wizMemoryDump();
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
    client.write((const uint8_t *)message, strlen(message));
    //client.print(message);
    flushRoutine();
}

void EthernetInterface::printfln(const char* message) {
	reconnectIfNeeded();
	sentCount++;
	char formatted[128];
	sprintf(formatted, "%s\n", message);
    client.write((const uint8_t *)formatted, strlen(formatted));
//    client.println(message);
    flushRoutine();
}

void EthernetInterface::reconnectIfNeeded() {
    //Serial.printf("Client status: %i\n", client.status());
    if( ! client.connected()) {

        if(Ethernet.localIP() != ip)                // Si on détecte qu'on a plus la bonne IP
        {
            setIP();                                // On essaye de la re-définir
        }

        Serial.println("Retry ethernet connection\n");
        while(!connect({192,168,1,2},13500)) {
            Serial.println("Retry ethernet connection...\n");
            delay(10);
        }
    }
}

void EthernetInterface::flushRoutine() {
    client.flush();
    Serial.printf("Available: %i\n", client.availableForWrite());
    Serial.printf("TX WritPtr: %i ; TX ReadPtr: %i\n", W5100.readSnTX_WR(0), W5100.readSnTX_RD(0));
    Serial.printf("SSIZE: %i ; SMASK: %i\n", W5100Class::SSIZE, W5100Class::SMASK);
    Serial.printf("Free Space TX: %i, Free Space RX: %i\n", W5100.readSnTX_FSR(0), W5100.readSnRX_RSR(0));
}

void EthernetInterface::wizMemoryDump() {
    Serial.printf("=== MEMORY DUMP ===\n");
    Serial.printf("MR: %i\n", W5100.readMR());
    uint8_t gar[4];
    uint8_t subr[4];
    uint8_t shar[6];
    uint8_t sipr[4];
    uint8_t upir[4];
    W5100.readGAR(gar);
    W5100.readSUBR(subr);
    W5100.readSHAR(shar);
    W5100.readSIPR(sipr);
    W5100.readUIPR(upir);
    Serial.printf("GAR: %i.%i.%i.%i\n", gar[0], gar[1], gar[2], gar[3]);
    Serial.printf("SUBR: %i.%i.%i.%i\n", subr[0], subr[1], subr[2], subr[3]);
    Serial.printf("SHAR: %i.%i.%i.%i.%i.%i\n", shar[0], shar[1], shar[2], shar[3], shar[4], shar[5]);
    Serial.printf("SIPR: %i.%i.%i.%i\n", subr[0], subr[1], subr[2], subr[3]);
    Serial.printf("IR: %i\n", W5100.readIR());
    Serial.printf("IMR: %i\n", W5100.readIMR());
    Serial.printf("RTR: %i\n", W5100.readRTR());
    Serial.printf("RCR: %i\n", W5100.readRCR());
    Serial.printf("PTIMER: %i\n", W5100.readPTIMER());
    Serial.printf("PMAGIC: %i\n", W5100.readPMAGIC());
    Serial.printf("UPIR: %i.%i.%i.%i\n", upir[0], upir[1], upir[2], upir[3]);
    Serial.printf("UPORT: %i\n", W5100.readUPORT());
    Serial.printf("PHYCFGR: %i\n", W5100.readPHYCFGR_W5500());
    Serial.printf("VERSIONR: %i\n", W5100.readVERSIONR_W5500());

    for (int i = 0; i < 8; ++i) {
        Serial.printf("--- SOCKET #%i ---\n", i);
        Serial.printf("Sn_MR: %i\n", W5100.readSnMR(i));
        Serial.printf("Sn_CR: %i\n", W5100.readSnCR(i));
        Serial.printf("Sn_IR: %i\n", W5100.readSnIR(i));
        Serial.printf("Sn_SR: %i\n", W5100.readSnSR(i));
        Serial.printf("Sn_PORT: %i\n", W5100.readSnPORT(i));

        uint8_t dhar[6];
        uint8_t dipr[4];
        W5100.readSnDHAR(i, dhar);
        W5100.readSnDIPR(i, dipr);
        Serial.printf("Sn_DHAR: %i.%i.%i.%i.%i.%i\n", dhar[0], dhar[1], dhar[2], dhar[3], dhar[4], dhar[5]);
        Serial.printf("Sn_DIPR: %i.%i.%i.%i\n", dipr[0], dipr[1], dipr[2], dipr[3]);
        Serial.printf("Sn_DPORT: %i\n", W5100.readSnPORT(i));
        Serial.printf("Sn_MSSR: %i\n", W5100.readSnMSSR(i));
        Serial.printf("Sn_TOS: %i\n", W5100.readSnTOS(i));
        Serial.printf("Sn_TTL: %i\n", W5100.readSnTTL(i));
        Serial.printf("Sn_RXBUF_SIZE: %i\n", W5100.readSnRX_SIZE(i));
        Serial.printf("Sn_TXBUF_SIZE: %i\n", W5100.readSnTX_SIZE(i));
        Serial.printf("Sn_TX_FSR: %i\n", W5100.readSnTX_FSR(i));
        Serial.printf("Sn_RX_RD: %i\n", W5100.readSnRX_RD(i));
        Serial.printf("Sn_TX_RD: %i\n", W5100.readSnTX_RD(i));
        Serial.printf("Sn_RX_WR: %i\n", W5100.readSnRX_WR(i));
        Serial.printf("Sn_TX_WR: %i\n", W5100.readSnTX_WR(i));
        Serial.printf("Sn_RX_RSR: %i\n", W5100.readSnRX_RSR(i));
    }
    Serial.printf("============\n");
}
