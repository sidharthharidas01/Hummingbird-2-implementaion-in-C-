#include<iostream>

using namespace std;

uint16_t R[8]/*, K[8]*/;
uint16_t t1, t2, t3, t4;
uint16_t u1, u2, u3;

uint16_t inv_s(uint16_t n, uint16_t x)         //S(x)
{
    uint16_t inv_sbox[4][16] = 
    {
        {11, 5, 4, 15, 12, 6, 9, 0, 13, 3, 14, 8, 1, 10, 2, 7},
        {9, 2, 15, 8, 0, 12, 3, 6, 4, 13, 1, 14, 7, 11, 10, 5},
        {12, 3, 0, 10, 11, 4, 5, 15, 9, 14, 6, 13, 2, 7, 8, 1},
        {10, 7, 6, 9, 1, 2, 12, 5, 3, 4, 8, 15, 13, 14, 11, 0}
    };
    return inv_sbox[n][x];
}
uint16_t inv_S(uint16_t x)
{
    uint16_t x0, x1, x2, x3;

    x0 = (x & 0xF000)>>12;
    x1 = (x & 0x0F00)>>8;
    x2 = (x & 0x00F0)>>4;
    x3 = (x & 0x000F);

    return ((inv_s(0, x0)<<12)|(inv_s(1, x1)<<8)|(inv_s(2, x2)<<4)|(inv_s(3, x3)));
}

uint16_t s(uint16_t n, uint16_t x)         //S(x)
{
    uint16_t sbox[4][16] = 
    {
        {7, 12, 14, 9, 2, 1, 5, 15, 11, 6, 13, 0, 4, 8, 10, 3},
        {4, 10, 1, 6, 8, 15, 7, 12, 3, 0, 14, 13, 5, 9, 11, 2},
        {2, 15, 12, 1, 5, 6, 10, 13, 14, 8, 3, 4, 0, 11, 9, 7},
        {15, 4, 5, 8, 9, 7, 2, 1, 10, 3, 0, 14, 6, 12, 13, 11}
    };
    return sbox[n][x];
}

uint16_t S(uint16_t x)
{
    uint16_t x0, x1, x2, x3;

    x0 = (x & 0xF000)>>12;
    x1 = (x & 0x0F00)>>8;
    x2 = (x & 0x00F0)>>4;
    x3 = (x & 0x000F);

    return ((s(0, x0)<<12)|(s(1, x1)<<8)|(s(2, x2)<<4)|(s(3, x3)));
}

uint16_t rotl(uint16_t x, uint16_t n)  //rotate left
{
    return (x << n) | (x >> (16 - n));
}

uint16_t rotr(uint16_t x, uint16_t n, uint16_t N = 4)  //rotate ryt
{
    return (x >> n) | (x << (16 - n));
}

uint16_t l(uint16_t x)
{
    return (x^rotl(x, 6)^rotl(x, 10));
}

uint16_t inv_l(uint16_t x)
{
    uint16_t y = x;
    for(int i=0;i<3;i++)
    {
        y = l(y);
    }
    return y;
}

uint16_t inv_f(uint16_t x)
{
    return inv_S(inv_l(x));
}

uint16_t f(uint16_t x)
{
    return l(S(x));
}

uint16_t wd16(uint16_t x, uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
    return f(f(f(f(x^a)^b)^c)^d);
}

uint16_t inv_wd16(uint16_t x, uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
    return inv_f(inv_f(inv_f(inv_f(x)^d)^c)^b)^a;
}

void calc_R0(uint16_t *iv)
{
    for(int i=0;i<8;i++)
    {
        R[i] = iv[i%4];
        cout<<"R["<<i<<"] = "<<R[i]<<endl;
    }
}

