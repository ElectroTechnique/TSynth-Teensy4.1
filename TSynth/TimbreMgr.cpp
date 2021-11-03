#include "TimbreMgr.h"
#include "AudioPatching.h" // For Global

CircularBuffer<TimbreProfile, TIMBRE_PROFILE_LIMIT> timbreProfiles;
CircularBuffer<TimbreSettings, MAX_NO_TIMBER> timbres;

void loadTimbreProfiles() {
  Serial.println("loadTimbreProfiles()");
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

FLASHMEM void refreshTimbres(std::vector<Voice *> &v, std::vector<VoiceGroup *> &g, Global &audio, patchInitCallback initCb) {
  // Clear out groups
  while (!g.empty()) {
    VoiceGroup* group = g.back();
    g.pop_back();
    delete group;
  }

  auto voiceIndex = 0;
  auto numTimbres = timbres.size();
  if (numTimbres > 0) {
    for (int i = 0; i < numTimbres; i++) {
      // TODO: Look at the config to decide how many voices to add.
      //VoiceGroup* group = new VoiceGroup();
      VoiceGroup *group = new VoiceGroup{audio.SharedAudio[g.size()]};
      for (uint32_t j = 0; j < (v.size() / numTimbres); j++) {
        group->add(v[voiceIndex++]);
      }
      g.push_back(group);

      // Load timbre using callback.
      Serial.printf("Timbre lookup '%s': ", timbres[i].timbreName.c_str());
      int patchNo = initCb(group, timbres[i].timbreName.c_str());
      if (patchNo < 0) 
        Serial.printf("not found\n");
      else
        Serial.printf("%d\n", patchNo);
    }
  }
}