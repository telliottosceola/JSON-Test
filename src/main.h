#include <Arduino.h>
#include <ArduinoJson.h>
#include <CloudInterface.h>
#include <DBQuery.h>
#include <FileSystem.h>
#include <HTMLHandler.h>
#include <SerialHandler.h>
#include <TCPServer.h>
#include <WiFiHandler.h>

Cloud cloud;
DBQuery dataBase;
FileSystem fileSystem;
HTMLHandler htmlHandler;
SerialHandler serialHandler;
TCPServer tcpServer;
WiFiHandler wifiHandler;
