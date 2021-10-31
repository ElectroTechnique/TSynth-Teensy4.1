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
  //timbres.push(TimbreSettings{1, "Pulstar"});
  //timbres.push(TimbreSettings{2, "Big Bass"});
  //timbres.push(TimbreProfile{-1, "NEW"});
}