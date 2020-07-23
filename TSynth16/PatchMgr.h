/*
  TSynth patch saving and recall works like an analogue polysynth from the late 70s (Prophet 5).
  When you recall a patch, all the front panel controls will be different values from those saved in the patch. Moving them will cause a jump to the current value.

  BACK
  Cancels current mode such as save, recall, delete and rename patches

  RECALL
  Recall shows list of patches. Use encoder to move through list.
  Enter button on encoder chooses highlighted patch or press Recall again.
  Recall also recalls the current patch settings if the panel controls have been altered.
  Holding Recall for 1.5s will initialise the synth with all the current panel control settings - the synth sounds the same as the controls are set.


  SAVE
  Save will save the current settings to a new patch at the end of the list or you can use the encoder to overwrite an existing patch.
  Press Save again to save it. If you want to name/rename the patch, press the encoder enter button and use the encoder and enter button to choose an alphanumeric name.
  Holding Save for 1.5s will go into a patch deletion mode. Use encoder and enter button to choose and delete patch. Patch numbers will be changed on the SD card to be consecutive again.
*/
//Agileware CircularBuffer available in libraries manager
#include <CircularBuffer.h>

#define TOTALCHARS 63

const char CHARACTERS[TOTALCHARS] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
int charIndex = 0;
char currentCharacter = 0;
String renamedPatch = "";

struct PatchNoAndName
{
  int patchNo;
  String patchName;
};

CircularBuffer<PatchNoAndName, PATCHES_LIMIT> patches;

size_t readField(File *file, char *str, size_t size, const char *delim)
{
  char ch;
  size_t n = 0;
  while ((n + 1) < size && file->read(&ch, 1) == 1)
  {
    // Delete CR.
    if (ch == '\r')
    {
      continue;
    }
    str[n++] = ch;
    if (strchr(delim, ch))
    {
      break;
    }
  }
  str[n] = '\0';
  return n;
}

void recallPatchData(File patchFile, String data[])
{
  //Read patch data from file and set current patch parameters
  size_t n;     // Length of returned field with delimiter.
  char str[20]; // Must hold longest field with delimiter and zero byte.
  int i = 0;
  while (patchFile.available() && i < NO_OF_PARAMS)
  {
    n = readField(&patchFile, str, sizeof(str), ",\n");
    // done if Error or at EOF.
    if (n == 0)
      break;
    // Print the type of delimiter.
    if (str[n - 1] == ',' || str[n - 1] == '\n')
    {
      // Remove the delimiter.
      str[n - 1] = 0;
    }
    else
    {
      // At eof, too long, or read error.  Too long is error.
      Serial.print(patchFile.available() ? F("error: ") : F("eof:   "));
    }
    // Print the field.
    //    Serial.print(i);
    //    Serial.print(" - ");
    //    Serial.println(str);
    data[i++] = String(str);
  }
}

int compare(const void *a, const void *b) {
  return ((PatchNoAndName*)a)->patchNo - ((PatchNoAndName*)b)->patchNo;
}

void sortPatches()
{
  int arraySize = patches.size();
  //Sort patches buffer to be consecutive ascending patchNo order
  struct PatchNoAndName arrayToSort[arraySize];

  for (int i = 0; i < arraySize; ++i)
  {
    arrayToSort[i] = patches[i];
  }
  qsort(arrayToSort, arraySize, sizeof(PatchNoAndName), compare);
  patches.clear();

  for (int i = 0; i < arraySize; ++i)
  {
    patches.push(arrayToSort[i]);
  }
}

void loadPatches()
{
  File file = SD.open("/");
  patches.clear();
  while (true)
  {
    String data[NO_OF_PARAMS]; //Array of data read in
    File patchFile = file.openNextFile();
    if (!patchFile)
    {
      break;
    }
    if (patchFile.isDirectory())
    {
      Serial.println("Ignoring Dir");
    }
    else
    {
      recallPatchData(patchFile, data);
      patches.push(PatchNoAndName{atoi(patchFile.name()), data[0]});
      Serial.println(String(patchFile.name()) + ":" + data[0]);
    }
    patchFile.close();
  }
  sortPatches();
}

void savePatch(const char *patchNo, String patchData)
{
  // Serial.print("savePatch Patch No:");
  //  Serial.println(patchNo);
  //Overwrite existing patch by deleting
  if (SD.exists(patchNo))
  {
    SD.remove(patchNo);
  }
  File patchFile = SD.open(patchNo, FILE_WRITE);
  if (patchFile)
  {
    //    Serial.print("Writing Patch No:");
    //    Serial.println(patchNo);
    //Serial.println(patchData);
    patchFile.println(patchData);
    patchFile.close();
  }
  else
  {
    Serial.print("Error writing Patch file:");
    Serial.println(patchNo);
  }
}

void savePatch(const char *patchNo, String patchData[])
{
  String dataString = patchData[0];
  for (int i = 1; i < NO_OF_PARAMS; i++)
  {
    dataString = dataString + "," + patchData[i];
  }
  savePatch(patchNo, dataString);
}

void deletePatch(const char *patchNo)
{
  if (SD.exists(patchNo)) SD.remove(patchNo);
}

void renumberPatchesOnSD() {
  for (int i = 0; i < patches.size(); i++)
  {
    String data[NO_OF_PARAMS]; //Array of data read in
    File file = SD.open(String(patches[i].patchNo).c_str());
    if (file) {
      recallPatchData(file, data);
      file.close();
      savePatch(String(i + 1).c_str(), data);
    }
  }
  deletePatch(String(patches.size() + 1).c_str()); //Delete final patch which is duplicate of penultimate patch
}

void setPatchesOrdering(int no) {
  if (patches.size() < 2)return;
  while (patches.first().patchNo != no) {
    patches.push(patches.shift());
  }
}

void resetPatchesOrdering() {
  setPatchesOrdering(1);
}
