#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <SPIFFS.h>
#include <DBQuery.h>
#include <ArduinoJson.h>

class FileSystem{
public:
  bool init(DBQuery& dbQuery);
  bool load();
  void storeFile(char* fileName, char* data, int dataLen);

private:
  DBQuery* _dbQuery;

  bool readFile(fs::FS &fs, const char * path, char* buffer, int len);
  int getFileSize(fs::FS &fs, const char * path);
};

#endif
