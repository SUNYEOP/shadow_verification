#include "shadow.h"

void shadow32_round(uint8_t* l0, uint8_t* l1, uint8_t* r0, uint8_t* r1, uint8_t kl0, uint8_t kl1, uint8_t kr0, uint8_t kr1)
{
    uint8_t t;

    //Left


    *l1 = (ROL8(*l0, 1) & ROL8(*l0, 7)) ^ ROL8(*l0, 2) ^ *l1 ^ kl0;
    *l0 = (ROL8(*l1, 1) & ROL8(*l1, 7)) ^ ROL8(*l1, 2) ^ *l0 ^ kl1;

    //Right
    *r1 = (ROL8(*r0, 1) & ROL8(*r0, 7)) ^ ROL8(*r0, 2) ^ *r1 ^ kr0;
    *r0 = (ROL8(*r1, 1) & ROL8(*r1, 7)) ^ ROL8(*r1, 2) ^ *r0 ^ kr1;

    //Rotate
    t = *l0;
    *l0 = *r1;
    *r1 = *r0;
    *r0 = *l1;
    *l1 = t;
}

void shadow32_dec_round(uint8_t* l0, uint8_t* l1, uint8_t* r0, uint8_t* r1, uint8_t kl0, uint8_t kl1, uint8_t kr0, uint8_t kr1)
{
    uint8_t t;

    //Left

    t = *r0;
    *r0 = (ROL8(*l0, 1) & ROL8(*l0, 7)) ^ ROL8(*l0, 2) ^ *r1 ^ kr0;
    *r1 = (ROL8(*r0, 1) & ROL8(*r0, 7)) ^ ROL8(*r0, 2) ^ *l0 ^ kr1;

    //Right
    *l0 = (ROL8(t, 1) & ROL8(t, 7)) ^ ROL8(t, 2) ^ *l1 ^ kl0;
    *l1 = (ROL8(*l0, 1) & ROL8(*l0, 7)) ^ ROL8(*l0, 2) ^ t ^ kr1;
}


int test_corollary5_1(uint8_t o1, uint8_t o2)
{

    uint8_t L0 = 0;
    uint8_t L1 = 0;
    uint8_t R0 = 0;
    uint8_t R1 = 0;

    word_t k0;
    word_t k1;
    word_t pt[4];
    uint8_t rk[4 * DEC_ROUND];

    _rdrand32_step(&k0);
    _rdrand32_step(&k1);


    for (uint32_t i = 0; i < DEC_ROUND; i++)
    {
        _rdrand32_step(rk + 4 * i);
    }


    for (uint32_t i = 0; i < TEST_NUM; i++)
    {
        //randomly select plaintext
        _rdrand32_step(pt);

        pt[1] = pt[0] ^ (1 << o1);
        pt[2] = pt[0] ^ (1 << o2);
        pt[3] = pt[0] ^ (1 << o1) ^ (1 << o2);


        //encrypt with fixed key


        for (uint32_t j = 0; j < 4; j++)
        {
            word_t ct = cipher(k0, k1, pt[j], _NUM_ROUNDS);
            //decrypt with fixed roundkey
            uint8_t l0 = (uint8_t)((ct & 0xff000000) >> 24);
            uint8_t l1 = (uint8_t)((ct & 0x00ff0000) >> 16);
            uint8_t r0 = (uint8_t)((ct & 0x0000ff00) >> 8);
            uint8_t r1 = (uint8_t)((ct & 0x000000ff));
            for (uint32_t k = 0; k < DEC_ROUND; k++)
            {
                shadow32_dec_round(&l0, &l1, &r0, &r1, rk[4 * k], rk[4 * k + 1], rk[4 * k + 2], rk[4 * k + 3]);
            }

            L0 = L0 ^ l0;
            L1 = L1 ^ l1;
            R0 = R0 ^ r0;
            R1 = R1 ^ r1;
        }
    }


    if (((L0 & 0xaa )!= 0) || ((R0 & 0xaa )!= 0))
    {
        //printf("fail\n");
        printf("8-bit value in hexadecimal: 0x%02X %02X %02X %02X\n", L0, L1, R0, R1);
        return 0;
    }
    else
    {
        //printf("pass\n");
        return 1;
    }
    //printf("8-bit value in hexadecimal: 0x%02X %02X %02X %02X\n", L0, L1, R0, R1);
}




