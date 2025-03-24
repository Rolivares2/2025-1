#include "table_info.h"

#include "relational_model/record.h"
#include "relational_model/schema.h"
#include "storage/heap_file/heap_file.h"

TableInfo::TableInfo(
    const std::string& name, std::unique_ptr<Schema> _schema, std::unique_ptr<HeapFile> heap_file
)
    : name(name),
      schema(std::move(_schema)),
      heap_file(std::move(heap_file)) {
  record_buf = std::make_unique<Record>(*schema);
}
