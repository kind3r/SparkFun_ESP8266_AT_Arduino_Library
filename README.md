Original [SparkFun AT Arduino Library](https://github.com/sparkfun/SparkFun_ESP8266_AT_Arduino_Library) updated with the following features:
* Arbitrary HardwareSerials (e.g. Serial1, Serial2, Serial3 on Arduino Mega 2560) - [mararn1618](https://github.com/mararn1618/SparkFun_ESP8266_AT_Arduino_Library).
* Buffer between ESP8266Client and ESP8266 for parsing AT controll commands - [mararn1618](https://github.com/mararn1618/SparkFun_ESP8266_AT_Arduino_Library).
* Initial UDP support - [semiotproject](https://github.com/semiotproject/SparkFun_ESP8266_AT_Arduino_Library).

TODO:
* Finalize UDP support.
* Update commands to [ESP8266_NONOS_SDK 2.0.0](https://github.com/espressif/ESP8266_NONOS_SDK); 2.1.0 seems to be buggy for me, keeps disconnecting and reconnecting the wifi in station mode.
* Create separate project with ESP8266 WiFi Shield firmware building instructions as fork from [original repository](https://github.com/sparkfun/ESP8266_WiFi_Shield).



SparkFun ESP8266 AT Arduino Library
========================================

![SparkFun ESP8266 WiFi Shield](https://cdn.sparkfun.com//assets/parts/1/0/5/3/8/13287-01.jpg)

[*SparkFun ESP8266 WiFi Shield (WRL-13287)*](https://www.sparkfun.com/products/13287)

An Arduino library for the SparkFun ESP8266 WiFi Shield. This library makes it easy to connect to WiFi networks, and also implements TCP/IP client and server.

Repository Contents
-------------------

* **/examples** - Example sketches for the library (.ino). Run these from the Arduino IDE. 
* **/extras** - Additional documentation for the user. These files are ignored by the IDE. 
* **/src** - Source files for the library (.cpp, .h).
* **keywords.txt** - Keywords from this library that will be highlighted in the Arduino IDE. 
* **library.properties** - General library properties for the Arduino package manager. 

Documentation
--------------

* **[Installing an Arduino Library Guide](https://learn.sparkfun.com/tutorials/installing-an-arduino-library)** - Basic information on how to install an Arduino library.
* **[Product Repository](https://github.com/sparkfun/ESP8266_WiFi_Shield)** - Main repository (including hardware files) for the ESP8266 WiFi Shield.
* **[Hookup Guide](https://learn.sparkfun.com/tutorials/esp8266-wifi-shield-hookup-guide)** - Basic hookup guide for the ESP8266 WiFi Shield.

Products that use this Library 
---------------------------------

* [SparkFun ESP8266 WiFi Shield (WRL-13287)](https://www.sparkfun.com/products/13287) - An Arduino shield featuring the ESP8266 WiFi SoC.

Version History
---------------

License Information
-------------------

This product is _**open source**_! 

The **code** is beerware; if you see me (or any other SparkFun employee) at the local, and you've found our code helpful, please buy us a round!

Please use, reuse, and modify these files as you see fit. Please maintain attribution to SparkFun Electronics and release anything derivative under the same license.

Distributed as-is; no warranty is given.

- Your friends at SparkFun.