int test_lemma1()
{
    word_t k0;
    word_t k1;
    word_t pt;
    uint8_t rk[4 * DEC_ROUND];
    word_t res1;
    word_t res2;
    _rdrand32_step(&k0);
    _rdrand32_step(&k1);


    for (uint32_t i = 0; i < DEC_ROUND; i++)
    {
        _rdrand32_step(rk + 4 * i);
    }


    for (uint32_t i = 0; i < TEST_NUM; i++)
    {
        //randomly select plaintext
        _rdrand32_step(&pt);

        //encrypt with fixed key

        word_t ct = cipher(k0, k1, pt, _NUM_ROUNDS);

        //decrypt with fixed roundkey

        uint8_t l0 = (uint8_t)((ct & 0xff000000) >> 24);
        uint8_t l1 = (uint8_t)((ct & 0x00ff0000) >> 16);
        uint8_t r0 = (uint8_t)((ct & 0x0000ff00) >> 8);
        uint8_t r1 = (uint8_t)((ct & 0x000000ff));

        for (uint32_t j = 0; j < DEC_ROUND; j++)
        {
            shadow32_dec_round(&l0, &l1, &r0, &r1, rk[4 * j], rk[4 * j + 1], rk[4 * j + 2], rk[4 * j + 3]);
        }

        //compute the sum
        //even

        if ((_NUM_ROUNDS + DEC_ROUND) % 2 == 0)
        {
            if (i == 0)
            {
                res1 = l0 ^ r0 ^ (uint8_t)((pt & 0xff000000) >> 24) ^ (uint8_t)((pt & 0x0000ff00) >> 8);
                res2 = l1 ^ r1 ^ (uint8_t)((pt & 0x00ff0000) >> 16) ^ (uint8_t)((pt & 0x000000ff)) ^ F(l0) ^ F(r0) ^ F((uint8_t)((pt & 0xff000000) >> 24)) ^ F((uint8_t)((pt & 0x0000ff00) >> 8));
            }

            word_t const1 = l0 ^ r0 ^ (uint8_t)((pt & 0xff000000) >> 24) ^ (uint8_t)((pt & 0x0000ff00) >> 8);
            word_t const2 = l1 ^ r1 ^ (uint8_t)((pt & 0x00ff0000) >> 16) ^ (uint8_t)((pt & 0x000000ff)) ^ F(l0) ^ F(r0) ^ F((uint8_t)((pt & 0xff000000) >> 24)) ^ F((uint8_t)((pt & 0x0000ff00) >> 8));

            if ((const1 != res1) || (const2 != res2))
            {
                printf("Lemma 1 TEST FAIL");
                return(0);
            }
        }

        //odd
        else
        {
            if (i == 0)
            {
                res1 = l1 ^ r1 ^ F(l0) ^ F(r0) ^ (uint8_t)((pt & 0xff000000) >> 24) ^ (uint8_t)((pt & 0x0000ff00) >> 8);
                res2 = l0 ^ r0 ^ (uint8_t)((pt & 0x00ff0000) >> 16) ^ (uint8_t)((pt & 0x000000ff)) ^ F((uint8_t)((pt & 0xff000000) >> 24)) ^ F((uint8_t)((pt & 0x0000ff00) >> 8));
            }
            word_t const1 = l1 ^ r1 ^ F(l0) ^ F(r0) ^ (uint8_t)((pt & 0xff000000) >> 24) ^ (uint8_t)((pt & 0x0000ff00) >> 8);
            word_t const2 = l0 ^ r0 ^ (uint8_t)((pt & 0x00ff0000) >> 16) ^ (uint8_t)((pt & 0x000000ff)) ^ F((uint8_t)((pt & 0xff000000) >> 24)) ^ F((uint8_t)((pt & 0x0000ff00) >> 8));
            if ((const1 != res1) || (const2 != res2))
            {
                printf("Lemma 1 TEST FAIL");
                return(0);
            }
        }

    }



    printf("Lemma 1 TEST PASS\n");
    printf("constant 1 : 0x%02X\n", res1);
    printf("constant 2 : 0x%02X\n", res2);

    return 1;

}




