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

static unsigned char *m;
static unsigned char *n;
static unsigned char *p;
static unsigned char *q;
static unsigned char *r;

static unsigned char *m2;
static unsigned char *n2;
static unsigned char *p2;
static unsigned char *q2;
static unsigned char *r2;

// int rnd[] = {};
int rndI = 0;
/**
 * Very bad random polyfill
 **/
int random()
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

unsigned char *alignedcalloc(unsigned long long len)
{
  unsigned char *x = (unsigned char *) calloc(1,len + 256);
  long long i;
  if (!x){
      //fail
  }
  /* will never deallocate so shifting is ok */
  for (i = 0;i < len + 256;++i) x[i] = random();
  x += 64;
  x += 63 & (-(unsigned long) x);
  for (i = 0;i < len;++i) x[i] = 0;
  return x;
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

//   if (crypto_scalarmult_base(p,n) != 0) return "crypto_scalarmult_base returns nonzero";

//   for (i = -16;i < nlen + 16;++i) if (n2[i] != n[i]) return "crypto_scalarmult_base overwrites input";
//   for (i = -16;i < 0;++i) if (p2[i] != p[i]) return "crypto_scalarmult_base writes before output";
//   for (i = plen;i < plen + 16;++i) if (p2[i] != p[i]) return "crypto_scalarmult_base writes after output";

//   for (tests = 0;tests < 100;++tests) {
//     for (i = -16;i < 0;++i) q[i] = random();
//     for (i = -16;i < 0;++i) p[i] = random();
//     for (i = -16;i < 0;++i) m[i] = random();
//     for (i = qlen;i < qlen + 16;++i) q[i] = random();
//     for (i = plen;i < plen + 16;++i) p[i] = random();
//     for (i = mlen;i < mlen + 16;++i) m[i] = random();
//     for (i = -16;i < qlen + 16;++i) q2[i] = q[i];
//     for (i = -16;i < plen + 16;++i) p2[i] = p[i];
//     for (i = -16;i < mlen + 16;++i) m2[i] = m[i];

//     if (crypto_scalarmult(q,m,p) != 0) return "crypto_scalarmult returns nonzero";

//     for (i = -16;i < mlen + 16;++i) if (m2[i] != m[i]) return "crypto_scalarmult overwrites n input";
//     for (i = -16;i < plen + 16;++i) if (p2[i] != p[i]) return "crypto_scalarmult overwrites p input";
//     for (i = -16;i < 0;++i) if (q2[i] != q[i]) return "crypto_scalarmult writes before output";
//     for (i = qlen;i < qlen + 16;++i) if (q2[i] != q[i]) return "crypto_scalarmult writes after output";

//     if (crypto_scalarmult(m2,m2,p) != 0) return "crypto_scalarmult returns nonzero";
//     for (i = 0;i < qlen;++i) if (q[i] != m2[i]) return "crypto_scalarmult does not handle n overlap";
//     for (i = 0;i < qlen;++i) m2[i] = m[i];

//     if (crypto_scalarmult(p2,m2,p2) != 0) return "crypto_scalarmult returns nonzero";
//     for (i = 0;i < qlen;++i) if (q[i] != p2[i]) return "crypto_scalarmult does not handle p overlap";

//     if (crypto_scalarmult(r,n,q) != 0) return "crypto_scalarmult returns nonzero";
//     if (crypto_scalarmult(q,n,p) != 0) return "crypto_scalarmult returns nonzero";
//     if (crypto_scalarmult(p,m,q) != 0) return "crypto_scalarmult returns nonzero";
//     for (j = 0;j < plen;++j) if (p[j] != r[j]) return "crypto_scalarmult not associative";
//     for (j = 0;j < mlen;++j) m[j] ^= q[j % qlen];
//     for (j = 0;j < nlen;++j) n[j] ^= p[j % plen];
//   }

  
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
  m = alignedcalloc(mlen);
  n = alignedcalloc(nlen);
  p = alignedcalloc(plen);
  q = alignedcalloc(qlen);
  r = alignedcalloc(rlen);
  m2 = alignedcalloc(mlen + CRYPTO_BYTES);
  n2 = alignedcalloc(nlen + CRYPTO_BYTES);
  p2 = alignedcalloc(plen + CRYPTO_BYTES);
  q2 = alignedcalloc(qlen + CRYPTO_BYTES);
  r2 = alignedcalloc(rlen + CRYPTO_BYTES);
}


int main(void){
    allocate();
    crypto_scalarmult(q,n,p);
    crypto_scalarmult_base(r,n);

    const char *problem = checksum_compute();

    hal_setup(CLOCK_FAST);
    send_start();

    printword(checksum);

    send_stop();
    // printword(p);
    // printword(q);

    return 0;
}