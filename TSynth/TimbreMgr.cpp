#include "TimbreMgr.h"
#include "AudioPatching.h" // For Global

CircularBuffer<TimbreProfile, TIMBRE_PROFILE_LIMIT> timbreProfiles;
CircularBuffer<TimbreSettings, MAX_NO_TIMBER> timbres;

void loadTimbreProfiles() {
  // TODO: Should the default profile be special, or should a profile be optional?
  timbreProfiles.push(TimbreProfile{1, "Default"});
  timbreProfiles.push(TimbreProfile{2, "Ghosts"});
  timbreProfiles.push(TimbreProfile{3, "Dynamic"});
  // TODO: hold settings to add a new profile.
  // TODO: hold save to delete a profile.


  timbres.push(TimbreSettings{1, "Overhead"});
  timbres.push(TimbreSettings{2, "Big Bass"});
  timbres.push(TimbreSettings{3, "Bells"});
}

FLASHMEM void refreshTimbres(std::vector<Voice *> &v, std::vector<VoiceGroup *> &g, Global &audio, patchInitCallback cb) {
  Serial.printf("Refresh timbres...\n");

  // Clear out groups
  while (!g.empty()) {
    VoiceGroup* group = g.back();
    g.pop_back();
    delete group;
  }

  Serial.printf("About to start.\n");
  auto voiceIndex = 0;
  if (timbres.size() > 0) {
    for (int i = 0; i < timbres.size(); i++) {
      Serial.printf("Adding timbre... %d\n", i);
      // TODO: Look at the config to decide how many voices to add.
      //VoiceGroup* group = new VoiceGroup();
      VoiceGroup *group = new VoiceGroup{audio.SharedAudio[g.size()]};
      for (uint32_t j = 0; j < (v.size() / timbres.size()); j++) {
        Serial.printf("Adding voice... %d\n", voiceIndex);
        group->add(v[voiceIndex++]);
      }
      g.push_back(group);

      // TODO: Look at patch name in config and lookup number.
      if (i == 0) {
        // Bells
        Serial.println("Recalling 23...");
        //recallPatch(23);
        cb(group, 23);
      }
      if (i == 1) {
        // Solid Bass
        Serial.println("Recalling 19...");
        cb(group, 19);
      }
      if (i == 2) {
        // Tremelo
        Serial.println("Recalling 25...");
        cb(group, 25);
      }
    }
  }
}