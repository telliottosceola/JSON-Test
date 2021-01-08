#include <Arduino.h>
#include <ArduinoJson.h>
#include <CloudInterface.h>
#include <DBQuery.h>
#include <HTMLHandler.h>
#include <SerialHandler.h>
#include <TCPServer.h>
#include <WiFiHandler.h>

Cloud cloud;
DBQuery dataBase;
HTMLHandler htmlHandler;
SerialHandler serialHandler;
TCPServer tcpServer;
WiFiHandler wifiHandler;
