#ifndef _SPARKFUNESP8266CLIENTBUFFER_H_
#define _SPARKFUNESP8266CLIENTBUFFER_H_
#include <Arduino.h>

#define ESP8266_CLIENT_MAX_BUFFER_SIZE 512

class ESP8266ClientReadBuffer {
public:
	int available();
	int read();
	void setSerialPort(Stream* serialPort);

protected:
	Stream* _serial;
	uint8_t receiveBufferSize = 0;
	uint8_t receiveBuffer[ESP8266_CLIENT_MAX_BUFFER_SIZE];

	void fillReceiveBuffer();
	void truncateReceiveBufferHead(uint8_t startingOffset, uint8_t truncateLength);
	void cleanReceiveBufferFromAT();
	boolean cleanReceiveBufferFromAT(const char *atCommand, uint8_t additionalSuffixToKill = 0, const char *untilText = 0);
};

#endif