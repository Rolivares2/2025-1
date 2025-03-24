#include "heap_file_page.h"

#include <cstdint>
#include <cstring>

#include "relational_model/record.h"
#include "storage/page.h"
#include "system/system.h"

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
}

bool HeapFilePage::try_insert_record(const Record& record, RID* out_record_id) {
  // TODO: implement
  return true;
}

void HeapFilePage::vacuum(const Schema& schema) {
  char* page_buf = new char[Page::SIZE];

  // TODO: implement

  delete[] page_buf;
}
