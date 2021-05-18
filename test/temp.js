// function squeeze(a)
// {
//   let j;
//   let u;
//   u = 0;
//   for(j = 0;j < 31;++j)
//   { 
//     u += a[j];
//     a[j] = u & 255;
//     u >>= 8; 
//   }

//   u += a[31];
//   a[31] = u & 127;
//   u = 19 * (u >> 7);

//   for(j = 0;j < 31;++j)
//   {
//     u += a[j];
//     a[j] = u & 255;
//     u >>= 8;
//   }
  
//   u += a[31];
//   a[31] = u;
// }

// function mult(out, a, b)
// {
//   let i;
//   let j;
//   let u;

//   for(i = 0; i < 32; ++i)
//   {
//     u = 0;
//     for(j = 0; j <= i; ++j){
//       u += a[j] * b[i - j];
//     }

//     for(j = i + 1; j < 32; ++j){
//       u += 38 * a[j] * b[i + 32 - j];
//     }
//     out[i] = u;
//   }

//   squeeze(out);
// }

// function mult2(out, a, b)
// {
//   let i;
//   let j;
//   let u;

//   for(i = 0; i < 32; ++i)
//   {
//     out[i] = karatsuba(a[i], b[i]);
//   }

//   squeeze(out);
// }

// function karatsuba(x, y) {
//     if (x<10 && y<10) {
//       return x*y;
//     }
//     let maxLength = Math.max(x.toString().length, y.toString().length);
//     let m = Math.round(maxLength/2);
  
//     let xHigh = Math.floor(x/ Math.pow(10,m));
//     let yHigh = Math.floor(y/ Math.pow(10,m));
//     let xLow = x %  Math.pow(10,m);
//     let yLow = y %  Math.pow(10,m);
  
//     let a = karatsuba(xHigh, yHigh);
//     let d = karatsuba(xLow, yLow);
//     let e = karatsuba(xLow+xHigh, yLow+yHigh)-a-d;
  
//     return a * Math.pow(10, m*2) + e * Math.pow(10,m) + d;
//   }

function multiply(r, x, y) {
  for (let i = 0; i < 16; i++) {
    for (let j = 0; j < 16; j++)
      r[i + j] += x[i] * y[j];
  }
}

function karatsuba(r, x, y) {
  let l = [];
  let h = [];
  let m = [];

  for (let i = 0; i < 31; i++) {
    l[i] = 0;
    h[i] = 0;
    m[i] = 0;
  }
  multiply(l, x, y);
  multiply(h, x + 16, y + 16);

  let x_sum = [];
  let y_sum = [];
  for (let i = 0; i < 16; i++) {
    x_sum[i] = x[i] + x[i + 16];
    y_sum[i] = y[i] + y[i + 16];
  }
  multiply(m, x_sum, y_sum);

  for (let i = 0; i < 31; i++)
    r[i] = l[i];

  for (let i = 32; i < 63; i++)
    r[i] = h[i - 32];
  
  r[31] = 0;

  for (let i = 0; i < 31; i++)
    r[i + 16] += m[i] - l[i] - h[i]; 
}

function times19(a)
{
  return (a << 4) + (a << 1) + a;
}

function times38(a)
{
  return (a << 5) + (a << 2) + (a << 1);
}

function reduce_mul(r)
{
  for(let rep=0;rep<3;rep++)
  {
    let t = r[31] >> 7;
    r[31] &= 127;
    t = times19(t);
    r[0] += t;
    for(let i=0;i<31;i++)
    {
      t = r[i] >> 8;
      r[i+1] += t;
      r[i] &= 255;
    }
  }
}


function kara(r, x, y)
{
  let t = [];
  karatsuba(t, x, y);

  for(let i=32;i<63;i++)
  {
    r[i-32] = t[i-32] + times38(t[i]); 
  }
  r[31] = t[31]; /* result now in r[0]...r[31] */

  reduce_mul(r);
}


function fe25519_mul(r, x, y)
{
  let i,j;
  let t = [];
  for(i=0;i<63;i++)t[i] = 0;

  for(i=0;i<32;i++)
    for(j=0;j<32;j++)
      t[i+j] += x[i] * y[j];

  for(i=32;i<63;i++)
    r[i-32] = t[i-32] + times38(t[i]); 
  r[31] = t[31]; /* result now in r[0]...r[31] */

  reduce_mul(r);
}


(function(){
  console.log("Karasuba");
  let o = [];
  let a = [...Array(32)].map(d => 10);
  let b = [...Array(32)].map(d => 2);
  console.log(a, b);
  kara(o, a, b);
  console.log(o);
  console.log("---------------");
})();


(function(){
  console.log("Standard");
  let o = [];
  let a = [...Array(32)].map((_, i) => i === 10 ? 3 : 0);
  let b = [...Array(32)].map((_, i) => i === 10 ? 2 : 0);
  console.log(a, b);
  fe25519_mul(o, a, b);
  console.log(o);
  console.log("---------------");
})();


// https://github.com/srijanshetty/DS/blob/master/practice/karatsuba.c

