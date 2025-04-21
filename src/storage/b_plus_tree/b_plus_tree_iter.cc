#include "b_plus_tree_iter.h"

BPlusTreeIter::BPlusTreeIter(
    const BPlusTree& bpt,
    const Value& min,
    const Value& max,
    int64_t key_column_idx,
    BPlusTreeRecord min_record,
    BPlusTreeRecord max_record
)
    : bpt(bpt),
      min(min),
      max(max),
      key_column_idx(key_column_idx),
      min_record(min_record),
      max_record(max_record) {}

void BPlusTreeIter::begin(Record& _out) {
  out = &_out;
  reset();
}

void BPlusTreeIter::reset() {
  auto search_leaf_res = bpt.root->search_leaf(min_record);
  current_leaf = std::make_unique<BPlusTreeLeaf>(bpt, search_leaf_res.leaf_page_number);
  current_leaf_pos = search_leaf_res.pos;
}

bool BPlusTreeIter::next() {
  while (true) {
    if (current_leaf_pos < current_leaf->get_record_count()) {
      auto pos = current_leaf_pos++;
      auto current_record = current_leaf->get_record(pos);

      if (max_record < current_record) {
        // in this case we know all next records will be greater than max
        return false;
      }

      bpt.heap_file.get_record(current_record.rid, *out);
      auto value = out->values[key_column_idx];

      // important to check this as we lose information for strings in key serializing
      if (value >= min && value <= max) {
        return true;
      }
    } else if (current_leaf->get_next_page_number() != 0) {
      current_leaf = std::make_unique<BPlusTreeLeaf>(bpt, current_leaf->get_next_page_number());
      current_leaf_pos = 0;
    } else {
        // there is no next leaf
        return false;
    }
  }
  return false;
}
