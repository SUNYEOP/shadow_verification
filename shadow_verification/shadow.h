#ifndef _SHADOW_H_
#define _SHADOW_H_

#define _DEVELOPMENT


#if defined _MSC_VER
//Visual Studio
#ifdef _DEVELOPMENT
#define DEV_DEFINE __declspec(dllexport)
#else
#define DEV_DEFINE __declspec(dllimport)
#endif
#elif defined __GNUC__
//GCC
#ifdef _DEVELOPMENT
#define DEV_DEFINE __attribute__ ((visibility("default")))
#else
	//nothing to define
#define DEV_DEFINE 
#endif
#endif
#include <immintrin.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define ROL8(x,n) (((x) >> (8 - (n))) | ((x) << (n)))
#define ROR8(x,n) (((x) << (8 - (n))) | ((x) >> (n)))
typedef uint32_t        word_t;
#define WORD_BSZ        32
#define WORD_MSB        0x80000000
#define WORD_MASK       0xFFFFFFFF
typedef uint32_t        idx_t;
#define IDX_BSZ         32
#define IDX_MASK        0xFFFFFFFF
#define _NUM_ROUNDS 14
#define IV_WSZ 1
#define KEY_WSZ 2
#define FAIL  -1
#define PASS   0
typedef uint64_t        num_t;
typedef uint64_t        mask_t;
typedef int             flag_t;
typedef unsigned int    uint_t;
typedef int             error_t;

#define F(A) (uint8_t)((ROL8(A, 1) & ROL8(A, 7)) ^ ROL8(A, 2))
#define TEST_NUM 200
#define DEC_ROUND 4
DEV_DEFINE void shadow32_round(uint8_t* l0, uint8_t* l1, uint8_t* r0, uint8_t* r1, uint8_t kl0, uint8_t kl1, uint8_t kr0, uint8_t kr1);
DEV_DEFINE void shadow32_key_round(uint64_t* key_state, uint32_t round);
DEV_DEFINE word_t cipher(word_t key0, word_t key1, word_t iv, int round);


#endif /*_SHADOW_H_*/