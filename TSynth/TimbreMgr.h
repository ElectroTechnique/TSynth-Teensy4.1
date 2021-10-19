#include <CircularBuffer.h>
#include "Constants.h"
#include <FS.h>
#include <SD.h>

#define TOTALCHARS 64

struct TimbreNoAndName{
  int timbreNo;
  String timbreName;
};

extern CircularBuffer<TimbreNoAndName, MAX_NO_TIMBER> timbres;