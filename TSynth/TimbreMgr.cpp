#include "TimbreMgr.h"

CircularBuffer<TimbreProfile, TIMBRE_PROFILE_LIMIT> timbreProfiles;
CircularBuffer<TimbreSettings, MAX_NO_TIMBER> timbres;

void loadTimbreProfiles() {
  timbreProfiles.push(TimbreProfile{1, "Default"});
  timbreProfiles.push(TimbreProfile{2, "Ghosts"});
  timbreProfiles.push(TimbreProfile{3, "Dynamic"});
  timbreProfiles.push(TimbreProfile{-1, "NEW"});


  timbres.push(TimbreSettings{0, "Early Synth"});
  timbres.push(TimbreSettings{1, "Pulstar"});
  timbres.push(TimbreSettings{2, "Big Bass"});
  // TODO: double click save to add a timbre to the current profile.
  // TODO: hold save to delete the selected timbre.
  //timbres.push(TimbreProfile{-1, "NEW"});
}