#include "globals.h"

uint32_t state = PARAMETER;

// Initialize the audio configuration.
Global global{VOICEMIXERLEVEL};

//VoiceGroup voices1{global.SharedAudio[0]};
std::vector<VoiceGroup *> groupvec;
uint8_t activeGroupIndex = 0;