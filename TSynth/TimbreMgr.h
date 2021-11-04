#include <CircularBuffer.h>
#include "Constants.h"
#include <FS.h>
#include <SD.h>
#include "Voice.h"
#include "VoiceGroup.h"
#include <vector>

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

typedef int (*patchInitCallback)(VoiceGroup *group, const char *patchNo);

void refreshTimbres(std::vector<Voice *> &v, std::vector<VoiceGroup *> &g, Global &audio, patchInitCallback initCb);
