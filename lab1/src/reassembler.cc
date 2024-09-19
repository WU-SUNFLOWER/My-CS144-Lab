#include "reassembler.hh"
#include <algorithm>
#include <cassert>

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  Writer& writer = output_.writer();
  uint64_t limit_handled_index = handled_index_ + writer.available_capacity();
  uint64_t start_index = std::max(first_index, handled_index_);
  uint64_t end_index = std::min(first_index + data.size(), limit_handled_index);

#define map_index(i) ((i) % total_capacity_)

  uint64_t mapped_index;
  for (uint64_t i = start_index; mapped_index = map_index(i), i < end_index; ++i) {
    if (bitmap_.count(mapped_index) > 0) {
      continue;
    }
    bitmap_.insert(mapped_index);
    internal_buffer_[mapped_index] = data[i - first_index];
    ++bytes_pending_;
  }

  std::string to_be_writed;
  while (
    bitmap_.count(mapped_index = map_index(handled_index_)) > 0
    && handled_index_ < limit_handled_index
  ) {
    bitmap_.erase(mapped_index);
    to_be_writed += internal_buffer_[mapped_index];
    --bytes_pending_;
    ++handled_index_;
  }
  writer.push(to_be_writed);

#undef map_index

  if (is_last_substring) {
    eof_index_ = first_index + data.size() + 1;
  }
  if (handled_index_ + 1 == eof_index_) {
    writer.close();
  }
}

uint64_t Reassembler::bytes_pending() const
{
  return bytes_pending_;
}