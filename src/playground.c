#include <stdio.h>

static void squeeze(unsigned int a[32])
{
  unsigned int j;
  unsigned int u;
  u = 0;
  for(j = 0;j < 31;++j)
  { 
    u += a[j];
    a[j] = u & 255;
    u >>= 8; 
  }

  u += a[31];
  a[31] = u & 127;
  u = 19 * (u >> 7);

  for(j = 0;j < 31;++j)
  {
    u += a[j];
    a[j] = u & 255;
    u >>= 8;
  }
  
  u += a[31];
  a[31] = u;
}




static void subr(unsigned int *out, const unsigned int *a, const unsigned int *b, int length)
{
  int c = 0;
  for(int i = length - 1; i >= 0; i--)
  {
    out[i] = (a[i] - b[i] - c) & 0x7FFFFFFF;
    c = (a[i] - b[i] - c) > 0x7FFFFFFF;
  }
}

static void addr(unsigned int *out, const unsigned int *a, const unsigned int *b, int length)
{
  int c = 0;
  for(int i = length - 1; i >= 0; i--)
  {
    out[i] = (a[i] + b[i] + c) & 0x7FFFFFFF;
    c = (a[i] + b[i] + c) > 0x7FFFFFFF;
  }
}

static void add(unsigned int out[32], const unsigned int a[32],const unsigned int b[32])
{
  addr(out, a, b, 32);
}

static void sub(unsigned int out[32],const unsigned int a[32],const unsigned int b[32])
{
  subr(out, a, b, 32);
}


static void mult(unsigned int out[32],const unsigned int a[32],const unsigned int b[32])
{
  unsigned int i;
  unsigned int j;
  unsigned int u;

  for(i = 0; i < 32; ++i)
  {
    u = 0;
    for(j = 0; j <= i; ++j){
      u += a[j] * b[i - j];
    }

    for(j = i + 1; j < 32; ++j){
      u += 38 * a[j] * b[i + 32 - j];
    }
    out[i] = u;
  }
  
  // squeeze(out, out);
}


static void multSimple(unsigned int *out, const unsigned int *x, const unsigned int *y, int length)
{
  for(int i = 0; i < length; i++)
  {
    for (int j = 0; j < length; j++)
    {
      out[i + j] += x[i] * y[j];
    }
  }
}


#define SHIFTED_L (length >> 1)
static void karat(unsigned int *out, const unsigned int *x, const unsigned int *y, int length)
{
  // printf("Karat start length = %d\n", length);

  int LOW[length + 1];
  int HIGH[length + 1];
  int MED[length + 2];

  int M0[length + 2];
  int M1[length + 1];
  int MM[length + 1];

  // for(int i = 0; i < length * 2; i++)
  // {
  //     out[i] = 0;
  // }

  for(int i = 0; i <= length; i++)
  {
      LOW[i] = 0;
      HIGH[i] = 0;
      MED[i] = 0;
      M0[i] = 0;
      M1[i] = 0;
      MM[i] = 0;
  }

  if(length > 2)
  {
    karat(LOW, &x[0], &y[0], SHIFTED_L);
    karat(HIGH, &x[SHIFTED_L], &y[SHIFTED_L], SHIFTED_L);
  }else{  
    multSimple(LOW, &x[0], &y[0], SHIFTED_L);
    multSimple(HIGH, &x[SHIFTED_L], &y[SHIFTED_L], SHIFTED_L);
  }

  addr(M0, &x[0], &x[SHIFTED_L], SHIFTED_L);
  addr(M1, &y[0], &y[SHIFTED_L], SHIFTED_L);

  multSimple(MM, M0, M1, SHIFTED_L + 1);

  subr(M0, MM, LOW, length + 1);
  subr(MED, M0, HIGH, length + 1);

  addr(out, out, LOW, length);
  addr(&out[SHIFTED_L], &out[SHIFTED_L], MED, length + 1);
  addr(&out[length], &out[length], HIGH, length);

  //  squeeze(out, length);
}


int main(void){
    unsigned int a[64] = {203,59,82,63,133,87,88,237,173,46,112,103,175,89,115,220,142,147,222,209,148,102,123,250,20,178,113,94,75,225,64,6};
    unsigned int b[64] = {71,131,1,54,178,232,130,106,45,231,145,52,27,126,184,178,169,10,248,77,148,50,14,96,125,56,96,44,188,182,123,511905208};
    unsigned int c[65] = {0};
    
    karat(c, a, b, 32);
    squeeze(c);
    for(int i = 0; i <= 32; i++)
    {
        printf("out[%i]: %i\n", i, c[i]);
        c[i] = 0;
    }
    printf("------ OLD -------\n");
    multSimple(c, a, b, 32);
    squeeze(c);
    for(int i = 0; i <= 32; i++)
    {
        printf("out[%i]: %i\n", i, c[i]);
        c[i] = 0;
    }
    return 0;
}