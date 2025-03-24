#include <iostream>

#include "system/system.h"

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cout << "Usage: insert <db_directory> <table_name> <args...>" << std::endl;
    return EXIT_FAILURE;
  }

  std::string db_directory(argv[1]);
  std::string table_name(argv[2]);

  // Need to call System::init before start using the database
  // When this object comes out of scope the database is no longer usable
  auto system = System::init(db_directory, BufferManager::DEFAULT_BUFFER_SIZE);

  Schema schema;
  HeapFile* heap_file = catalog.get_table(table_name, &schema);

  if (heap_file == nullptr) {
    std::cout << "Table " << table_name << " does not exists\n";
    return EXIT_FAILURE;
  }

  if (static_cast<size_t>(argc - 3) != schema.columns.size()) {
    std::cout << "Table " << table_name << " expected " << schema.columns.size() << " columns, but received "
              << (argc - 3) << "\n";
    return EXIT_FAILURE;
  }

  std::vector<std::variant<std::string_view, int64_t>> values;
  for (int i = 3; i < argc; i++) {
    switch (schema.columns[i - 3].datatype) {
    case DataType::INT: {
      char* ptr;
      auto int_value = strtol(argv[i], &ptr, 10);
      if (*ptr != '\0') {
        std::cout << "Expected a number for column " << schema.columns[i - 3].name << ". Got '" << argv[i]
                  << "'\n";
        return EXIT_FAILURE;
      }
      values.emplace_back(int_value);
      break;
    }
    case DataType::STR: {
      values.emplace_back(argv[i]);
      break;
    }
    default: {
    }
    }
  }

  catalog.insert_record(table_name, values);

  std::cout << "Record inserted" << std::endl;
  return EXIT_SUCCESS;
}
