#include <stdlib.h>
#include <stdio.h>
// #include <hal.h>
// #include <sendfn.h>
#include "smult.c"
// #include "rng.h"

#define CRYPTO_BYTES 32
#define CRYPTO_SCALARBYTES 32

const unsigned char base[32] = {9};

#define mlen CRYPTO_SCALARBYTES
#define nlen CRYPTO_SCALARBYTES
#define plen CRYPTO_BYTES
#define qlen CRYPTO_BYTES
#define rlen CRYPTO_BYTES

#define MODE_BLANK 0
#define MODE_HASH 1
#define MODE_SEED 2
#define MODE_PING 3


static unsigned char m[mlen] = {0};
static unsigned char n[nlen] = {0};
static unsigned char p[plen] = {0};
static unsigned char q[qlen] = {0};
static unsigned char r[rlen] = {0};

static unsigned char m2[mlen + CRYPTO_BYTES] = {0};
static unsigned char n2[nlen + CRYPTO_BYTES] = {0};
static unsigned char p2[plen + CRYPTO_BYTES] = {0};
static unsigned char q2[qlen + CRYPTO_BYTES] = {0};
static unsigned char r2[rlen + CRYPTO_BYTES] = {0};

// 07a37cbc142093c8b755dc1b10e86cb426374ad16aa853ed0bdfc0b2b86d1c7c

char checksum[CRYPTO_SCALARBYTES * 2 + 1];

int crypto_scalarmult_base(unsigned char *q,
  const unsigned char *n)
{
  return crypto_scalarmult(q,n,base);
}

// void fillMemory(unsigned char *x, unsigned long long len){
//   unsigned long long i;

//   // for (i = 0;i < len + 256;++i) x[i] = random();
//   // x += 64;
//   // x += 63 & (-(unsigned long) x);
//   for (i = 0;i < len;++i) x[i] = 0;
// }

const char *checksum_compute(void)
{
  long long i;
  // long long j;
  // long long tests;

  /*for (i = 0;i < mlen;++i) m[i] = i;
  for (i = 0;i < nlen;++i) n[i] = random();
  for (i = 0;i < plen;++i) p[i] = random();
  for (i = 0;i < qlen;++i) q[i] = i + 3;
  for (i = 0;i < rlen;++i) r[i] = i + 4;*/
  
  //crypto_scalarmult_base(p,n);

  for (i = 0;i < CRYPTO_SCALARBYTES;++i) {
    checksum[2 * i] = "0123456789abcdef"[15 & (p[i] >> 4)];
    checksum[2 * i + 1] = "0123456789abcdef"[15 & p[i]];
  }
  checksum[2 * i] = 0;
  return 0;
}


int main(void){
    printf("Startin base..\n");
    crypto_scalarmult_base(p, m);
    printf("Crypto done!\n\n");
    checksum_compute();
    printf("checksum done!\n\n");
    printf("Checksum:\n");
    for(int i = 0; i < 64; i++)
    {
      printf("%c", checksum[i]);
    }
    printf("\n");
    return 0;
}

// int main(void){

//     unsigned char seed[48] = {0};
//     randombytes_init(seed, NULL, 256);


//     hal_setup(CLOCK_FAST);

//     send_start();
//     send_string("log", "Starting...");
//     send_stop();

//     // allocate();
    
//     // crypto_scalarmult(q,n,p);
    
//     // send_start();
//     // send_string("log", "Computing base..");
//     // send_stop();

//     // crypto_scalarmult_base(r,n);
    
    
//     send_start();
//     send_string("log", "Computing checksum..");
//     send_stop();

//     // const char *problem = 
//     checksum_compute();

//     send_start();
//     send_string("checksum", checksum);
//     send_stop();
//     // printword(p);
//     // printword(q);
//     while (1)
// 		  ;
//     return 0;
// }