#ifndef PRESENT_H
#define PRESENT_H

#include <stdint.h>

// Full-round should be 31, i.e. rounds = 31
// Plain and cipher can overlap, so do key and cipher
void present_rounds(const uint8_t *plain, const uint8_t *key, const uint8_t rounds, uint8_t *cipher);
#define present_encrypt(plain, key, cipher) present_rounds((plain), (key), 31, (cipher))

// full-round should be 31, i.e. rounds = 31
// plain and cipher can overlap, so do key and cipher
void present_decrypt_rounds(const uint8_t *cipher, const uint8_t *key, const uint8_t rounds, uint8_t *plain);
#define present_decrypt(cipher, key, plain) present_decrypt_rounds((cipher), (key), 31, (plain))

#endif /* PRESENT_H */
