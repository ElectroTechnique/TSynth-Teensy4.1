#include "MonoNoteHistory.h"

MonoNoteHistory::MonoNoteHistory():
    numElements(0){
}

// Drop element at index, shift everything else down.
void shiftDataFrom(uint8_t index, uint8_t numElements, MonoNoteHistory::Element data[MAX_NOTE_HISTORY]) {
    for(uint8_t cur = index; (cur+1) < numElements; cur++) {
        data[cur] = data[cur+1];
    }
}

void MonoNoteHistory::push(uint8_t note, uint8_t velocity) {
    if (numElements == MAX_NOTE_HISTORY) {
        shiftDataFrom(0, numElements, data);
        numElements--;
    }
    data[numElements++] = Element{note, velocity};
}

void MonoNoteHistory::clear() {
    numElements = 0;
}

MonoNoteHistory::Element MonoNoteHistory::getLast() {
    Element result = data[numElements-1];
    return result;
}

MonoNoteHistory::Element MonoNoteHistory::getFirst() {
    Element result = data[0];
    return result;
}

MonoNoteHistory::Element MonoNoteHistory::getHighest() {
   uint8_t highest = 0;
   for (int cur = 1; cur < size(); cur++) {
       if (data[cur].note > data[highest].note) {
           highest = cur;
       }
   }
   return data[highest];
}

MonoNoteHistory::Element MonoNoteHistory::getLowest() {
   uint8_t highest = 0;
   for (uint8_t cur = 1; cur < size(); cur++) {
       if (data[cur].note < data[highest].note) {
           highest = cur;
       }
   }
   return data[highest];
}

int MonoNoteHistory::size() {
    return numElements;
}

void MonoNoteHistory::erase(uint8_t note) {
   for (uint8_t cur = 0; cur < size(); cur++) {
       if (data[cur].note == note) {
           shiftDataFrom(cur, size(), data);
           numElements--;
           cur--;
       }
   }
}
