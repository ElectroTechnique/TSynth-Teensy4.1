#include <stdint.h>
#include "AudioPatching.h"
#include "Constants.h"
#include "VoiceGroup.h"

extern uint32_t state;

// Initialize the audio configuration.
extern Global global;
//VoiceGroup voices1{global.SharedAudio[0]};
extern std::vector<VoiceGroup *> groupvec;
extern uint8_t activeGroupIndex;