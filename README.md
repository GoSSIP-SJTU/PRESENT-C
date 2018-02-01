PRESENT is an ultra-lightweight block cipher well suitable for extremely constrained environments such as RFID tags and sensor networks [1]. Here we give a fast and compact software implementation of PRESENT. The program is written in plain C, which is easy to be changed to work on various platforms.

This implementation of Present adds decryption interface to the original version of Bo Zhu [2].

## License

BSD license

## Usage

There are two interfaces:

1. The full-round encryption function:
```c
    void present_encrypt(const uint8_t *plain, const uint8_t *key, uint8_t *cipher)
```
Before using this function, the plaintext should have already been placed in the array `plain`, and the 80-bit secret key should also be arranged at `key`. After the encryption is done, the ciphertext will be computed and copied into `cipher`. Please note that the areas identified by `plain` and `cipher` can overlap with each other (also `key` and `cipher`).

2. The full-round decryption function:
```c
    void present_decrypt(const uint8_t *cipher, const uint8_t *key, uint8_t *plain)
```
Before using this function, the ciphertext should have already been placed in the array `cipher`, and the 80-bit secret key should also be arranged at `key`. After the encryption is done, the plaintext will be computed and copied into `plain`. Please note that the areas identified by `plain` and `cipher` can overlap with each other (also `key` and `cipher`).


## References

1. A. Bogdanov, L. R. Knudsen, G. Leander, C. Paar, A. Poschmann, M. J. B. Robshaw, Y. Seurin and C. Vikkelsoe. PRESENT: An Ultra-Lightweight Block Cipher. CHES 2007.
2. https://github.com/bozhu/PRESENT-C
