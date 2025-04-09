#include "heap_file_page.h"

#include <cstdint>
#include <cstring>

#include "relational_model/record.h"
#include "storage/page.h"
#include "system/system.h"
#include <iostream>

HeapFilePage::HeapFilePage(FileId file_id, uint64_t page_number)
    : page(buffer_mgr.get_page(file_id, page_number)) {
  // if new page, initialize to be valid
  // new pages comes with all bytes setted at 0
  if (get_dir_count() == 0 && get_free_space() == 0) {
    set_free_space(Page::SIZE - 2 * sizeof(uint32_t));
  }
}

HeapFilePage::~HeapFilePage() {
  page.unpin();
}

void HeapFilePage::set_dir_count(int32_t new_dir_count) {
  page.write_int32(0, new_dir_count);
}

void HeapFilePage::set_free_space(int32_t new_free_space) {
  page.write_int32(4, new_free_space);
}

void HeapFilePage::set_dir(int32_t idx, int32_t new_dir_value) {
  page.write_int32(8 + 4 * idx, new_dir_value);
}

int32_t HeapFilePage::get_dir_count() const {
  return page.read_int32(0);
}

int32_t HeapFilePage::get_free_space() const {
  return page.read_int32(4);
}

int32_t HeapFilePage::get_dir(int32_t idx) const {
  return page.read_int32(8 + 4 * idx);
}

bool HeapFilePage::get_record(int32_t dir_pos, Record& out) const {
  auto offset = get_dir(dir_pos);
  if (offset <= 0) {
    return false;
  }

  for (auto& v : out.values) {
    switch (v.datatype) {
    case DataType::INT: {
      v.value.as_int = page.read_int64(offset);
      offset += sizeof(int64_t);
      break;
    }
    case DataType::STR: {
      uint8_t len = page.read_uint8(offset);
      offset += 1;

      page.read(offset, len, v.value.as_str);
      offset += len;

      v.value.as_str[len] = '\0';
      break;
    }
    }
  }

  return true;
}

void HeapFilePage::delete_record(int32_t dir_pos) {
  // TODO: implement
  //set_dir(dir_pos, -1);
}

bool HeapFilePage::try_insert_record(const Record& record, RID* out_record_id) {
  // TODO: implement
  auto serialized_record = serializeRecord(record);
  size_t record_size = serialized_record.size();
  size_t free_space = get_free_space();
  auto dir_count = get_dir_count();

  if (free_space < record_size) {
    // La página no tiene suficiente espacio.
    return false;
  }

  size_t free_space_offset = 8 + dir_count * 4 + free_space - record_size;
  std::cout << free_space << std::endl;
  std::cout << free_space_offset << std::endl;
  for (int i = 0; i < dir_count; i++) {
    if (get_dir(i) == -1) {
      page.write(free_space_offset, record_size, serialized_record.data());
      set_free_space(free_space - record_size);
      set_dir(i, free_space_offset);
      *out_record_id = RID(page.get_page_number(), i);
      return true;
    }
  }

  if (free_space > record_size + 4) {
      set_dir_count(dir_count + 1);
      page.write(free_space_offset, record_size, serialized_record.data());
      set_dir(dir_count, free_space_offset);
      set_free_space(free_space - record_size - 4);
      *out_record_id = RID(page.get_page_number(), dir_count);
      return true;
    }

  return false;
}

void HeapFilePage::vacuum(const Schema& schema) {
  char* page_buf = new char[Page::SIZE];
  // TODO: implement
  delete[] page_buf;
}

std::vector<char> HeapFilePage::serializeRecord(const Record& record) {
  std::vector<char> bytes;
  for (const auto& v : record.values) {
      switch (v.datatype) {
        case DataType::INT: {
          int64_t int_value = v.value.as_int;
          for (int i = 0; i<8 ;i++){
              bytes.push_back(static_cast<uint8_t>((int_value) >> (i*8) & 0xFF));
          }
          break;
        }
        case DataType::STR: {
          auto str_value = v.value.as_str;
          size_t str_len = strlen(str_value);
          std::cout << "El largo del string" <<str_len << std::endl;
          bytes.push_back(static_cast<uint8_t>(str_len));
          bytes.insert(bytes.end(), str_value, str_value + str_len);
          break;
        }
      }
  }
  return bytes;
}