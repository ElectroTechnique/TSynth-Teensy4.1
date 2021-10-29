#include <CircularBuffer.h>
#include "Constants.h"
#include <FS.h>
#include <SD.h>

#define TOTALCHARS 64

struct TimbreProfile{
  int profileNo;
  String profileName;
};

struct TimbreSettings{
  int timbreVoiceGroupIdx;
  String timbreName;
};

extern CircularBuffer<TimbreProfile, TIMBRE_PROFILE_LIMIT> timbreProfiles;
extern CircularBuffer<TimbreSettings, MAX_NO_TIMBER> timbres;

void loadTimbreProfiles();