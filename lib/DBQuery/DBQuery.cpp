#include <DBQuery.h>

bool DBQuery::init(){
  if(!SPIFFS.begin()){
    Serial.println("SPIFFS load failed");
    return false;
  }
  return load();
}

bool DBQuery::load(){
  //Load JSON database file from storage
  int dataBaseFileLength = getFileSize(SPIFFS, "/dataBaseFile.json");
  if(dataBaseFileLength != 0){
    char dataBaseFileBuffer[dataBaseFileLength+1];
    if(readFile(SPIFFS, "/dataBaseFile.json", dataBaseFileBuffer, dataBaseFileLength)){
      //Parse loaded database JSON file and verify it is valid
      DynamicJsonBuffer jsonBuffer;
      JsonObject& dataBase = jsonBuffer.parseObject(dataBaseFileBuffer);
      if(!dataBase.success()){
        Serial.println("database is corrupt");
        //set the database to a blank JSON since it's corrupt.
        File file = SPIFFS.open("/dataBaseFile.json", FILE_WRITE);
        JsonObject& blankJSON = jsonBuffer.createObject();
        blankJSON.printTo(file);
        memset(dbBuffer, 0, sizeof(dbBuffer));
        blankJSON.printTo(dbBuffer, sizeof(dbBuffer));
        file.close();
        return false;
      }
      //Put the database into the RAM buffer
      dataBase.printTo(dbBuffer, sizeof(dbBuffer));
    }
  }
  Serial.println("FileSystem Load Failed.");
  return false;
}

void DBQuery::storeFile(char* fileName, char* data, int dataLen){
  //Parse data passed in to see if it is valid
  DynamicJsonBuffer jBuffer;
  JsonObject& newData = jBuffer.parseObject(data);
  if(!newData.success()){
    Serial.printf("invalid data in file %s\n", fileName);
    return;
  }
  //Load database out of RAM into JSON object
  JsonObject& dB = jBuffer.parseObject(dbBuffer);
  //Iterate through values to see if they exist in database already, if so update, if not add them to the database
  for(auto kv : newData){
    dB[kv.key] = kv.value;
  }
  memset(dbBuffer, 0, sizeof(dbBuffer));
  dB.printTo(dbBuffer, sizeof(dbBuffer));
}

//Helper function to read a Spiffs file into a char array
bool DBQuery::readFile(fs::FS &fs, const char * path, char* buffer, int len){
  char internalBuffer[len];
  memset(internalBuffer, 0, len);
  delay(300);

  File file = fs.open(path);
  if(!file || file.isDirectory()){
    #ifdef DEBUG
    Serial.println("- failed to open file for reading(readFile)");
    Serial.println(path);
    #endif
    return false;
  }
  for(int i = 0; i < len; i++){
    internalBuffer[i] = file.read();
  }
  memcpy(buffer, internalBuffer, len);
  return true;
}

//Helper function to get the size of a SPIFFS file
int DBQuery::getFileSize(fs::FS &fs, const char * path){
  File file = fs.open(path);
  if(!file || file.isDirectory()){
    #ifdef DEBUG
    Serial.println("- failed to open file for reading(getFileSize)");
    Serial.println(path);
    #endif
    return 0;
  }else{
    return file.available();
  }
}
