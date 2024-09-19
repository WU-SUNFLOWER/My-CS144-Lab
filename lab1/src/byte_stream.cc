#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

bool Writer::is_closed() const
{
  return is_closed_;
}

void Writer::push( string data )
{
  uint64_t cur_capacity = available_capacity();
  if (is_closed_ || cur_capacity == 0 || data.empty()) {
    return;
  }
  if (cur_capacity < data.size()) {
    data.resize(cur_capacity);
  }
  bytes_buffered_ += data.size();
  bytes_pushed_ += data.size();
  stream_.emplace(std::move(data));
}

void Writer::close()
{
  is_closed_ = true;
}

uint64_t Writer::available_capacity() const
{
  return capacity_ - bytes_buffered_;
}

uint64_t Writer::bytes_pushed() const
{
  return bytes_pushed_;
}

bool Reader::is_finished() const
{
  return is_closed_ && bytes_buffered_ == 0;
}

uint64_t Reader::bytes_popped() const
{
  return bytes_popped_;
}

string_view Reader::peek() const
{
  return stream_.empty() ? 
    string_view {} 
    : string_view {stream_.front()}.substr(bytes_remove_prefix_);
}

void Reader::pop( uint64_t len )
{
  while (len != 0U && bytes_buffered_ != 0U && !stream_.empty()) {
      uint64_t front_size = stream_.front().size() - bytes_remove_prefix_;
      if (len < front_size) {
        bytes_remove_prefix_ += len;
        bytes_buffered_ -= len;
        bytes_popped_ += len;
        break;
      }
      stream_.pop();
      bytes_remove_prefix_ = 0;
      bytes_buffered_ -= front_size;
      bytes_popped_ += front_size;
      len -= front_size;
  }
}

uint64_t Reader::bytes_buffered() const
{
  return bytes_buffered_;
}
