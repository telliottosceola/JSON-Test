#ifndef DBQUERY_H
#define DBQUERY_H

#include <ArduinoJson.h>
#include <SPIFFS.h>

class DBQuery{
public:
  bool init();
  bool load();
  void storeFile(char* fileName, char* data, int dataLen);
private:
  char dbBuffer[5012];
  bool readFile(fs::FS &fs, const char * path, char* buffer, int len);
  int getFileSize(fs::FS &fs, const char * path);
};

#endif
