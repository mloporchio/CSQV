/**
 *  @file Buffer.hpp
 *  @author Matteo Loporchio
 */

#ifndef BUFFER_H
#define BUFFER_H

#include <vector>

/**
 *
 */
struct Buffer {
private:
  std::vector<uint8_t> buf;
public:
  /**
   *  Default class constructor.
   */
  Buffer();

  /**
   *
   */
  const uint8_t *data() const;

  /**
   *
   */
  size_t size() const;

  /**
   *
   */
  Buffer &put_bytes(uint8_t *data, size_t length);

  /**
   *
   */
  template<typename T>
  Buffer &put(T &x) {
    uint8_t *ptr = reinterpret_cast<uint8_t*>(&x);
    for (size_t i = 0; i < sizeof(T); i++) buf.push_back(ptr[i]);
    return *this;
  }
};

#endif
