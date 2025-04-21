
#pragma once

#include "storage/b_plus_tree/b_plus_tree.h"
#include "storage/b_plus_tree/b_plus_tree_utils.h"

/*
  Page layup:
  - First we have the record count (rc)
  - Then we have the next leaf page number
  - Then we have (rc) records
 */
class BPlusTreeLeaf {
public:
  using record_count_t = int32_t;
  using next_leaf_t = int32_t;
  using record_t = BPlusTreeRecord;

  static constexpr int32_t max_records =
      (Page::SIZE - sizeof(record_count_t) - sizeof(next_leaf_t)) / (sizeof(record_t));

  static constexpr auto max_space_used =
      sizeof(record_count_t) + sizeof(next_leaf_t) + max_records * sizeof(record_t);

  static constexpr auto OFFSET_RECORD_COUNT = 0;

  static constexpr auto OFFSET_NEXT_LEAF = sizeof(record_count_t);

  static constexpr auto OFFSET_RECORDS = sizeof(record_count_t) + sizeof(next_leaf_t);

  static_assert(max_space_used <= Page::SIZE);

  static_assert(OFFSET_RECORD_COUNT % alignof(record_count_t) == 0, "record count should be aligned");

  static_assert(OFFSET_NEXT_LEAF % alignof(next_leaf_t) == 0, "next leaf should be aligned");

  static_assert(OFFSET_RECORDS % alignof(record_t) == 0, "records should be aligned");

  BPlusTreeLeaf(const BPlusTree& bpt, int32_t page_number);

  // used to create a new page
  BPlusTreeLeaf(const BPlusTree& bpt);

  ~BPlusTreeLeaf();

  std::unique_ptr<BPlusTreeSplit> insert_record(const BPlusTreeRecord& record);

  void delete_record(const BPlusTreeRecord& record);

  const BPlusTree& bpt;

  Page& page;

  int32_t get_record_count() const;

  // returns 0 if there is no next leaf (page 0 is always the leftmost leaf)
  int32_t get_next_page_number() const;

  BPlusTreeRecord get_record(int32_t idx) const;

  void set_record(int32_t idx, const BPlusTreeRecord& record);

  void set_record_count(int32_t);

  void set_next_page_number(int32_t);

  // returns the index of the lowest record that is greater or equal than the record received
  // if no such record exists (all records are lower) this method will return the record_count
  // BE CAREFUL on that case, accessing that index (same as calling get_record(get_record_count()))
  // is undefined behaviour and must be avoided
  int32_t search_index(const BPlusTreeRecord& record) const;
};
