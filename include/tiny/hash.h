#pragma once

#include <stddef.h>
#include <stdint.h>

namespace tiny
{

#define FORCE_INLINE inline __attribute__((always_inline))

#if defined(__has_builtin) && __has_builtin(__builtin_rotateleft32) &&         \
    __has_builtin(__builtin_rotateleft64)
#    define ROTL32 __builtin_rotateleft32
#    define ROTL64 __builtin_rotateleft64
/* Note: although _rotl exists for minGW (GCC under windows), performance seems
 * poor */
#elif defined(_MSC_VER)
#    define ROTL32(x, r) _rotl(x, r)
#    define ROTL64(x, r) _rotl64(x, r)
#else
#    define ROTL32(x, r) (((x) << (r)) | ((x) >> (32 - (r))))
#    define ROTL64(x, r) (((x) << (r)) | ((x) >> (64 - (r))))
#endif

struct FNV1a {
    static void hash_x64_64(const void *data,
        const size_t len,
        const uint32_t seed,
        void *out)
    {
        const uint64_t fnv_offset = 14695981039346656037ULL;
        const uint64_t fnv_prime = 1099511628211ULL;

        uint64_t hash = fnv_offset;
        const uint8_t *ptr = static_cast<const uint8_t *>(data);
        for (size_t i = 0; i < len; ++i) {
            hash ^= ptr[i];
            hash *= fnv_prime;
        }
        *(uint64_t *)out = hash;
    }
};

struct MurmurHash3 {
    // Finalization mix - force all bits of a hash block to avalanche
    FORCE_INLINE static uint32_t fmix32(uint32_t h)
    {
        h ^= h >> 16;
        h *= 0x85ebca6b;
        h ^= h >> 13;
        h *= 0xc2b2ae35;
        h ^= h >> 16;

        return h;
    }

    FORCE_INLINE static uint64_t fmix64(uint64_t k)
    {
        k ^= k >> 33;
        k *= 0xff51afd7ed558ccdULL;
        k ^= k >> 33;
        k *= 0xc4ceb9fe1a85ec53ULL;
        k ^= k >> 33;

        return k;
    }

    static void
    hash_x86_32(const void *key, const int len, uint32_t seed, void *out)
    {
        const uint8_t *data = (const uint8_t *)key;
        const int nblocks = len / 4;

        uint32_t h1 = seed;

        const uint32_t c1 = 0xcc9e2d51;
        const uint32_t c2 = 0x1b873593;

        // body

        const uint32_t *blocks = (const uint32_t *)(data + nblocks * 4);

        for (int i = -nblocks; i; i++) {
            uint32_t k1 = blocks[i];

            k1 *= c1;
            k1 = ROTL32(k1, 15);
            k1 *= c2;

            h1 ^= k1;
            h1 = ROTL32(h1, 13);
            h1 = h1 * 5 + 0xe6546b64;
        }

        // tail

        const uint8_t *tail = (const uint8_t *)(data + nblocks * 4);

        uint32_t k1 = 0;

        switch (len & 3) {
            case 3: k1 ^= tail[2] << 16;
            case 2: k1 ^= tail[1] << 8;
            case 1:
                k1 ^= tail[0];
                k1 *= c1;
                k1 = ROTL32(k1, 15);
                k1 *= c2;
                h1 ^= k1;
        };

        // finalization

        h1 ^= len;

        h1 = fmix32(h1);

        *(uint32_t *)out = h1;
    }

