#include "b_plus_tree_leaf.h"

#include "system/system.h"

BPlusTreeLeaf::BPlusTreeLeaf(const BPlusTree& bpt, int32_t page_number)
    : bpt(bpt),
      page(buffer_mgr.get_page(bpt.leaf_file_id, page_number)) {}

BPlusTreeLeaf::BPlusTreeLeaf(const BPlusTree& bpt)
    : bpt(bpt),
      page(buffer_mgr.append_page(bpt.leaf_file_id)) {}

BPlusTreeLeaf::~BPlusTreeLeaf() {
  page.unpin();
}

std::unique_ptr<BPlusTreeSplit> BPlusTreeLeaf::insert_record(const BPlusTreeRecord& record) {
  const auto record_count = get_record_count();

  // border case, needs to be handled differently
  if (record_count == 0) {
    set_record(0, record);
    set_record_count(1);
    return nullptr;
  }
  auto index = search_index(record);

  // avoid inserting duplicated record
  if (index < max_records && get_record(index) == record) {
    return nullptr;
  }

  if (record_count < max_records) {
    // TODO: Testear Problema 1
    const auto key = search_index(record);
    for (auto i = record_count; i >= key; i--) {
      set_record(i+1, get_record(i));
    }
    set_record(key, record);
    set_record_count(record_count++);
    return nullptr
  } else {
    // TODO: Problema 2
    // 1. Encontrar el índice donde el record se inserta:
    const auto key = search_index(record);
    // 2. Definir secuencia
    vector<BPlusTreeRecord> record_seq;
    for (int32_t i = 0; i < record_count; i++){
      // Se construye la secuencia hasta el record a insertar
      record_seq.push_back(get_record(i));
    }
    // Se inserta el record dado en la función
    record_seq.insert(record_seq.begin() + key, record);
    // 3. Se crea una nueva hoja con el constructor BPlusTreeLeaf
    BPlusTreeLeaf new_leaf = BPlusTreeLeaf(bpt);
    // 4. Definición del Middle index.
    const auto middle = (max_records+1)/2;
    // 5. Actualización del N de la hoja original
    set_record_count(middle);

    for (int32_t i = middle; i < record_seq.size(); i++) {
      // Inserción de los records a la nueva hoja
      new_leaf.set_record(i - middle, record_seq[i]);
    }
    new_leaf.set_record_count((max_records/2)+1);
    new_leaf.set_next_page_number(get_next_page_number());
    // Actualización del next_ptr de la página original
    set_next_page_number(new_leaf.page.get_page_number());
    // 7. Retornar BPlusTreeSplit
    return BPlusTreeSplit(new_leaf.get_record(0).encoded_key, new_leaf.page.get_page_number());
  }
}


void BPlusTreeLeaf::delete_record(const BPlusTreeRecord& record) {
  const auto record_count = get_record_count();
  if (record_count == 0) {
    return;
  }
  // TODO: Bonus
}
push_back
int32_t BPlusTreeLeaf::get_record_count() const {
  return page.read_int32(OFFSET_RECORD_COUNT);
}

void BPlusTreeLeaf::set_record_count(int32_t record_count) {
  page.write_int32(0, record_count);
}

int32_t BPlusTreeLeaf::get_next_page_number() const {
  return page.read_int32(OFFSET_NEXT_LEAF);
}

void BPlusTreeLeaf::set_next_page_number(int32_t n) {
  page.write_int32(4, n);
}

BPlusTreeRecord BPlusTreeLeaf::get_record(int32_t idx) const {
  auto offset = OFFSET_RECORDS + idx * sizeof(BPlusTreeRecord);
  auto key = page.read_int64(offset);
  auto page_num = page.read_int32(offset + 8);
  auto dir_slot = page.read_int32(offset + 12);
  return BPlusTreeRecord(key, RID(page_num, dir_slot));
}

void BPlusTreeLeaf::set_record(int32_t idx, const BPlusTreeRecord& record) {
  auto offset = OFFSET_RECORDS + idx * sizeof(BPlusTreeRecord);
  page.write_int64(offset, record.encoded_key);
  page.write_int32(offset + 8, record.rid.page_num);
  page.write_int32(offset + 12, record.rid.dir_slot);
}

int32_t BPlusTreeLeaf::search_index(const BPlusTreeRecord& record) const {
  auto from = 0;
  auto to = get_record_count() - 1;

  while (from < to) {
    auto mid = (from + to) / 2;

    auto mid_record = get_record(mid);
    if (record < mid_record) {
      to = mid - 1;
      continue;
    } else if (mid_record < record) {
      from = mid + 1;
      continue;
    }
    // record is equal
    return mid;
  }
  // from >= to
  if (get_record(from) < record) {
    return from + 1;
  } else {
    return from;
  }
}
