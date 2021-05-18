#include <stdio.h>

static void squeeze(int *a, int length)
{
  unsigned int j;
  unsigned int u;
  u = 0;
  int l = length - 1;
  for(j = 0;j < l;++j)
  { 
    u += a[j];
    a[j] = u & 255;
    u >>= 8; 
  }

  u += a[l];
  a[l] = u & 127;
  u = 19 * (u >> 7);

  for(j = 0;j < l;++j)
  {
    u += a[j];
    a[j] = u & 255;
    u >>= 8;
  }
  
  u += a[l];
  a[l] = u;
}



static void sub(int *out, int *a, int *b, int length)
{
  int c = 0;
  for(int i = length - 1; i >= 0; i--)
  {
    out[i] = (a[i] - b[i] - c) & 0x7FFFFFFF;
    c = (a[i] - b[i] - c) > 0x7FFFFFFF;
  }
}

static void add(int *out, int *a, int *b, int length)
{
  int c = 0;
  for(int i = length - 1; i >= 0; i--)
  {
    out[i] = (a[i] + b[i] + c) & 0x7FFFFFFF;
    c = (a[i] + b[i] + c) > 0x7FFFFFFF;
  }
}


// static void mult(int *out, int *a, int *b, int length)
// {
//   unsigned int i;
//   unsigned int j;
//   unsigned int u;

//   for(i = 0; i < length; ++i)
//   {
//     u = 0;
//     for(j = 0; j <= i; ++j){
//       u += a[j] * b[i - j];
//     }

//     for(j = i + 1; j < length; ++j){
//       u += 38 * a[j] * b[i + length - j];
//     }
//     out[i] = u;
//   }
//   // squeeze(out, length);
// }


int mult(int *out, int *x, int *y, int len)
{
  for(int i = 0; i < len; i++)
  {
    for (int j = 0; j < len; j++)
    {
      out[i + j] += x[i] * y[j];
    }
  }

  return 0;
}


#define SHIFTED_L (length >> 1)
static void karatsuba(int *out, int *x, int *y, int length)
{
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

  if(length > 4)
  {
    karatsuba(LOW, &x[0], &y[0], SHIFTED_L);
    karatsuba(HIGH, &x[SHIFTED_L], &y[SHIFTED_L], SHIFTED_L);
  }else{  
    mult(LOW, &x[0], &y[0], SHIFTED_L);
    mult(HIGH, &x[SHIFTED_L], &y[SHIFTED_L], SHIFTED_L);
  }

  add(M0, &x[0], &x[SHIFTED_L], SHIFTED_L);
  add(M1, &y[0], &y[SHIFTED_L], SHIFTED_L);

  mult(MM, M0, M1, SHIFTED_L + 1);

  sub(M0, MM, LOW, length + 1);
  sub(MED, M0, HIGH, length + 1);

  add(out, out, LOW, length);
  add(&out[SHIFTED_L], &out[SHIFTED_L], MED, length + 1);
  add(&out[length], &out[length], HIGH, length);

  //  squeeze(out, length);
}


int main(void){
    int a[32] = {255, 255, 255, 255, 255, 100};
    int b[32] = {255, 255, 255, 255, 255, 100};
    int c[33] = {0};
    
    karatsuba(c, a, b, 16);
    squeeze(c, 32);
    for(int i = 0; i <= 32; i++)
    {
        printf("out[%i]: %i\n", i, c[i]);
        c[i] = 0;
    }
    printf("------ OLD -------\n");
    mult(c, a, b, 32);
    squeeze(c, 32);
    for(int i = 0; i <= 32; i++)
    {
        printf("out[%i]: %i\n", i, c[i]);
        c[i] = 0;
    }
    return 0;
}