//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// clock_replacer.cpp
//
// Identification: src/buffer/clock_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/clock_replacer.h"

namespace bustub {

ClockReplacer::ClockReplacer(size_t num_pages) {
  this->frame = std::vector<std::optional<Slot>>(num_pages, std::nullopt);
}

ClockReplacer::~ClockReplacer() = default;

bool ClockReplacer::Victim(frame_id_t *frame_id) {
  auto slot = this->frame.begin();
  slot += this->frame_pointer;

  for (; slot != this->frame.end(); slot++) {
    this->frame_pointer = (this->frame_pointer + 1) % this->frame.size();
    if (slot->has_value() && !slot->value().isPinned()) {
      *frame_id = slot - this->frame.begin();
      this->frame[slot - this->frame.begin()] = std::nullopt;
      return true;
    }
  }

  return false;
}

void ClockReplacer::Pin(frame_id_t frame_id) {
  if (this->frame[frame_id].has_value()) {
    this->frame[frame_id].value().setPinned(true);
    this->frame_pointer = (this->frame_pointer + 1) % this->frame.size();
  }
}

void ClockReplacer::Unpin(frame_id_t frame_id) { this->frame[frame_id] = Slot{}; }

size_t ClockReplacer::Size() {
  size_t count = 0;
  for (std::optional<Slot> &s : this->frame) {
    if (s.has_value() && !s->isPinned()) {
      count++;
    }
  }

  return count;
}

}  // namespace bustub
