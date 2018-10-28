#include "present64.h"
#include <string.h>

static inline void key_expand(const u1 * key, PresKey & rk) 
{
	// actually is (sbox[] << 4)
	const u1 sbox[16] = { 0xC0, 0x50, 0x60, 0xB0, 0x90, 0x00, 0xA0, 0xD0, 0x30, 0xE0, 0xF0, 0x80, 0x40, 0x70, 0x10, 0x20 };
	memcpy( rk.rKey, key, 8 );
	
	u1 tk[PRESENT_ROUNDS + 1][10];
	memcpy( tk[0], key, 10 );

	for (size_t i = 1; i <= PRESENT_ROUNDS; i++) 
	{
		tk[i][9] = tk[i - 1][6] << 5 | tk[i - 1][7] >> 3;
		tk[i][8] = tk[i - 1][5] << 5 | tk[i - 1][6] >> 3;
		tk[i][7] = tk[i - 1][4] << 5 | tk[i - 1][5] >> 3;
		tk[i][6] = tk[i - 1][3] << 5 | tk[i - 1][4] >> 3;
		tk[i][5] = tk[i - 1][2] << 5 | tk[i - 1][3] >> 3;
		tk[i][4] = tk[i - 1][1] << 5 | tk[i - 1][2] >> 3;
		tk[i][3] = tk[i - 1][0] << 5 | tk[i - 1][1] >> 3;
		tk[i][2] = tk[i - 1][9] << 5 | tk[i - 1][0] >> 3;
		tk[i][1] = tk[i - 1][8] << 5 | tk[i - 1][9] >> 3;
		tk[i][0] = tk[i - 1][7] << 5 | tk[i - 1][8] >> 3;

		tk[i][0] = (tk[i][0] & 0x0F) | sbox[tk[i][0] >> 4];

		tk[i][7] ^= i >> 1;
		tk[i][8] ^= i << 7;
		
		memcpy( rk.rKey + i, tk[i], 8 );
	}
}

static inline u8 SP(u8 state) 
{
	u1 * s = (u1 * )&state;

	return T0[s[0]] | T1[s[1]] | T2[s[2]] | T3[s[3]] | T4[s[4]] | T5[s[5]] | T6[s[6]] | T7[s[7]];
}

static inline u8 P_inv(u8 state) 
{
	u1 * s = (u1 * )&state;

	return Tinv0[s[0]] | Tinv1[s[1]] | Tinv2[s[2]] | Tinv3[s[3]] | Tinv4[s[4]] | Tinv5[s[5]] | Tinv6[s[6]] | Tinv7[s[7]];
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

u8 present_enc(const u8 plain, PresKey k )
{
	u8 state = plain;

	for ( size_t i = 0; i < PRESENT_ROUNDS; ++i ) 
	{
		state ^= k.rKey[i];
		state = SP(state);
	}
	
	return state ^ k.rKey[PRESENT_ROUNDS];
}

u8 present_dec(const u8 cipher, PresKey k )
{
	u8 state = cipher;
	u1 * s = (u1 * )&state;

	for ( size_t i = 0; i < PRESENT_ROUNDS; ++i ) 
	{
		state ^= k.rKey[PRESENT_ROUNDS - i];
		state = P_inv(state);
		for ( size_t k = 0; k < 8; ++k )
			s[k] = SboxInv[s[k]];
	}
	return state ^ k.rKey[0];	
}

#define PRESENT_TEST
#ifdef PRESENT_TEST
#include <stdio.h>
#include <liarod.h>
void test_vector()
{
	u8 input = 0;
	u8 output = 0;
	
	u1 key[10];
	memset(key, 0x55, 10);
	PresKey rk;
	key_expand(key, rk);

	output = present_enc(input, rk);
	printf("%016llx\n", output); // should be af427b81cf6f391f
	
	output = present_dec(output, rk);
	printf("%016llx\n", output);	
}

#include <time.h>

int main()
{
	test_vector();

	u8 input = 0;
	u1 key[10];
	memset(key, 0x44, 10);
	PresKey rk;
	key_expand(key, rk);

	clock_t t = clock();
	
	forloop (i, 0, 10000000)
	{
		input = present_enc(input, rk);
	}
	t = clock() - t;
	
	printf("I/O: %f MB/s\n", 80 / (1.0 * t / 1000) );
}
#endif