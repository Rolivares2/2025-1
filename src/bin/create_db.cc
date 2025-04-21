#include <iostream>

#include "system/system.h"

constexpr int64_t GB = 1024 * 1024 * 1024; // 1 GB

HeapFile& get_or_create_table(const std::string& table_name, std::vector<ColumnInfo>&& columns) {
  Schema table_schema(std::move(columns));
  Schema existing_table_schema;

  HeapFile* table = catalog.get_table(table_name, &existing_table_schema);
  if (table == nullptr) { // table doesn't exist
    table = catalog.create_table(table_name, table_schema);
  } else {
    assert(existing_table_schema == table_schema);
  }
  return *table;
}

void populate_t1() {
  for (int i = 1; i < 15; i++) {
    catalog.insert_record("T1", {"test.record." + std::to_string(i), i});
  }
}

void populate_t2() {
  for (int i = 10; i < 15; i++) {
    catalog.insert_record("T2", {i + 1, i + 5, i * 2 - 1});
  }
}

void populate_t3() {
  for (int i = 20; i < 25; i++) {
    catalog.insert_record("T3", {i, 2 * i, 3 * i});
  }
}

int main() {
  auto buffer_size = 1 * GB;
  std::string database_folder = "data/test_example";

  // Need to call System::init before start using the database
  // When this object comes out of scope the database is no longer usable
  auto system = System::init(database_folder, buffer_size);

  get_or_create_table("T1", {{"a", DataType::STR}, {"b", DataType::INT}});

  get_or_create_table("T2", {{"a", DataType::INT}, {"b", DataType::INT}, {"c", DataType::INT}});

  get_or_create_table("T3", {{"x", DataType::INT}, {"y", DataType::INT}, {"z", DataType::INT}});

  populate_t1();
  populate_t2();
  populate_t3();

  std::cout << "Database " << database_folder << " created" << std::endl;

  return EXIT_SUCCESS;
}
