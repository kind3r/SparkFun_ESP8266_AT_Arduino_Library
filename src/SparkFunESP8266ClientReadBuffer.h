#ifndef _SPARKFUNESP8266CLIENTBUFFER_H_
#define _SPARKFUNESP8266CLIENTBUFFER_H_
#include <Arduino.h>

#define ESP8266_CLIENT_MAX_BUFFER_SIZE 128
#define ESP8266_CLIENT_MAX_BUFFER_COUNT 20

class ESP8266ClientReadBuffer {
public:
	ESP8266ClientReadBuffer();
	int available();
	int read();
	void setSerialPort(Stream* serialPort);

protected:
	Stream* _serial;
	byte receiveBufferRead = 0;
	byte receiveBufferWrite = 0;
	uint8_t receiveBufferSize[ESP8266_CLIENT_MAX_BUFFER_COUNT];
	uint8_t receiveBuffer[ESP8266_CLIENT_MAX_BUFFER_COUNT][ESP8266_CLIENT_MAX_BUFFER_SIZE];

	void fillReceiveBuffer();
	void truncateReceiveBufferHead(uint8_t startingOffset, uint8_t truncateLength);
	void incrementWriteBuffer();
	void incrementReadBuffer();
};

#endif