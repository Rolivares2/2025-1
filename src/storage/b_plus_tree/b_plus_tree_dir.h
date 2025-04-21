#pragma once

#include <memory>

#include "storage/b_plus_tree/b_plus_tree_utils.h"
#include "storage/page.h"

class BPlusTree;

/*
  Page layup:
  - First we have the child count (cc)
  - Then we have (cc) children, cc >= 1
  - Then we may have an empty space (unless cc == max_children)
  - Then we have (cc-1) records
 */
class BPlusTreeDir {
  friend class BPlusTree;

public:
  using record_t = BPlusTreeRecord;
  using child_t = int32_t;
  using child_count_t = int32_t;

  static constexpr int32_t max_records =
      (Page::SIZE - sizeof(child_count_t) - sizeof(child_t)) / (sizeof(child_t) + sizeof(record_t));

  static constexpr int32_t max_children = max_records + 1;

  static constexpr auto max_space_used =
      sizeof(child_count_t) + max_records * sizeof(record_t) + max_children * sizeof(child_t);

  static constexpr auto OFFSET_CHILD_COUNT = 0;

  static constexpr auto OFFSET_CHILDREN = sizeof(child_t);

  static constexpr auto OFFSET_RECORD = OFFSET_CHILDREN + (max_children) * sizeof(child_t);

  static_assert(max_space_used <= Page::SIZE);

  static_assert(OFFSET_CHILD_COUNT % alignof(child_count_t) == 0, "child count should be aligned");

  static_assert(OFFSET_CHILDREN % alignof(child_t) == 0, "children should be aligned");

  static_assert(OFFSET_RECORD % alignof(record_t) == 0, "records should be aligned");

  BPlusTreeDir(const BPlusTree& bpt, int32_t page_number);

  // used for new dir
  BPlusTreeDir(const BPlusTree& bpt);

  ~BPlusTreeDir();

  std::unique_ptr<BPlusTreeSplit> insert_record(const BPlusTreeRecord& record);

  void delete_record(const BPlusTreeRecord& record);

  // returns the leaf page number
  BPlusTreeSearchResult search_leaf(const BPlusTreeRecord& record);

private:
  int32_t search_child_idx(const BPlusTreeRecord& record);

  const BPlusTree& bpt;

  Page& page;

  int32_t get_child_count() const;

  // valid index goes from [0, get_key_count() - 1]
  int32_t get_child(size_t index) const;

  // valid index goes from [0, get_child_count() - 2]
  BPlusTreeRecord get_record(size_t index) const;

  void set_child_count(int32_t child_count);

  void set_child(size_t index, int32_t child);

  void set_record(size_t index, const BPlusTreeRecord& record);
};
