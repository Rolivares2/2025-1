#pragma once

#include <cstdint>

#include "storage/heap_file/rid.h"

struct BPlusTreeRecord {
  int64_t encoded_key;
  RID rid;

  BPlusTreeRecord(int64_t encoded_key, RID rid)
      : encoded_key(encoded_key),
        rid(rid) {}

  bool operator<(const BPlusTreeRecord& other) const {
    if (encoded_key < other.encoded_key) {
      return true;
    } else if (other.encoded_key < encoded_key) {
      return false;
    }

    return rid < other.rid;
  }

  bool operator==(const BPlusTreeRecord& other) const {
    return encoded_key == other.encoded_key && rid == other.rid;
  }
};

struct BPlusTreeSplit {
  BPlusTreeRecord record;
  int32_t encoded_page_number;

  BPlusTreeSplit(BPlusTreeRecord record, int32_t encoded_page_number)
      : record(record),
        encoded_page_number(encoded_page_number) {}
};

struct BPlusTreeSearchResult {
  int32_t leaf_page_number;
  int32_t pos;

  BPlusTreeSearchResult(int32_t leaf_page_number, int32_t pos)
      : leaf_page_number(leaf_page_number),
        pos(pos) {}
};
