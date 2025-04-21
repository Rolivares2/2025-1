#pragma once

#include "relational_model/relation_iter.h"
#include "storage/b_plus_tree/b_plus_tree.h"
#include "storage/b_plus_tree/b_plus_tree_leaf.h"

class BPlusTreeIter : public RelationIter {
public:
  BPlusTreeIter(
      const BPlusTree& bpt,
      const Value& min,
      const Value& max,
      int64_t key_column_idx,
      BPlusTreeRecord min_record,
      BPlusTreeRecord max_record
  );

  virtual void begin(Record& out) override;

  virtual bool next() override;

  virtual void reset() override;

private:
  const BPlusTree& bpt;

  const Value min;

  const Value max;

  const int64_t key_column_idx;

  const BPlusTreeRecord min_record;

  const BPlusTreeRecord max_record;

  std::unique_ptr<BPlusTreeLeaf> current_leaf;

  Record* out;

  int64_t current_leaf_pos;
};
