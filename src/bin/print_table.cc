#include <iostream>

#include "storage/heap_file/heap_file.h"
#include "storage/heap_file/heap_file_page.h"
#include "system/system.h"

int main(int argc, char* argv[]) {
  std::ios_base::sync_with_stdio(false);

  if (argc < 3) {
    std::cout << "Usage: print_table <db_directory> <table_name>" << std::endl;
    return EXIT_FAILURE;
  }

  std::string db_directory(argv[1]);
  std::string table_name(argv[2]);

  // Need to call System::init before start using the database
  // When this object comes out of scope the database is no longer usable
  auto system = System::init(db_directory, BufferManager::DEFAULT_BUFFER_SIZE);

  try {
    // print header
    Schema schema;
    HeapFile* heap_file = catalog.get_table(table_name, &schema);

    if (heap_file == nullptr) {
      std::cout << "Table " << table_name << " does not exists\n";
      return EXIT_FAILURE;
    }

    // print column names
    if (schema.columns.size() > 0) {
      std::cout << schema.columns[0].name;
    }
    for (size_t i = 1; i < schema.columns.size(); i++) {
      std::cout << ',' << schema.columns[i].name;
    }
    std::cout << '\n';

    int64_t total_results = 0;

    auto iter = heap_file->get_record_iter();

    Record record_buf(schema);
    iter->begin(record_buf);
    while (iter->next()) {
      std::cout << record_buf << '\n';
      total_results++;
    }
    std::cout << "\n got " << total_results << " results." << std::endl;

  } catch (const std::exception& e) {
    std::cout << "Query exception:\n";
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
