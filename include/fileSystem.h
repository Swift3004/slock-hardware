#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

class FileSystem {
  private:

  public:
  FileSystem();
  ~FileSystem();
  std::string readFile(fs::FS &fs, const char * path);
  void writeFile(fs::FS &fs, const char * path, const char * message);
};

#endif