    static void
    hash_x86_128(const void *key, const int len, uint32_t seed, void *out)
    {
        const uint8_t *data = (const uint8_t *)key;
        const int nblocks = len / 16;

        uint32_t h1 = seed;
        uint32_t h2 = seed;
        uint32_t h3 = seed;
        uint32_t h4 = seed;

        const uint32_t c1 = 0x239b961b;
        const uint32_t c2 = 0xab0e9789;
        const uint32_t c3 = 0x38b34ae5;
        const uint32_t c4 = 0xa1e38b93;

        // body

        const uint32_t *blocks = (const uint32_t *)(data + nblocks * 16);

        for (int i = -nblocks; i; i++) {
            uint32_t k1 = blocks[i * 4 + 0];
            uint32_t k2 = blocks[i * 4 + 1];
            uint32_t k3 = blocks[i * 4 + 2];
            uint32_t k4 = blocks[i * 4 + 3];

            k1 *= c1;
            k1 = ROTL32(k1, 15);
            k1 *= c2;
            h1 ^= k1;

            h1 = ROTL32(h1, 19);
            h1 += h2;
            h1 = h1 * 5 + 0x561ccd1b;

            k2 *= c2;
            k2 = ROTL32(k2, 16);
            k2 *= c3;
            h2 ^= k2;

            h2 = ROTL32(h2, 17);
            h2 += h3;
            h2 = h2 * 5 + 0x0bcaa747;

            k3 *= c3;
            k3 = ROTL32(k3, 17);
            k3 *= c4;
            h3 ^= k3;

            h3 = ROTL32(h3, 15);
            h3 += h4;
            h3 = h3 * 5 + 0x96cd1c35;

            k4 *= c4;
            k4 = ROTL32(k4, 18);
            k4 *= c1;
            h4 ^= k4;

            h4 = ROTL32(h4, 13);
            h4 += h1;
            h4 = h4 * 5 + 0x32ac3b17;
        }

        // tail

        const uint8_t *tail = (const uint8_t *)(data + nblocks * 16);

        uint32_t k1 = 0;
        uint32_t k2 = 0;
        uint32_t k3 = 0;
        uint32_t k4 = 0;

        switch (len & 15) {
            case 15: k4 ^= tail[14] << 16;
            case 14: k4 ^= tail[13] << 8;
            case 13:
                k4 ^= tail[12] << 0;
                k4 *= c4;
                k4 = ROTL32(k4, 18);
                k4 *= c1;
                h4 ^= k4;

            case 12: k3 ^= tail[11] << 24;
            case 11: k3 ^= tail[10] << 16;
            case 10: k3 ^= tail[9] << 8;
            case 9:
                k3 ^= tail[8] << 0;
                k3 *= c3;
                k3 = ROTL32(k3, 17);
                k3 *= c4;
                h3 ^= k3;

            case 8: k2 ^= tail[7] << 24;
            case 7: k2 ^= tail[6] << 16;
            case 6: k2 ^= tail[5] << 8;
            case 5:
                k2 ^= tail[4] << 0;
                k2 *= c2;
                k2 = ROTL32(k2, 16);
                k2 *= c3;
                h2 ^= k2;

            case 4: k1 ^= tail[3] << 24;
            case 3: k1 ^= tail[2] << 16;
            case 2: k1 ^= tail[1] << 8;
            case 1:
                k1 ^= tail[0] << 0;
                k1 *= c1;
                k1 = ROTL32(k1, 15);
                k1 *= c2;
                h1 ^= k1;
        };

        // finalization

        h1 ^= len;
        h2 ^= len;
        h3 ^= len;
        h4 ^= len;

        h1 += h2;
        h1 += h3;
        h1 += h4;
        h2 += h1;
        h3 += h1;
        h4 += h1;

        h1 = fmix32(h1);
        h2 = fmix32(h2);
        h3 = fmix32(h3);
        h4 = fmix32(h4);

        h1 += h2;
        h1 += h3;
        h1 += h4;
        h2 += h1;
        h3 += h1;
        h4 += h1;

        ((uint32_t *)out)[0] = h1;
        ((uint32_t *)out)[1] = h2;
        ((uint32_t *)out)[2] = h3;
        ((uint32_t *)out)[3] = h4;
    }