function karatArr(arr1, arr2, size, s)
{
  let mid=size/2;
  mid=size-mid;
    
  let o1, o2, o3, o4, o5;//Temporary variables

  //variables to store size
  let s1, s2, s3, s4, s5;
  o1=karatArr(array1, array2, mid, s1);//ac
  o2=karatArr((array1+mid), (array2+mid), (size-mid), s2);//bd
  o4=add(array1, mid, (array1+mid), (size-mid), s4);//(a+b)
  o5=add(array2, mid, (array2+mid), (size-mid), s5);//(c+d)
  o3=karatArr(o4,o5,s4, s3);//(a+b)(c+d)
  o4=add(o1,s1, o2, s2, s4);//ac+bd
  o5=subtract(o3, s3, o4, s4, s5);//(a+b)(c+d)-ac-bd
  o1=shift(o1, s1, 2*(size-mid), s1);//ac*2^2m
  o2=add(o1, s1, o2, s2, s2);//ac*2^2m+bd
  o5=shift(o5, s5, (size-mid), s5);//(ad+bc)*2^m
		
		result=add(o5, s5, o2, s2, s);//ac*2^2m+(ad+bc)*2^m+bd	
		
		return result; 

  // int mid=size/2;
	// 	mid=size-mid;
			
	// 	int *o1, *o2, *o3, *o4, *o5;//Temporary variables
		
	// 	//variables to store size
	// 	int s1, s2, s3, s4, s5;
	// 	o1=karatsuba(array1, array2, mid, &s1);//ac
	// 	o2=karatsuba((array1+mid), (array2+mid), (size-mid), &s2);//bd
	// 	o4=add(array1, mid, (array1+mid), (size-mid), &s4);//(a+b)
	// 	o5=add(array2, mid, (array2+mid), (size-mid), &s5);//(c+d)
	// 	o3=karatsuba(o4,o5,s4, &s3);//(a+b)(c+d)
	// 	o4=add(o1,s1, o2, s2, &s4);//ac+bd
	// 	o5=subtract(o3, s3, o4, s4, &s5);//(a+b)(c+d)-ac-bd
	// 	o1=shift(o1, s1, 2*(size-mid), &s1);//ac*2^2m
	// 	o2=add(o1, s1, o2, s2, &s2);//ac*2^2m+bd
	// 	o5=shift(o5, s5, (size-mid), &s5);//(ad+bc)*2^m
		
	// 	result=add(o5, s5, o2, s2, s);//ac*2^2m+(ad+bc)*2^m+bd	
		
	// 	return result; 
}

function karatOwn(x, y)
{

  var x1,x0,y1,y0,base,m;
  base  = 10;


  if((x < base) || (y < base)){
    // console.log( " X - y = " , x,y, x*y)
    return x * y;
  }

  var dummy_x = x.toString();
  var dummy_y = y.toString();

  var n = (dummy_x.length > dummy_y.length) ? dummy_y.length : dummy_x.length;
  m = Math.round(n/2);



  var high1 = parseInt(dummy_x.substring(0,dummy_x.length-m));
  var low1 = parseInt(dummy_x.substring(dummy_x.length-m,dummy_x.length  )) ;

  var high2 = parseInt(dummy_y.substring(0,dummy_y.length-m)); 
  var low2 = parseInt(dummy_y.substring(dummy_y.length-m,dummy_y.length));


  var z0   =   karatOwn( low1, low2);
  var z1   =   karatOwn(low1+high1, low2+high2);
  var z2   =   karatOwn(high1,high2);

  var res  =   (z2 *  Math.pow(10, 2 * m )  ) + ( (z1-z2-z0) * Math.pow(10,  m )) + z0;

  return res;

 }

function add(out, a, b, length)
{
  let c = 0;
  for(let i = length; i >= 0; i--)
  {
    out[i] = (a[i] + b[i] + c) & 0x7FFFFFFF;
    c = (a[i] + b[i] + c) > 0x7FFFFFFF;
    // c = a[i] & b[i];
    if(c)
    {
      console.log("Has carry11!", c, a[i], b[i]);
    }
    // let middle = a[i] + b[i];
    // c = c | (middle & c);
    // if(c)
    // {
    //   console.log("Has carry!", c);
    // }
  }
}

(function(){
  console.log("own");
  let o = [];
  let a = [...Array(32)].map((_, i) => i === 10 ? 0x7FFFFFFF : 0);
  let b = [...Array(32)].map((_, i) => i === 10 ? 2 : 0);
  console.log(3000000000, 2000000000);
  
  // console.log( karatOwn(3000000000, 2000000000));
  add(o, a, b, 32);
  console.log(o);
  console.log("---------------");
})();

// def karat(x,y):
//     if len(str(x)) == 1 or len(str(y)) == 1:
//         return x*y
//     else:
//         m = max(len(str(x)),len(str(y)))
//         m2 = m // 2

//         a = x // 10**(m2)
//         b = x % 10**(m2)
//         c = y // 10**(m2)
//         d = y % 10**(m2)

//         z0 = karat(b,d)
//         z1 = karat((a+b),(c+d))
//         z2 = karat(a,c)

//         return (z2 * 10**(2*m2)) + ((z1 - z2 - z0) * 10**(m2)) + (z0)