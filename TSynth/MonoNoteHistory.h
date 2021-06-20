/**
 * This is a special data structure that stores notes for monophonic access.
 * It maintains a stack/queue for LIFO/FIFO access, and also provides getters
 * for access by highest/lowest note. Finally, notes at any index can be
 * removed.
 */
#ifndef MONONOTEHISTORY_H
#define MONONOTEHISTORY_H

#include <vector>
#include <stdint.h>
#include <stddef.h>

#define MAX_NOTE_HISTORY 20

class MonoNoteHistory {
    public:
    struct Element {
        uint8_t note;
        uint8_t velocity;
    };

    MonoNoteHistory();

    // Remove everything from the note stack.
    void clear();

    // Number of notes in history.
    int size();

    // Add notes to history.
    void push(uint8_t note, uint8_t velocity);

    // Erase note from history.
    void erase(uint8_t note);

    // Get the last note (most recently played).
    Element getLast();
    // Get the first note (oldest played).
    Element getFirst();
    // Get the highest note.
    Element getHighest();
    // Get the loweset note.
    Element getLowest();

    private:
    uint8_t numElements;
    Element data[MAX_NOTE_HISTORY];
};

#endif