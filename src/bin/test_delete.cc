#include <iostream>

#include "system/system.h"

// ADVERTENCIA: Esta prueba esta hecha para ser ejecutada usando
//              la base de datos proporcionada (esta contiene los datos para borrar)

int main(int argc, char* argv[]) {
  // if (argc < 3) {
  //   std::cout << "Usage: insert <db_directory> <table_name> <args...>" << std::endl;
  //   return EXIT_FAILURE;
  // }

  // std::string db_directory(argv[1]);
  // std::string table_name(argv[2]);

  std::string db_directory = "data/to_delete";
  std::string table_name = "T1";

  // Need to call System::init before start using the database
  // When this object comes out of scope the database is no longer usable
  auto system = System::init(db_directory, BufferManager::DEFAULT_BUFFER_SIZE);

  Schema schema;
  HeapFile* heap_file = catalog.get_table(table_name, &schema);

  if (heap_file == nullptr) {
    std::cout << "Table " << table_name << " does not exists\n";
    return EXIT_FAILURE;
  }

  auto iter = heap_file->get_record_iter();

  Record record_buf(schema);
  iter->begin(record_buf);
  std::string look = "test.record.7";
  while(iter->next()) {
    if (!strcmp(record_buf.values[0].value.as_str, look.c_str())) {
      break;
    }
  }

  RID rid = iter->get_current_RID();
  catalog.delete_record(table_name, rid);

  return EXIT_SUCCESS;
}
