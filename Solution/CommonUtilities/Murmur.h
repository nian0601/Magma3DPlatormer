//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

#ifndef _MURMURHASH3_H_
#define _MURMURHASH3_H_

//-----------------------------------------------------------------------------
// Platform-specific functions and macros

// Microsoft Visual Studio

#if defined(_MSC_VER) && (_MSC_VER < 1600)

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned __int64 uint64_t;

// Other compilers

#else	// defined(_MSC_VER)

#include <stdint.h>
#endif // !defined(_MSC_VER)

//-----------------------------------------------------------------------------
void MurmurHash3_x86_32(const void * key, int len, uint32_t seed, void * out);

//-----------------------------------------------------------------------------

#endif // _MURMURHASH3_H_
































//#pragma once
//
//#include <algorithm>
//
//namespace Murmur
//{
//	//**********************************
//	//** MURMUR SPECIFIC CODE, LET IT BE
//	//**********************************
//	typedef unsigned int       uint32_t;
//	typedef unsigned char      uint8_t;
//
//	__forceinline uint32_t getblock32(const uint32_t * p, int i)
//	{
//		return p[i];
//	}
//
//	__forceinline uint32_t fmix32(uint32_t h)
//	{
//		h ^= h >> 16;
//		h *= 0x85ebca6b;
//		h ^= h >> 13;
//		h *= 0xc2b2ae35;
//		h ^= h >> 16;
//
//		return h;
//	}
//
//#define ROTL32(x,y)     _rotl(x,y)
//	//**********************************
//	//** MURMUR SPECIFIC CODE, LET IT BE
//	//**********************************
//
//	inline unsigned int Hash(const char* aKey)
//	{
//		const uint8_t * data = (const uint8_t*)aKey;
//		const int nblocks = strlen(aKey) / 4;
//
//		uint32_t h1 = 0;
//
//		const uint32_t c1 = 0xcc9e2d51;
//		const uint32_t c2 = 0x1b873593;
//
//		//----------
//		// body
//
//		const uint32_t * blocks = (const uint32_t *)(data + nblocks * 4);
//
//		for (int i = -nblocks; i; i++)
//		{
//			uint32_t k1 = getblock32(blocks, i);
//
//			k1 *= c1;
//			k1 = ROTL32(k1, 15);
//			k1 *= c2;
//
//			h1 ^= k1;
//			h1 = ROTL32(h1, 13);
//			h1 = h1 * 5 + 0xe6546b64;
//		}
//
//		//----------
//		// tail
//
//		const uint8_t * tail = (const uint8_t*)(data + nblocks * 4);
//
//		uint32_t k1 = 0;
//
//		switch (strlen(aKey) & 3)
//		{
//		case 3: k1 ^= tail[2] << 16;
//		case 2: k1 ^= tail[1] << 8;
//		case 1: k1 ^= tail[0];
//			k1 *= c1; k1 = ROTL32(k1, 15); k1 *= c2; h1 ^= k1;
//		};
//
//		//----------
//		// finalization
//
//		h1 ^= strlen(aKey);
//
//		h1 = fmix32(h1);
//
//
//		return (uint32_t)h1;
//	}
//
//	unsigned int Hash2(const void * key, int len, unsigned int seed)
//	{
//		// 'm' and 'r' are mixing constants generated offline.
//		// They're not really 'magic', they just happen to work well.
//
//		const unsigned int m = 0x5bd1e995;
//		const int r = 24;
//
//		// Initialize the hash to a 'random' value
//
//		unsigned int h = seed ^ len;
//
//		// Mix 4 bytes at a time into the hash
//
//		const unsigned char * data = (const unsigned char *)key;
//
//		while (len >= 4)
//		{
//			unsigned int k = *(unsigned int *)data;
//
//			k *= m;
//			k ^= k >> r;
//			k *= m;
//
//			h *= m;
//			h ^= k;
//
//			data += 4;
//			len -= 4;
//		}
//
//		// Handle the last few bytes of the input array
//
//		switch (len)
//		{
//		case 3: h ^= data[2] << 16;
//		case 2: h ^= data[1] << 8;
//		case 1: h ^= data[0];
//			h *= m;
//		};
//
//		// Do a few final mixes of the hash to ensure the last few
//		// bytes are well-incorporated.
//
//		h ^= h >> 13;
//		h *= m;
//		h ^= h >> 15;
//
//		return h;
//	}
//
//}