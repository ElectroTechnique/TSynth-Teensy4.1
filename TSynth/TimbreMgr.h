#include <CircularBuffer.h>
#include "Constants.h"
#include <FS.h>
#include <SD.h>
#include <Voice.h>
#include <VoiceGroup.h>
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


//std::vector<VoiceGroup *> groupvec;
//std::vector<Voice*> voices;
//void refreshTimbres(std::vector<Voice *> &voices, std::vector<VoiceGroup *> &groups);
//void refreshTimbres(std::vector<Voice *> &voices, std::vector<VoiceGroup *> &groups, global &Global) {