/**
 *  @file Buffer.hpp
 *  @author Matteo Loporchio
 */

#include "Buffer.hpp"

/**
 *  Default class constructor.
 */
Buffer::Buffer() {/* Nothing to do here! */}

/**
 *  Returns a pointer to the underlying array.
 *  @return a pointer to the underlying array
 */
const uint8_t *Buffer::data() const {
  return buf.data();
}

/**
 *  Returns the current size of the buffer.
 *  @return the buffer size
 */
size_t Buffer::size() const {
  return buf.size();
}

/**
 *  Inserts a sequence of bytes into the buffer.
 *  @param data pointer to the sequence of bytes
 *  @param length number of bytes in the sequence
 *  @return a reference to the updated buffer
 */
Buffer &Buffer::put_bytes(uint8_t *data, size_t length) {
  if (data) {
    for (size_t i = 0; i < length; i++) buf.push_back(data[i]);
  }
  return *this;
}
