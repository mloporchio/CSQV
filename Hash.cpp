/**
 *  @file Hash.cpp
 *  @author Matteo Loporchio
 */

#include "Hash.hpp"
#include <cassert>
#include <iomanip>
#include <sstream>

/**
 *  Computes the SHA-256 digest of a generic buffer.
 *  @param buf pointer to the buffer
 *  @param size length of the buffer
 *  @return the digest of the buffer
 */
hash_t sha256(const uint8_t *buf, size_t size) {
  // Check if the buffer reference is not null.
  assert(buf != NULL);
  hash_t h;
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, buf, size);
  SHA256_Final(h.data(), &sha256);
  return h;
}

/**
 *
 */
hash_t sha256(const Buffer &buf) {
  return sha256(buf.data(), buf.size());
}

/**
 *  Computes the SHA-256 digest of a string.
 *  @param s string
 *  @return a digest of the string
 */
hash_t sha256(const std::string &s) {
  return sha256((const uint8_t *) s.c_str(), s.size());
}

/**
 *  Converts an array of bytes to a human-readable hexadecimal string.
 *  @param hash array of bytes
 *  @return a readable string representing the content of the array
 */
std::string toHex(hash_t h) {
  std::stringstream ss;
  for (size_t i = 0; i < h.size(); i++)
    ss << std::hex << std::setw(2) << std::setfill('0') << (int) h[i];
  return ss.str();
}
