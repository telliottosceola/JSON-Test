#include <FileSystem.h>

void FileSystem::init(DBQuery& dbQuery){
  _dbQuery = &dbQuery;
  load();
}

bool FileSystem::load(){
  //Retrieve file names from file map.  This is a JSON array containing the names of all stored JSON files
  int mapFileLength = getFileSize(SPIFFS, "/file_map.txt");
  if(mapFileLength != 0){
    char mapFileBuffer[mapFileLength+1];
    if(readFile(SPIFFS, "/file_map.txt", mapFileBuffer, mapFileLength)){
      //Parse loaded array of names to a JsonArray we can itterate.
      DynamicJsonBuffer jsonBuffer;
      JsonArray& jsonFiles = jsonBuffer.parseArray(mapFileBuffer);
      JsonObject& rootObject = jsonBuffer.createObject();
      if(!jsonFiles.success()){
        return false;
      }
      //Iterate through stored files and combine them into a single Object
      int loadedFiles = 0;
      unsigned long now = millis();
      for(auto fileName : jsonFiles){
        Serial.printf("Loading file: %s\n", fileName.as<char*>());
        int fileLen = getFileSize(SPIFFS, fileName.as<char*>());
        if(fileLen != 0){
          Serial.printf("Loading file: %s", fileName.as<char*>());
          char fileBuffer[fileLen+1];
          if(readFile(SPIFFS, "/file_map.txt", fileBuffer, fileLen)){
            //Put loaded file into JSON object
            JsonObject& loadedObject = jsonBuffer.parseObject(fileBuffer);
            if(!loadedObject.success()){
              Serial.println("-File Corrupt");
            }else{
              //Iterate through all Key Value Pairs in this file and put them in the root object
              for(auto kv : loadedObject){
                rootObject[kv.key] = kv.value;
              }
              Serial.println("-Complete");
              loadedFiles++;
            }
          }
        }
      }
      Serial.printf("Loaded %i files in %i milliseconds", loadedFiles, (int)(millis()-now));

      //Pass Object to DBQuery for future use there.
      char dbObject[rootObject.size()+1];
      rootObject.printTo(dbObject, sizeof(dbObject));
      _dbQuery->update(dbObject, sizeof(dbObject));
    }
  }
  Serial.println("FileSystem Load Failed.");
  return false;
}
void FileSystem::storeFile(char* fileName, char* data, int dataLen){

}

//Helper function to read a Spiffs file into a char array
bool FileSystem::readFile(fs::FS &fs, const char * path, char* buffer, int len){
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
int FileSystem::getFileSize(fs::FS &fs, const char * path){
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
