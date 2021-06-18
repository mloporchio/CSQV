/**
 *  @file Hash.hpp
 *  @author Matteo Loporchio
 */

#ifndef HASH_H
#define HASH_H

#include "Buffer.hpp"
#include <array>
#include <openssl/sha.h>
#include <string>

/// A SHA-256 digest is represented as a fixed-size array of bytes.
typedef std::array<uint8_t, SHA256_DIGEST_LENGTH> hash_t;

/**
 *  Computes the SHA-256 digest of an array of raw bytes.
 *  @param buf pointer to the buffer
 *  @param size length of the buffer
 *  @return the digest of the buffer
 */
hash_t sha256(const uint8_t *buf, size_t size);

/**
 *  Computes the SHA-256 digest of the input buffer.
 *  @param buf reference to the buffer
 *  @return the digest of the bytes contained in the buffer
 */
hash_t sha256(const Buffer &buf);

/**
 *  Computes the SHA-256 digest of a string.
 *  @param s input string
 *  @return a digest of the string
 */
hash_t sha256(const std::string &s);

/**
 *  Converts an array of bytes to a human-readable hexadecimal string.
 *  @param hash array of bytes
 *  @return a readable string representing the content of the array
 */
std::string toHex(hash_t h);

#endif
