/**
 *  @file Buffer.hpp
 *  @author Matteo Loporchio
 */

#include "Buffer.hpp"

/**
 *
 */
Buffer::Buffer() {}

/**
 *
 */
const uint8_t *Buffer::data() const {
  return buf.data();
}

/**
 *
 */
size_t Buffer::size() const {
  return buf.size();
}

/**
 *
 */
Buffer &Buffer::put_bytes(uint8_t *data, size_t length) {
  if (data) {
    for (size_t i = 0; i < length; i++) buf.push_back(data[i]);
  }
  return *this;
}