void update_R()
{
    R[0] = rotl(((R[0]+t4)%65536), 3);
    R[1] = rotr(((R[1]+t1)%65536), 1);
    R[2] = rotl(((R[2]+t2)%65536), 8);
    R[3] = rotl(((R[3]+t3)%65536), 1);
    R[4] = R[4]+R[0];
    R[5] = R[5]+R[1];
    R[6] = R[6]+R[2];
    R[7] = R[4]+R[3];

    cout<<"\nUpdated R: \n";
    for(int i=0;i<8;i++)
        cout<<"R["<<i<<"] = "<<R[i]<<endl;
}

void initialize(uint16_t *iv, uint16_t *K)
{ 
    calc_R0(iv);

    for(int i =0; i<4; i++)
    {
        t1 = wd16((((R[0]+i))%65536), K[0], K[1], K[2], K[3]);
        t2 = wd16(((R[1]+t1)%65536), K[4], K[5], K[6], K[7]);
        t3 = wd16(((R[2]+t2)%65536), K[0], K[1], K[2], K[3]);
        t4 = wd16(((R[3]+t3)%65536), K[4], K[5], K[6], K[7]);
        update_R();
    }
}

void enc_update_R()
{
    R[0] = (R[0] + t3) % 65536;
    R[1] = (R[1] + t1) % 65536;
    R[2] = (R[2] + t2) % 65536;
    R[3] = (R[3] + ((t1 + R[0])%65536))%65536;
    R[4] = R[4] ^ R[0];
    R[5] = R[5] ^ R[1];
    R[6] = R[6] ^ R[2];
    R[7] = R[7] ^ R[3];

    cout<<"\nUpdated R: \n";
    for(int i=0;i<8;i++)
        cout<<"R["<<i<<"] = "<<R[i]<<endl;
}

uint16_t decrypt16b(uint16_t ct_block, uint16_t *K)
{
    uint16_t pt;
    cout<<"\npt = "<<ct_block<<" R0 = "<<R[0]<<endl;
    for(int i=0;i<8;i++)
        cout<<"K["<<i<<"] = "<<K[i]<<endl;

    u3 = inv_wd16(((ct_block-R[0])%65536), K[4], K[5], K[6], K[7]);
    u2 = inv_wd16(((u3-R[3])%65536), (K[0]^R[4]), (K[1]^R[5]), (K[2]^R[6]), (K[3]^R[7]));
    u1 = inv_wd16(((u2-R[2])%65536), (K[4]^R[4]), (K[5]^R[5]), (K[6]^R[6]), (K[7]^R[7]));
    pt = (inv_wd16(((u1-R[1])%65536), K[0], K[1], K[2], K[3])-R[0]) % 65536;
    cout<<"\nu1 = "<<u1<<"\nu2 = "<<u2<<"\nu3 = "<<u3;

    t3 = (u3 - R[3])%65536;
    t2 = (u2 - R[2])%65536;
    t1 = (u1 - R[1])%65536;
    cout<<"\nt1 = "<<t1<<"\nt2 = "<<t2<<"\nt3 = "<<t3;

    enc_update_R();
    return pt;
}

uint16_t *decrypt(uint16_t *ct, int len, uint16_t *k)
{
    static uint16_t pt[10];

    for(int i=0;i<len;i++)
    {
        pt[i] = decrypt16b(ct[i], k);
    }
    cout<<endl;
    for(int i=0; i<len; i++)
        cout<<pt[i]<<"\t";
    return pt;
}

int main()
{
    uint16_t ct[5] = {16952, 44391, 15674, 59474, 29457};
    uint16_t key[8] = {25,10,5,12,23,1,2,3};
    uint16_t iv[4] = {25,10,5,12};
    uint16_t *pt;

    cout<<"Cipher = : ";
    for(int i=0; i<5; i++)
        cout<<ct[i]<<"\t";
    
    cout<<"\n Key: ";
    for(int i=0; i<8; i++)
        cout<<key[i]<<"\t";

    cout<<"\nInitializing....";
    initialize(iv, key);

    cout<<"\nDecrypting msg.....";
    pt = decrypt(ct, 5, key);

    cout<<"\n\n Decrypted msg = ";
    for(int i=0; i<5; i++)
        cout<<*(pt+i)<<"\t";
}