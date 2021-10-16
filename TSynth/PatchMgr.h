/*
  TSynth patch saving and recall works like an analogue polysynth from the late 70s (Prophet 5).
  When you recall a patch, all the front panel controls will be different values from those saved in the patch. 
  Moving them will cause a jump to the current value.
*/
//Agileware CircularBuffer available in libraries manager
#include <CircularBuffer.h>
#include "Constants.h"
#include <FS.h>
#include <SD.h>

#define TOTALCHARS 64

struct PatchNoAndName{
  int patchNo;
  String patchName;
};

extern CircularBuffer<PatchNoAndName, PATCHES_LIMIT> patches;

size_t readField(File *file, char *str, size_t size, const char *delim);

void recallPatchData(File patchFile, String data[]);

void sortPatches();

void loadPatches();

void savePatch(const char *patchNo, String patchData);

void savePatch(const char *patchNo, String patchData[]);

void deletePatch(const char *patchNo);

void renumberPatchesOnSD();

void setPatchesOrdering(int no);

void resetPatchesOrdering();
