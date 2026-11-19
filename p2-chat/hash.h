#ifndef __cs361_hash_h__
#define __cs361_hash_h__

#include <stddef.h>

// Hash helpers for the Advanced (A) authentication requirement.
// The hash is SHA-256, written out as 64 lowercase hex characters.
//
// Both functions keep no global state, so any number of threads may
// call them at the same time without a mutex.
//
// Example (client answering a CHALLENGE):
//
//   char input[512];
//   char hash[HASH_BUF_SIZE];
//   snprintf (input, sizeof input, "%s%s", challenge, secret);
//   hash_string (input, hash);
//   // now send "RESPONSE <hash>\r\n" to the server

// Number of hex characters in a hash, not counting the '\0'.
#define HASH_HEX_LEN 64

// Size of a char buffer that can hold a hash as a C string.
#define HASH_BUF_SIZE (HASH_HEX_LEN + 1)

// Hashes the C string input (up to, not including, its '\0').
// out must point to at least HASH_BUF_SIZE chars; it receives the
// 64 hex characters followed by '\0'.
void hash_string (const char *input, char *out);

// Hashes exactly len bytes starting at data (the bytes may include '\0').
// out works the same way as for hash_string.
void hash_bytes (const void *data, size_t len, char *out);

#endif