void shadow32_key_round(uint64_t* key_state, uint32_t round)
{
    //AddRoundCounter
    uint64_t rc = round + 1;
    rc = rc << 56;
    *key_state = *key_state ^ rc;

    //NX Operation
    uint8_t nx_in[8], nx_ou[8];
    uint32_t i;
    uint8_t mask = 0x80;
    for (i = 0; i < 8; i++)
    {
        if ((((uint8_t)(*key_state)) & mask) != 0)
        {
            nx_in[i] = 0x01;
        }
        else
        {
            nx_in[i] = 0x00;
        }
        mask = mask >> 1;
    }

    //XORs
    nx_ou[0] = nx_in[0] ^ nx_in[6];
    nx_ou[2] = nx_in[2] ^ nx_ou[0];
    nx_ou[4] = nx_in[4] ^ nx_ou[2];
    nx_ou[6] = nx_in[6] ^ nx_ou[4];

    nx_ou[1] = nx_in[1] ^ nx_in[7];
    nx_ou[3] = nx_in[3] ^ nx_ou[1];
    nx_ou[5] = nx_in[5] ^ nx_ou[3];
    nx_ou[7] = nx_in[7] ^ nx_ou[5];

    //Ands
    nx_ou[0] = nx_ou[0] & nx_in[0];
    nx_ou[2] = nx_ou[2] & nx_in[2];
    nx_ou[4] = nx_ou[4] & nx_in[4];
    nx_ou[6] = nx_ou[6] & nx_in[6];

    nx_ou[1] = nx_ou[1] & nx_in[1];
    nx_ou[3] = nx_ou[3] & nx_in[3];
    nx_ou[5] = nx_ou[5] & nx_in[5];
    nx_ou[7] = nx_ou[7] & nx_in[7];

    uint8_t nx_out = 0;

    nx_out |= nx_ou[0] << 7;
    nx_out |= nx_ou[1] << 6;
    nx_out |= nx_ou[2] << 5;
    nx_out |= nx_ou[3] << 4;
    nx_out |= nx_ou[4] << 3;
    nx_out |= nx_ou[5] << 2;
    nx_out |= nx_ou[6] << 1;
    nx_out |= nx_ou[7];

    *key_state = (*key_state & 0xFFFFFFFFFFFFFF00ULL) ^ (uint64_t)nx_out;
    //Permutation
    *key_state = ((*key_state & 0x0000000fffffff00ULL) << 8)
        | ((*key_state & 0x0000fff000000000ULL) << 12)
        | ((*key_state & 0xffff000000000000ULL) >> 48)
        | ((*key_state & 0x000000000000000fULL) << 44)
        | ((*key_state & 0x00000000000000f0ULL) << 56);
}




void key_expansion(uint32_t key0, uint32_t key1, uint8_t* rk, uint32_t round)
{
    uint32_t i;
    uint64_t key_state = ((uint64_t)key0 << 32) | (uint64_t)key1;
    uint8_t kl0, kl1, kr0, kr1;

    for (i = 0; i < round; i++)
    {
        shadow32_key_round(&key_state, i);
        kl0 = ((uint8_t)((key_state & 0xF000000000000000ULL) >> 60) << 4);
        kr0 = ((uint8_t)((key_state & 0x0F00000000000000ULL) >> 56) << 4);
        kl0 |= ((uint8_t)((key_state & 0x00F0000000000000ULL) >> 52));
        kr0 |= ((uint8_t)((key_state & 0x000F000000000000ULL) >> 48));

        kl1 = ((uint8_t)((key_state & 0x0000F00000000000ULL) >> 44) << 4);
        kr1 = ((uint8_t)((key_state & 0x00000F0000000000ULL) >> 40) << 4);
        kl1 |= ((uint8_t)((key_state & 0x000000F000000000ULL) >> 36));
        kr1 |= ((uint8_t)((key_state & 0x0000000F00000000ULL) >> 32));

        rk[4 * i + 0] = kl0;
        rk[4 * i + 1] = kl1;
        rk[4 * i + 2] = kr0;
        rk[4 * i + 3] = kr1;
    }

}







