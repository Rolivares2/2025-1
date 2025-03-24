#pragma once

#include <cstdint>
#include <memory>
#include <string>

using TableId = uint32_t;

class Schema;
class HeapFile;
class Record;

struct TableInfo {
  std::string name;
  std::unique_ptr<Schema> schema;
  std::unique_ptr<HeapFile> heap_file;

  // used as buffer, to avoid allocations
  std::unique_ptr<Record> record_buf;

  TableInfo(const std::string& name, std::unique_ptr<Schema> schema, std::unique_ptr<HeapFile> heap_file);
};