    static void
    hash_x64_128(const void *key, const int len, const uint32_t seed, void *out)
    {
        const uint8_t *data = (const uint8_t *)key;
        const int nblocks = len / 16;

        uint64_t h1 = seed;
        uint64_t h2 = seed;

        const uint64_t c1 = 0x87c37b91114253d5ULL;
        const uint64_t c2 = 0x4cf5ad432745937fULL;

        // body

        const uint64_t *blocks = (const uint64_t *)(data);

        for (int i = 0; i < nblocks; i++) {
            uint64_t k1 = blocks[i * 2 + 0];
            uint64_t k2 = blocks[i * 2 + 1];

            k1 *= c1;
            k1 = ROTL64(k1, 31);
            k1 *= c2;
            h1 ^= k1;

            h1 = ROTL64(h1, 27);
            h1 += h2;
            h1 = h1 * 5 + 0x52dce729;

            k2 *= c2;
            k2 = ROTL64(k2, 33);
            k2 *= c1;
            h2 ^= k2;

            h2 = ROTL64(h2, 31);
            h2 += h1;
            h2 = h2 * 5 + 0x38495ab5;
        }

        // tail

        const uint8_t *tail = (const uint8_t *)(data + nblocks * 16);

        uint64_t k1 = 0;
        uint64_t k2 = 0;

        switch (len & 15) {
            case 15: k2 ^= ((uint64_t)tail[14]) << 48;
            case 14: k2 ^= ((uint64_t)tail[13]) << 40;
            case 13: k2 ^= ((uint64_t)tail[12]) << 32;
            case 12: k2 ^= ((uint64_t)tail[11]) << 24;
            case 11: k2 ^= ((uint64_t)tail[10]) << 16;
            case 10: k2 ^= ((uint64_t)tail[9]) << 8;
            case 9:
                k2 ^= ((uint64_t)tail[8]) << 0;
                k2 *= c2;
                k2 = ROTL64(k2, 33);
                k2 *= c1;
                h2 ^= k2;

            case 8: k1 ^= ((uint64_t)tail[7]) << 56;
            case 7: k1 ^= ((uint64_t)tail[6]) << 48;
            case 6: k1 ^= ((uint64_t)tail[5]) << 40;
            case 5: k1 ^= ((uint64_t)tail[4]) << 32;
            case 4: k1 ^= ((uint64_t)tail[3]) << 24;
            case 3: k1 ^= ((uint64_t)tail[2]) << 16;
            case 2: k1 ^= ((uint64_t)tail[1]) << 8;
            case 1:
                k1 ^= ((uint64_t)tail[0]) << 0;
                k1 *= c1;
                k1 = ROTL64(k1, 31);
                k1 *= c2;
                h1 ^= k1;
        };

        // finalization

        h1 ^= len;
        h2 ^= len;

        h1 += h2;
        h2 += h1;

        h1 = fmix64(h1);
        h2 = fmix64(h2);

        h1 += h2;
        h2 += h1;

        ((uint64_t *)out)[0] = h1;
        ((uint64_t *)out)[1] = h2;
    }
};

template <typename Out, typename In>
struct hash_base {
    typedef Out result_type;
    typedef In argument_type;
};

template <typename T>
struct hash {
};

// Explicit specialization for pointer types.
template <typename T>
struct hash<T *> : public hash_base<size_t, T *> {
    size_t operator()(const T *key) const noexcept
    {
        return reinterpret_cast<size_t>(key);
    }
};

// Explicit specializations for integer types.
#define _Cxx_hashtable_define_trivial_hash(_Tp)                                \
    template <>                                                                \
    struct hash<_Tp> : public hash_base<size_t, _Tp> {                         \
        size_t operator()(_Tp val) const noexcept                              \
        {                                                                      \
            return static_cast<size_t>(val);                                   \
        }                                                                      \
    }

_Cxx_hashtable_define_trivial_hash(bool);
_Cxx_hashtable_define_trivial_hash(char);
_Cxx_hashtable_define_trivial_hash(signed char);
_Cxx_hashtable_define_trivial_hash(unsigned char);
_Cxx_hashtable_define_trivial_hash(wchar_t);
_Cxx_hashtable_define_trivial_hash(short);
_Cxx_hashtable_define_trivial_hash(unsigned short);
_Cxx_hashtable_define_trivial_hash(int);
_Cxx_hashtable_define_trivial_hash(unsigned int);
_Cxx_hashtable_define_trivial_hash(long);
_Cxx_hashtable_define_trivial_hash(unsigned long);
_Cxx_hashtable_define_trivial_hash(long long);
_Cxx_hashtable_define_trivial_hash(unsigned long long);

#undef _Cxx_hashtable_define_trivial_hash

template <>
struct hash<float> : public hash_base<size_t, float> {
    size_t operator()(float val) const noexcept
    {
        uint64_t h1[2];
        // 0 and -0 are treated as the same value and hash to 0
        return val == 0.0f ?
                   0 :
                   (MurmurHash3::hash_x64_128(&val, sizeof(val), 0, &h1),
                       h1[0]);
    }
};

template <>
struct hash<double> : public hash_base<size_t, double> {
    size_t operator()(double val) const noexcept
    {
        uint64_t h1[2];
        // 0 and -0 are treated as the same value and hash to 0
        return val == 0.0 ?
                   0 :
                   (MurmurHash3::hash_x64_128(&val, sizeof(val), 0, &h1),
                       h1[0]);
    }
};

template <>
struct hash<long double> : public hash_base<size_t, long double> {
    size_t operator()(long double val) const noexcept
    {
        uint64_t h1[2];
        // 0 and -0 are treated as the same value and hash to 0
        return val == 0.0 ?
                   0 :
                   (MurmurHash3::hash_x64_128(&val, sizeof(val), 0, &h1),
                       h1[0]);
    }
};

template <>
struct hash<std::nullptr_t> : public hash_base<size_t, std::nullptr_t> {
    // Hash of nullptr is 0
    size_t operator()(std::nullptr_t) const noexcept { return 0; }
};

}  // namespace tiny