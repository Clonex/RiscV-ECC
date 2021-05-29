// #include <stdlib.h>
// #include <stdio.h>
#include <hal.h>
#include <sendfn.h>
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


unsigned char seed[48] = {0};

int main(void){
    // randombytes_init(seed, NULL, 256);
    hal_setup(CLOCK_FAST);
    int mode = MODE_BLANK;
    while(1)
    {
      if(mode == MODE_HASH)
      {
        send_start();
        send_string("log", "Hashing..");
        send_stop();

        randombytes(m, CRYPTO_SCALARBYTES);
        randombytes(n, CRYPTO_SCALARBYTES);

        int start = hal_get_time();

        crypto_scalarmult(p, m, base);
        int cycles = hal_get_time() - start;
        // checksum_compute();

        send_start();
        // send_string("checksum", checksum);
        send_unsigned("cycles", cycles, 10);
        send_stop();
      }else if(mode == MODE_PING)
      {
        send_start();
        send_string("ping", "Hello");
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