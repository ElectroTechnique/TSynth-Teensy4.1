#include <MonoNoteHistory.h>

void MonoNoteHistory::push(uint8_t note, uint8_t velocity) {
    data.push_back(Element{note, velocity});
}

void MonoNoteHistory::clear() {
    data.clear();
}

MonoNoteHistory::Element MonoNoteHistory::getLast() {
    Element result = data.back();
    return result;
}

MonoNoteHistory::Element MonoNoteHistory::getFirst() {
    Element result = data.front();
    return result;
}

MonoNoteHistory::Element MonoNoteHistory::getHighest() {
    Element highest = data.back();
    for(auto it = data.begin(); it != data.end(); it++) {
        if (it->note > highest.note) {
            highest = *it;
        }
    }
    return highest;
}

MonoNoteHistory::Element MonoNoteHistory::getLowest() {
    Element lowest = data.back();
    for(auto it = data.begin(); it != data.end(); it++) {
        if (it->note < lowest.note) {
            lowest = *it;
        }
    }
    return lowest;
}

int MonoNoteHistory::size() {
    return data.size();
}

void MonoNoteHistory::erase(uint8_t note) {
    for(auto it = data.begin(); it != data.end(); it++) {
        if (it->note == note) {
            data.erase(it--);
            return;
        }
    }
}