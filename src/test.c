// #include <stdlib.h>
// #include <stdio.h>
#include <hal.h>
#include <sendfn.h>
#include "base.h"
#include "smult.c"
#include "rng.h"

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
#define MODE_SET_KARAT 4


static unsigned char m[mlen] = {0};
static unsigned char n[nlen] = {0};
static unsigned char p[plen] = {0};
// static unsigned char q[qlen] = {0};
// static unsigned char r[rlen] = {0};

// static unsigned char m2[mlen + CRYPTO_BYTES] = {0};
// static unsigned char n2[nlen + CRYPTO_BYTES] = {0};
// static unsigned char p2[plen + CRYPTO_BYTES] = {0};
// static unsigned char q2[qlen + CRYPTO_BYTES] = {0};
// static unsigned char r2[rlen + CRYPTO_BYTES] = {0};

char checksum[CRYPTO_SCALARBYTES * 2 + 1];

int crypto_scalarmult_base(unsigned char *q,
  const unsigned char *n)
{
  return crypto_scalarmult(q,n,base);
}

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
    unsigned char seed[48] = {0};
    // randombytes_init(seed, NULL, 256);
    hal_setup(CLOCK_FAST);
    int mode = MODE_BLANK;
    while(1)
    {
      if(mode == MODE_HASH)
      {
        // send_start();
        // send_string("log", "Hashing..");
        // send_stop();

        randombytes(m, CRYPTO_SCALARBYTES);
        randombytes(n, CRYPTO_SCALARBYTES);

        int start = hal_get_time();

        crypto_scalarmult_base(p, m);
        int cycles = hal_get_time() - start;
        checksum_compute();

        send_start();
        send_string("checksum", checksum);
        send_unsigned("cycles", cycles, 10);
        send_stop();
      }else if(mode == MODE_PING)
      {
        send_start();
        send_string("ping", "pong");
        send_stop();
      }else if(mode == MODE_SEED)
      {
				for(int i = 0; i < 48; i++)
				{
					int curr = hal_getc();
					seed[i] = curr;
				}
				 randombytes_init(seed, NULL, 256);
      }else if(mode == MODE_SET_KARAT)
      {
				KARAT_L = hal_getc();
      }

    	mode = MODE_BLANK;

      if(mode == MODE_BLANK)
      {
        mode = hal_getc();
      }
    }
    return 0;
}