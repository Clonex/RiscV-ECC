function squeeze(a)
{
  let j;
  let u;
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

function mult(out, a, b)
{
  let i;
  let j;
  let u;

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

  squeeze(out);
}

function mult2(out, a, b)
{
  let i;
  let j;
  let u;

  for(i = 0; i < 32; ++i)
  {
    out[i] = karatsuba(a[i], b[i]);
  }

  squeeze(out);
}

function karatsuba(x, y) {
    if (x<10 && y<10) {
      return x*y;
    }
    let maxLength = Math.max(x.toString().length, y.toString().length);
    let m = Math.round(maxLength/2);
  
    let xHigh = Math.floor(x/ Math.pow(10,m));
    let yHigh = Math.floor(y/ Math.pow(10,m));
    let xLow = x %  Math.pow(10,m);
    let yLow = y %  Math.pow(10,m);
  
    let a = karatsuba(xHigh, yHigh);
    let d = karatsuba(xLow, yLow);
    let e = karatsuba(xLow+xHigh, yLow+yHigh)-a-d;
  
    return a * Math.pow(10, m*2) + e * Math.pow(10,m) + d;
  }


let o = [];
let a = [...Array(32)].map(d => 1);
let b = [...Array(32)].map(d => 2);
console.log(a, b);
mult2(o, a, b);

console.log(o);