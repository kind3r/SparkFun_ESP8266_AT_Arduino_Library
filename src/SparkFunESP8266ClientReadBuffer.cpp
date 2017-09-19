#include "SparkFunESP8266ClientReadBuffer.h"
#include "SparkFunESP8266WiFi.h"

void ESP8266ClientReadBuffer::setSerialPort(Stream* serialPort) 
{
	_serial = serialPort;
}

int ESP8266ClientReadBuffer::available()
{
	// if (receiveBufferSize > 0)//client has already buffered some payload
	// 	return receiveBufferSize;

	int available = _serial->available();
	if (available == 0)
	{
		// Delay for the amount of time it'd take to receive one character
		delayMicroseconds((1 / esp8266._baud) * 10 * 1E6);
		// Check again just to be sure:
		available = _serial->available();
	}
	return available + receiveBufferSize;
}

int ESP8266ClientReadBuffer::read()
{
	this->fillReceiveBuffer();//append to buffer BEFORE we read, so that chances are higher to detect AT commands

	//read from buffer
	if (receiveBufferSize > 0) {
		uint8_t ret = receiveBuffer[0];
		this->truncateReceiveBufferHead(0, 1);
		return ret;
	}

	return -1;
}

void ESP8266ClientReadBuffer::flush() 
{
	receiveBufferSize = 0;
}

void ESP8266ClientReadBuffer::truncateReceiveBufferHead(uint8_t startingOffset, uint8_t truncateLength) {
	for (uint8_t i = startingOffset; i < receiveBufferSize - truncateLength; i++)//shift buffer content; todo: better implementation
		receiveBuffer[i] = receiveBuffer[i + truncateLength];
	receiveBufferSize -= truncateLength;
}

void ESP8266ClientReadBuffer::fillReceiveBuffer() {
	//fill the receive buffer as much as possible from esp8266.read()

	if(_serial->available()) {
		// Serial.print("Filling up receive buffer with ");Serial.println(_serial->available());
		for (uint8_t attemps = 0; attemps < 5; attemps++) {//often 1st available() call does not yield all bytes => outer while
			while (uint8_t availableBytes = _serial->available() > 0) {
				for (; availableBytes > 0 && receiveBufferSize < ESP8266_CLIENT_MAX_BUFFER_SIZE; availableBytes--) {
					receiveBuffer[receiveBufferSize++] = _serial->read();
				}
				delay(10);
			}
			delay(10);
		}

		this->cleanReceiveBufferFromAT();
	}
}

void ESP8266ClientReadBuffer::cleanReceiveBufferFromAT() {
	//get rid of these esp8266 commands
	while(this->cleanReceiveBufferFromAT("\r\n+IPD,", 3, ":")) {}//typical answer looks like \r\n\r\n+IPD,0,4:<payload>
	// TODO: further refine to properly parse the \r\n+IPD,<slot>,<lenght>:<payload> packet
	// One major issue here with the current architecture is that even if we read the slot we don't know which client 
	// to send the <payload> to as it will be sent by the client doing the reading. So in principle esp8266 should do
	// reading and pass it to the client instance.
}

boolean ESP8266ClientReadBuffer::cleanReceiveBufferFromAT(const char *atCommand, uint8_t additionalSuffixToKill, const char *untilText) {
	uint8_t atLen = strlen(atCommand);
	boolean cleaned = false;

	// Serial.print("Dumping buffer of size ");Serial.println(receiveBufferSize);
	// for(uint8_t i=0; i < receiveBufferSize; i++) {
	// 	Serial.print("0x");
	// 	Serial.println(receiveBuffer[i], HEX);
	// }

	//uint8_t offset = 0;
	for (uint8_t offset = 0; offset <= receiveBufferSize - atLen; offset++) {
		if (0 == memcmp((receiveBuffer + offset), atCommand, atLen)) {
			// Serial.println("Found IPD");
			cleaned = true;
			//found the at command. KILL IT!
			//go further until we find the next text as well
			if(untilText != 0) {
				uint8_t untilLen = strlen(untilText);
				for(;additionalSuffixToKill <= receiveBufferSize - offset - atLen - untilLen; additionalSuffixToKill++) {
					if(0 == memcmp((receiveBuffer + offset + atLen + additionalSuffixToKill), untilText, untilLen)) {
						additionalSuffixToKill++;
						break;
					}
				}
			}
			// Serial.print("Truncating from ");Serial.print(offset); Serial.print(" length ");Serial.print(atLen + additionalSuffixToKill);
			// Serial.print(" remaining "); Serial.println(receiveBufferSize - atLen - additionalSuffixToKill);
			this->truncateReceiveBufferHead(offset, atLen + additionalSuffixToKill);
			break;
		}
	}
	 return cleaned;
}

