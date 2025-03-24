#include <algorithm>
#include <iostream>

#include "system/system.h"

int main(int argc, char* argv[]) {
  if (argc < 4) {
    std::cout << "Usage: insert <db_directory> <table_name> <col_name:datatype...>\n"
                 "Valid datatypes: STR|INT"
              << std::endl;
    return EXIT_FAILURE;
  }

  std::string db_directory(argv[1]);
  std::string table_name(argv[2]);

  std::string database_folder = "data/test_example";

  // Need to call System::init before start using the database
  // When this object comes out of scope the database is no longer usable
  auto system = System::init(database_folder, BufferManager::DEFAULT_BUFFER_SIZE);

  try {
    std::vector<ColumnInfo> columns;

    for (int i = 3; i < argc; i++) {
      std::string str(argv[i]);
      size_t colon_pos = str.find(':');
      if (colon_pos == std::string::npos) {
        std::cout << "Malformed '" << str << "'. Expected something like 'col_name:datatype'\n";
        return EXIT_FAILURE;
      }

      // Split the string into two parts
      auto col_name = str.substr(0, colon_pos);
      auto dt = str.substr(colon_pos + 1);

      if (col_name.empty() || dt.empty()) {
        std::cout << "Malformed '" << str << "'. Expected something like 'col_name:datatype'\n";
        return EXIT_FAILURE;
      }
      std::transform(dt.begin(), dt.end(), dt.begin(), [](unsigned char c) { return std::tolower(c); });

      DataType datatype;
      if (dt == "int") {
        datatype = DataType::INT;
      } else if (dt == "str") {
        datatype = DataType::STR;
      } else {
        std::cout << "unrecognized datatype " << dt << ". Expecting STR | INT\n";
        return EXIT_FAILURE;
      }

      columns.push_back({col_name, datatype});
    }

    Schema schema(std::move(columns));
    HeapFile* table = catalog.get_table(table_name, &schema);
    if (table == nullptr) { // table doesn't exist
      table = catalog.create_table(table_name, schema);
    } else {
      std::cout << "Table " << table_name << " already exists\n";
      return EXIT_FAILURE;
    }

    std::cout << "Table " << table_name << " created" << std::endl;

  } catch (const std::exception& e) {
    std::cout << "EXCEPTION:\n" << e.what() << std::endl;
    return EXIT_SUCCESS;
  }

  return EXIT_SUCCESS;
}
