#include "fileSystem.h"

FileSystem::FileSystem()
{
  if (!SD.begin())
  {
    Serial.println("Card Mount Failed");
    return;
  }
}

FileSystem::~FileSystem()
{
}

std::string FileSystem::readFile(fs::FS &fs, const char *path)
{
  std::string value = "";

  //Serial.printf("Reading file: %s\n", path);
  // open file for reading
  File file = fs.open(path);
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return "";
  }
  // read from file
  while (file.available())
  {
    value += (char)file.read();
  }

  // close the file
  file.close();
  return value;
}

void FileSystem::writeFile(fs::FS &fs, const char *path, const char *message)
{
  // open file for writin
  File file = fs.open(path, FILE_WRITE);
  int bytesWritten = file.print(message);
 
  if (bytesWritten > 0) { 
  } else {
    Serial.println("File write failed");
  }
  // close the file
  file.close();
}
