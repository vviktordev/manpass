//
// Created by Andrzej Machnik on 27/05/2025.
//

// Directory: src/vault/NoteEntry.cpp
#include "vault/NoteEntry.h"

namespace vault {

    NoteEntry::NoteEntry(const std::string &note) : noteText(note) {}

    std::string NoteEntry::getType() const {
        return "note";
    }

    const std::string &NoteEntry::getNoteText() const {
        return noteText;
    }

} // namespace vault