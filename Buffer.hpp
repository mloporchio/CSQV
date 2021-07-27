/**
 *  @file Buffer.hpp
 *  @author Matteo Loporchio
 */

#ifndef BUFFER_H
#define BUFFER_H

#include <vector>

/**
 *  This class implements a buffer containing raw bytes.
 */
class Buffer {
private:
  std::vector<uint8_t> buf; ///< The vector where bytes are actually stored.
public:
  /**
   *  Default class constructor.
   */
  Buffer();

  /**
   *  Class constructor that initializes the buffer with a given capacity.
   *  @param capacity the initial buffer capacity
   */
  Buffer(size_t capacity);

  /**
   *  Returns a pointer to the underlying array.
   *  @return a pointer to the underlying array
   */
  const uint8_t *data() const;

  /**
   *  Returns the current size of the buffer.
   *  @return the buffer size
   */
  size_t size() const;

  /**
   *  Inserts a sequence of bytes into the buffer.
   *  @param data pointer to the sequence of bytes
   *  @param length number of bytes in the sequence
   *  @return a reference to the updated buffer
   */
  Buffer &put_bytes(uint8_t *data, size_t length);

  /**
   *  Inserts a generic element into the buffer.
   *  The element is interpreted as a sequence of raw bytes.
   *  @param x the element
   *  @return a reference to the updated buffer
   */
  template<typename T>
  Buffer &put(const T &x) {
    const uint8_t *ptr = reinterpret_cast<const uint8_t*>(&x);
    for (size_t i = 0; i < sizeof(T); i++) buf.push_back(ptr[i]);
    return *this;
  }

};

#endif
