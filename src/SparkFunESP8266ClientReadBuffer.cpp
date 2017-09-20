#include "SparkFunESP8266ClientReadBuffer.h"
#include "SparkFunESP8266WiFi.h"

ESP8266ClientReadBuffer::ESP8266ClientReadBuffer()
{
	for (byte i = 0; i < ESP8266_CLIENT_MAX_BUFFER_COUNT; i++)
	{
		receiveBufferSize[i] = 0;
	}
}

void ESP8266ClientReadBuffer::setSerialPort(Stream *serialPort)
{
	_serial = serialPort;
}

int ESP8266ClientReadBuffer::available()
{
	fillReceiveBuffer();
	incrementReadBuffer();
	return receiveBufferSize[receiveBufferRead];
}

int ESP8266ClientReadBuffer::read()
{
	//read from buffer
	incrementReadBuffer();
	if (receiveBufferSize[receiveBufferRead] > 0)
	{
		uint8_t ret = receiveBuffer[receiveBufferRead][0];
		this->truncateReceiveBufferHead(0, 1);
		return ret;
	}

	return -1;
}

void ESP8266ClientReadBuffer::truncateReceiveBufferHead(uint8_t startingOffset, uint8_t truncateLength)
{
	for (uint8_t i = startingOffset; i < receiveBufferSize[receiveBufferRead] - truncateLength; i++) //shift buffer content; todo: better implementation
		receiveBuffer[receiveBufferRead][i] = receiveBuffer[receiveBufferRead][i + truncateLength];
	receiveBufferSize[receiveBufferRead] -= truncateLength;
}

void ESP8266ClientReadBuffer::fillReceiveBuffer()
{
	// while(_serial->available()) {
	// 	Serial.print(_serial->read(), HEX);Serial.print(' ');
	// }
	/*
+IPD 2C 30 2C 37 3A 7 0 40 0 21 24 5 
+IPD 2C 30 2C 37 3A 7 0 40 0 21 24 5 
+IPD 2C 30 2C 39 3A 9 0 40 0 E3 F0 0 3 10 
+IPD 2C 
+IPD 2C 30 2C 39 3A 9 0 40 0 E3 F0 0 3 10 
+IPD 2C 30 2C 39 3A 9 0 40 0 E3 F0 0 3 10 
+IPD 2C 30 2C 36 3A 6 0 60 0 0 3 
+IPD 2C 30 2C 37 3A 7 0 40 0 21 24 5 
+IPD 2C 30 2C 39 3A 9 0 40 0 E3 F0 0 3 10 


+IPD 2C 30 2C 39 3A 9 0 40 0 E3 F0 0 3 10 
+IPD 2C 30 2C 37 3A 7 0 40 0 21 24 5 
+IPD 2C 30 2C 38 3A 8 0 50 0 3 0 0 9 
+IPD 0 3 10 D 2B 50 44 30 39 3A 0 0 E3 F0 0 3 D +IPD 2C 30 39 9 0 0 F0 0 10 A 2B 49 50 2C 30 36 3A 6 0 60 0 
+IPD 2C 30 2C 37 3A 7 0 40 0 21 24 5 
+IPD 2C 30 2C 39 3A 9 0 40 0 E3 F0 0 3 10 
+IPD 2C 30 2C 31 30 3A A 0 40 0 E4 13 0 3 C2 36 
+IPD 2C 30 2C 37 3A 7 0 40 0 21 24 5 
+IPD 2C 30 2C 39 3A 9 0 40 0 E3 F0 0 3 10 
	*/
	// if at least 10(header)+1(data) serial data is available and we don't allready have a ready buffer
	// start reading the full packet (until the declared length from header)
	int lastAvailable = _serial->available();
	if (lastAvailable >= 12 && _serial->peek() == 13)
	{
		Serial.print("Reading from available ");Serial.print(lastAvailable);Serial.println(" bytes");
		incrementWriteBuffer();

		// Serial.println("Reading incomming packet");
		char cmd[7];
		// blank read until the + from +IPD (can be one or two CR+LF)
		byte crlfSize = _serial->readBytesUntil('+', cmd, 6);
		if (crlfSize > 5)
		{
			// then we don't really have an incomming packet do we ?
			Serial.println("Error receiving packet");
			return 0;
		}
		_serial->readBytes((char *)cmd, 4);
		if (0 == memcmp(cmd, "IPD,", 4))
		{
			// Serial.println("GOT IPD");
			// read slot
			char slot = _serial->read();
			// read separator (comma)
			_serial->read();
			char packetSizeString[5];
			int sizeOfPacketSize = _serial->readBytesUntil(':', packetSizeString, 4);
			if (sizeOfPacketSize == 0)
			{
				// error reading packet
				Serial.println("Inconsistent read");
			}
			// Serial.print("Read string packet size ");Serial.println(packetSizeString);
			int packetSize = atoi(packetSizeString);
			Serial.print("Reading packet from ");Serial.print(slot);Serial.print(" size ");Serial.println(packetSize);
			// Serial.println(_serial->peek());
			lastAvailable = _serial->available();
			while (receiveBufferSize[receiveBufferWrite] < packetSize)
			{
				receiveBuffer[receiveBufferWrite][receiveBufferSize[receiveBufferWrite]++] = _serial->read();
				// Serial.println(receiveBuffer[receiveBufferWrite][receiveBufferSize[receiveBufferWrite]-1], HEX);
				lastAvailable--;
				if (lastAvailable == 0)
				{
					delayMicroseconds((1 / esp8266._baud) * 10 * 1E6);
					// delay(1);
					lastAvailable = _serial->available();
				}
			}
			Serial.print("Read ");Serial.print(receiveBufferSize[receiveBufferWrite]);Serial.println(" bytes");
			// Serial.println(lastAvailable);
		}
		else
		{
			Serial.println("Error in reading IPD");
			for (byte i = 0; i < 4; i++)
			{
				Serial.println(cmd[i], HEX);
			}
			for(int i =0;i<_serial->available();i++) {
				Serial.println(_serial->read(), HEX);
			}
		}
	}
}

void ESP8266ClientReadBuffer::incrementWriteBuffer()
{
	if (receiveBufferSize[receiveBufferWrite] > 0)
	{
		receiveBufferWrite = (receiveBufferWrite + 1) % ESP8266_CLIENT_MAX_BUFFER_COUNT;
	}
}

void ESP8266ClientReadBuffer::incrementReadBuffer()
{
	while (receiveBufferSize[receiveBufferRead] == 0 && receiveBufferRead != receiveBufferWrite)
	{
		receiveBufferRead = (receiveBufferRead + 1) % ESP8266_CLIENT_MAX_BUFFER_COUNT;
	}
}