word_t cipher_dec(uint8_t* rk, word_t iv, int round)
{
    int i;

    uint8_t l0 = (uint8_t)((iv & 0xff000000) >> 24);
    uint8_t l1 = (uint8_t)((iv & 0x00ff0000) >> 16);
    uint8_t r0 = (uint8_t)((iv & 0x0000ff00) >> 8);
    uint8_t r1 = (uint8_t)((iv & 0x000000ff));

    for (i = round - 1; i > -1; i--)
    {
        shadow32_dec_round(&l0, &l1, &r0, &r1, rk[4 * i], rk[4 * i + 1], rk[4 * i + 2], rk[4 * i + 3]);
    }

    return (((uint32_t)l0) << 24) | (((uint32_t)l1) << 16) | (((uint32_t)r0) << 8) | (((uint32_t)r1));
}



word_t cipher(word_t key0, word_t key1, word_t iv, int round)
{
    uint32_t i;
    uint8_t kl0, kr0, kl1, kr1;
    uint8_t l0 = (uint8_t)((iv & 0xff000000) >> 24);
    uint8_t l1 = (uint8_t)((iv & 0x00ff0000) >> 16);
    uint8_t r0 = (uint8_t)((iv & 0x0000ff00) >> 8);
    uint8_t r1 = (uint8_t)((iv & 0x000000ff));

    uint64_t key_state = ((uint64_t)key0 << 32) | (uint64_t)key1;


    for (i = 0; i < round; i++)
    {
        shadow32_key_round(&key_state, i);
        kl0 = ((uint8_t)((key_state & 0xF000000000000000ULL) >> 60) << 4);
        kr0 = ((uint8_t)((key_state & 0x0F00000000000000ULL) >> 56) << 4);
        kl0 |= ((uint8_t)((key_state & 0x00F0000000000000ULL) >> 52));
        kr0 |= ((uint8_t)((key_state & 0x000F000000000000ULL) >> 48));

        kl1 = ((uint8_t)((key_state & 0x0000F00000000000ULL) >> 44) << 4);
        kr1 = ((uint8_t)((key_state & 0x00000F0000000000ULL) >> 40) << 4);
        kl1 |= ((uint8_t)((key_state & 0x000000F000000000ULL) >> 36));
        kr1 |= ((uint8_t)((key_state & 0x0000000F00000000ULL) >> 32));

        shadow32_round(&l0, &l1, &r0, &r1, kl0, kl1, kr0, kr1);
        if (round == _NUM_ROUNDS + 1)
        {
            //printf("l0 : %02X\n", l0);
            //printf("l1 : %02X\n", l1);
            //printf("r0 : %02X\n", r0);
            //printf("r1 : %02X\n", r1);
        }
    }

    return (((uint32_t)l0) << 24) | (((uint32_t)l1) << 16) | (((uint32_t)r0) << 8) | (((uint32_t)r1));
}








#define BLOCK_LEN       32
#define ACTIVE_TAB_LEN  31
#define CONST1_TAB_LEN  1
#define CONST0_TAB_LEN  BLOCK_LEN - 1 - (ACTIVE_TAB_LEN + CONST1_TAB_LEN)
#define OUTPUT_IDX      9


uint8_t ACTIVE_TAB[ACTIVE_TAB_LEN] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30 };
uint8_t CONST1_TAB[CONST1_TAB_LEN] = { 31 };





int main()
{

    test_lemma1();

    for (int i = 0; i < TEST_NUM; i++)
    {
        if (test_corollary5_1(3, 5) == 0)
        {
            printf("Corollary 5.1 TEST FAIL");
            return -1;
        }
    }
    printf("Corollary 5.1 TEST PASS");

}