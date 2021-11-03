#include <stdint.h>
#include "AudioPatching.h"
#include "Constants.h"
#include "VoiceGroup.h"

extern uint32_t state;

// Initialize the audio configuration.
extern Global global;
extern std::vector<VoiceGroup *> groupvec;

// Reference to all of the voices, connected through the shared audio chain.
extern std::vector<Voice*> voices;
extern uint8_t activeGroupIndex;