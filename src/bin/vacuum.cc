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
    std::cout << "Table " << table_name << " does not exist\n";
    return EXIT_FAILURE;
  }

  heap_file->vacuum();

  return EXIT_SUCCESS;
}
