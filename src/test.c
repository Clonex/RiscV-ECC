// #include <stdlib.h>
// #include <stdio.h>
#include <hal.h>
#include <sendfn.h>
#include "smult.c"

#define CRYPTO_BYTES 32
#define CRYPTO_SCALARBYTES 32

const unsigned char base[32] = {9};

#define mlen CRYPTO_SCALARBYTES
#define nlen CRYPTO_SCALARBYTES
#define plen CRYPTO_BYTES
#define qlen CRYPTO_BYTES
#define rlen CRYPTO_BYTES


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

/**
 * Very bad random polyfill
 **/
int rndI = 0;
long int random()
{
  rndI++;
  return rndI % 10000; //rnd[rndI % 100];
}

char checksum[CRYPTO_SCALARBYTES * 2 + 1];

int crypto_scalarmult_base(unsigned char *q,
  const unsigned char *n)
{
  return crypto_scalarmult(q,n,base);
}

void fillMemory(unsigned char *x, unsigned long long len){
  long long i;

  for (i = 0;i < len + 256;++i) x[i] = random();
  x += 64;
  x += 63 & (-(unsigned long) x);
  for (i = 0;i < len;++i) x[i] = 0;
}

const char *checksum_compute(void)
{
  long long i;
  long long j;
  long long tests;

  for (i = 0;i < mlen;++i) m[i] = i;
  for (i = 0;i < nlen;++i) n[i] = i + 1;
  for (i = 0;i < plen;++i) p[i] = i + 2;
  for (i = 0;i < qlen;++i) q[i] = i + 3;
  for (i = 0;i < rlen;++i) r[i] = i + 4;

  for (i = -16;i < 0;++i) p[i] = random();
  for (i = -16;i < 0;++i) n[i] = random();
  for (i = plen;i < plen + 16;++i) p[i] = random();
  for (i = nlen;i < nlen + 16;++i) n[i] = random();
  for (i = -16;i < plen + 16;++i) p2[i] = p[i];
  for (i = -16;i < nlen + 16;++i) n2[i] = n[i];
  
  crypto_scalarmult_base(p,n);

  for (i = 0;i < CRYPTO_SCALARBYTES;++i) {
    checksum[2 * i] = "0123456789abcdef"[15 & (p[i] >> 4)];
    checksum[2 * i + 1] = "0123456789abcdef"[15 & p[i]];
  }
  checksum[2 * i] = 0;
  return 0;
}

static void printword(const char *s)
{
  // if (!*s) putchar('-');
  // while (*s) {
  //   if (*s == ' ') putchar('_');
  //   else if (*s == '\t') putchar('_');
  //   else if (*s == '\r') putchar('_');
  //   else if (*s == '\n') putchar('_');
  //   else putchar(*s);
  //   ++s;
  // }
  // putchar(' ');
  // putchar('\n');
  send_string("word", s);
}

void allocate(void)
{
  fillMemory(m, mlen);
  fillMemory(n, nlen);
  fillMemory(p, plen);
  fillMemory(q, qlen);
  fillMemory(r, rlen);
  fillMemory(m2, mlen + CRYPTO_BYTES);
  fillMemory(n2, nlen + CRYPTO_BYTES);
  fillMemory(p2, plen + CRYPTO_BYTES);
  fillMemory(q2, qlen + CRYPTO_BYTES);
  fillMemory(r2, rlen + CRYPTO_BYTES);
}


int main(void){
    hal_setup(CLOCK_FAST);

    send_start();
    send_string("strings", "Starting...");
    send_stop();

    allocate();
    
    crypto_scalarmult(q,n,p);
    
    send_start();
    send_string("strings", "Computing base..");
    send_stop();

    crypto_scalarmult_base(r,n);
    
    
    send_start();
    send_string("strings", "Computing checksum..");
    send_stop();

    // const char *problem = 
    checksum_compute();

    send_start();
    printword(checksum);

    send_stop();
    // printword(p);
    // printword(q);
    while (1)
		  ;
    return 0;
}