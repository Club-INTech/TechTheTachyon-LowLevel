/**
*
*Classe gerant la communication avec le Haut Niveau(messages arrivant, sortant)
*
*ASCII:13 = CR, 10 = LF
*/

#include "EthernetInterface.h"

EthernetInterface::EthernetInterface():server{ PORT }
{
	resetCard();

	setIP();

    Serial.print("Ethernet Ready\nLocal ip: ");
    Serial.println(Ethernet.localIP());
    server.begin();
    Serial.printf("Port is %i\n", server.server_port[0]);
    delay(1000);
    client = server.available();
    if (client.connected()) {
        client.println("CONNECTED");
    }
}

void EthernetInterface::resetCard() {

    Serial.println("Resetting WIZ820io");

    pinMode(PWD, OUTPUT);
    digitalWrite(PWD, HIGH);
    delay(10);
    digitalWrite(PWD, LOW);

    pinMode(RST, OUTPUT);
    digitalWrite(RST, LOW);
    delayMicroseconds(3);
    digitalWrite(RST, HIGH);
    delay(150);

    Ethernet.init(PIN_SPI_SS);
    Ethernet.begin(mac, ip, dns, gateway, subnet);
    digitalWrite(30,LOW);
}
void EthernetInterface::setIP() {
	while(Ethernet.localIP() != ip && (ETHERNET_RW & com_options)) {
		digitalWrite(30,HIGH);
        pinMode(LED_DEBUG_ETH,OUTPUT);
        digitalWrite(LED_DEBUG_ETH,!digitalRead(LED_DEBUG_ETH)); // La LED de debug ETH clignote en cas d'erreur
        delay(200);
        Serial.print("ERR\tIP CONFIGURATON FAILED, expected was ");
        ip.printTo(Serial);
        Serial.print(" got ");
        Ethernet.localIP().printTo(Serial);
        Serial.println();
        resetCard();
    }
	digitalWrite(LED_DEBUG_ETH,LOW);
}

bool inline EthernetInterface::read_char(char & buffer)
{
    int readValue = client.read();
    buffer = (char)readValue;
    if(readValue == -1)                         // Si il y a une erreur de lecture
    {
        resetCard();
        digitalWrite(LED_DEBUG_ETH,HIGH);
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
	EthernetClient newClient = server.available();

	if(Ethernet.localIP() != ip)                // Si on détecte qu'on a plus la bonne IP
    {
	    setIP();                                // On essaye de la re-définir
    }

	int messageSize = newClient.available();
	if (messageSize) {							// Si on est connectes et il ya des choses a lire

		client=newClient;
		char readChar;
		int i = 0;
        bool status = true;


		while (i < messageSize && status) {	    // Tant qu'on a pas vidé le buffer, atteint une fin de ligne ou une erreur de lecture
			status = read_char(readChar);
			order[i] = readChar;
			i++;
		}
		return (strcmp(order, ""));
	}
	else {
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
	client.print(message);
}

void EthernetInterface::printfln(const char* message) {
	client.println(message);
}
