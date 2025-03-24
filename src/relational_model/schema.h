#pragma once

#include <string>
#include <vector>

// IIC3413-DB will only support as datatypes:
// - strings up to 255 bytes length in UTF-8 representation
// - 64 bit signed integer
enum class DataType { STR, INT };

struct ColumnInfo {
  std::string name;
  DataType datatype;

  bool operator==(const ColumnInfo& other) const {
    return name == other.name && datatype == other.datatype;
  }
};

class Schema {
public:
  Schema(std::vector<ColumnInfo> columns)
      : columns(std::move(columns)) {}

  Schema() {}

  std::vector<ColumnInfo> columns;

  bool operator==(const Schema& other) const {
    return columns == other.columns;
  }
};
