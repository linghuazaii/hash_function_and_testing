#ifndef _HASH_H
#define _HASH_H
/**
 * File: hash.h
 * Descriptions: hash functions and it's score
 * Author: Charles,Liu. 2017-7-10
 * Mailto: charlesliu.cn.bj@gmail.com
 */
#include <inttypes.h>

/**
 * 1000 * 1000 times, conflicts: 14506
 * 1 - 14506 / 1000000 = %98.5494
 * rating: %98.5494
 */
uint32_t djb2_hash(const char *str, uint32_t length) {
    uint32_t hval = 5381;
    for (int i = 0; i < length; ++i) {
        hval = ((hval << 5) + hval) + int(str[i]);
    }
    return hval;
}

/**
 * 1000000 times, conflicts: 104
 * 1 - 104 / 1000000 = %99.9896
 * rating: %99.9896
 */
uint32_t sdbm_hash(const char *str, uint32_t length) {
    uint32_t hval = 0;
    for (int i = 0; i < length; ++i) 
        hval = (int)str[i] + (hval << 6) + (hval << 16) - hval;
    return hval;
}

/**
 * 1000000 times, conflicts: 971028
 * 1 - 971028 / 1000000 = %2.8971
 * rating: %2.8971
 */
uint32_t stupid_hash(const char *str, uint32_t length) {
    uint32_t hval = 0;
    for (int i = 0; i < length; ++i)
        hval += (int)str[i];
    return hval;
}

/**
 * 1000000 times, conflicts: 210
 * 1 - 210.0 / 1000000 = %99.979
 * rating: %99.979
 * It seems that sdbm is good than this
 */
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                       +(uint32_t)(((const uint8_t *)(d))[0]) )
uint32_t SuperFastHash (const char *data, uint32_t len) {
    uint32_t hash = len, tmp;
    int rem;

    if (len <= 0 || data == NULL) return 0;

    rem = len & 3;
    len >>= 2;

    /* Main loop */
    for (;len > 0; len--) {
        hash  += get16bits (data);
        tmp    = (get16bits (data+2) << 11) ^ hash;
        hash   = (hash << 16) ^ tmp;
        data  += 2*sizeof (uint16_t);
        hash  += hash >> 11;
    }

    /* Handle end cases */
    switch (rem) {
        case 3: hash += get16bits (data);
                hash ^= hash << 16;
                hash ^= ((signed char)data[sizeof (uint16_t)]) << 18;
                hash += hash >> 11;
                break;
        case 2: hash += get16bits (data);
                hash ^= hash << 11;
                hash += hash >> 17;
                break;
        case 1: hash += (signed char)*data;
                hash ^= hash << 10;
                hash += hash >> 1;
    }

    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}

/**
 * 1000000 times, conflicts: 3536
 * 1 - 3536 / 1000000 = %99.6464
 * rating: %99.6464
 */
#define FNV_32_PRIME ((uint32_t)0x01000193)
uint32_t fnv_hash(const char *str, uint32_t length, uint32_t sault = 0) {
    uint32_t hval = sault;
    for (int i = 0; i < length; ++i) {
        hval *= FNV_32_PRIME;
        hval += (hval << 1) + (hval << 4) + (hval << 8) + (hval << 24);
        hval ^= (int)str[i];
    }
    return hval;
}

/**
 * 1000000 times, conflicts: 166
 * rating: %99.9834
 */
uint32_t jenkins_hash(const char *str, uint32_t length) {
    uint32_t hval = 0;
    for (int i  = 0; i < length; ++i) {
        hval += (int)str[i];
        hval += hval << 10;
        hval ^= hval >> 6;
    }
    hval += hval << 3;
    hval ^= hval >> 11;
    hval += hval << 15;
    return hval;
}

/**
 * 1000000 times, conflicts: 26549
 * rating: %97.3471
 */
uint32_t pjw_hash(const char *str, uint32_t length) {
    const uint32_t bits_in_unsigned_int = 32;
    const uint32_t three_quarters = 32 * 3 / 4;
    const uint32_t one_eight = 32 / 8;
    const uint32_t high_bits = (uint32_t)(-1) << (bits_in_unsigned_int - one_eight);

    uint32_t hval = 0;
    uint32_t test = 0;
    for (int i = 0; i < length; ++i) {
        hval = (hval << one_eight) + (int)str[i];
        if ((test = hval & high_bits) != 0)
            hval = ((hval ^ (test > three_quarters)) & (~high_bits));
    }   
    return hval;
}

/**
 * 1000000 times, conflicts: 25090
 * rating: %97.4910
 */
uint32_t elf_hash(const char *str, uint32_t length) {
    uint32_t hval = 0, high;
    for (int i = 0; i < length; ++i) {
        hval = (hval << 4) + (int)str[i];
        if ((high = hval & 0xF0000000) != 0)
            hval ^= high >> 24;
        hval &= ~high;
    }
    return hval;
}

/**
 * 1000000 times, conflicts: 24334
 * rating: %97.5666
 */
uint32_t murmur3_hash(const char *str, uint32_t length, uint32_t seed = 0) {
    uint32_t hval = seed;
    if (length > 3) {
        const uint32_t *key_x4 = (const uint32_t *)str;
        size_t i = length >> 2;
        do {
            uint32_t k = *key_x4++;
            k *= 0xcc9e2d51;
            k = (k << 15) | (k >> 17);
            k *= 0x1b873593;
            hval ^= k;
            hval = (hval << 13) | (hval >> 19);
            hval = (hval << 2) + hval + 0xe6546b64;
        } while (--i);
        str = (const char *)key_x4;
    }
    if (length & 3) {
        size_t i = length & 3;
        uint32_t k = 0;
        str = &str[i - 1];
        do {
            k <<= 8;
            k |= (int)(*str--);
        } while (--i);
        k *= 0xcc9e2d51;
        k = (k << 15) | (k >> 17);
        k *= 0x1b873593;
        hval ^= k;
    }
    hval ^= length;
    hval ^= hval >> 16;
    hval *= 0x85ebca6b;
    hval ^= hval >> 13;
    hval *= 0xc2b2ae35;
    hval ^= hval >> 16;
    return hval;
}

/**
 * 1000000 times, conflicts: 3642
 * rating: %99.6358
 */
uint32_t bkdr_hash(const char *str, uint32_t length) {
    uint32_t seed = 131;
    uint32_t hval = 0;
    for (int i = 0; i < length; ++i)
        hval = (hval * seed) + (int)str[i];
    return hval;
}

#endif
