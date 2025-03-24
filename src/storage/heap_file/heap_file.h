#pragma once

#include <memory>
#include <string>

#include "relational_model/record.h"
#include "relational_model/table_info.h"
#include "storage/file_id.h"
#include "storage/heap_file/heap_file_iter.h"
#include "storage/heap_file/rid.h"

class HeapFile {
public:
  const Schema& schema;

  const FileId file_id;

  const TableId table_id;

  HeapFile(TableId table_id, const Schema& schema, const std::string& table_name);

  // prevent accidental copies
  HeapFile(const HeapFile& other) = delete;

  RID insert_record(const Record& record);

  void delete_record(RID rid);

  void get_record(RID rid, Record& out) const;

  void vacuum();

  // Iterates over all results
  std::unique_ptr<HeapFileIter> get_record_iter() const;

private:
  // remembers where was the last insert so it doesn't begin from the start
  // the next time
  uint_fast32_t last_insert_page = 0;
};
