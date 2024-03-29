# Karatsuba and Curve25519 on RISC-V
Curve25519 has been ported to RISC-V. The Karatsuba multiplication algorithm is used to optimize the speed of the proccess. It is built on the [PQVexRiscV](https://github.com/mupq/pqriscv-vexriscv) platform.

## Tests
Open the device, with the test program loaded.
```bash
sbt "runMain mupq.PQVexRiscvSim --ram 256,128 --init <path/to/repo>/build/test.bin"
```
The test scripts is located in the `./test/` directory.
```bash
cd test
npm rum start <devpts-port>
```

## Report
To compile the report
```bash
cd report
make
```

## Requirements
- [PQVexRiscV](https://github.com/mupq/pqriscv-vexriscv)
- [NodeJS](https://nodejs.org/en/)


<!-- ## Litterature

 - 2006, Curve25519: new Diffie-Hellman speed records, https://cr.yp.to/ecdh/curve25519-20060209.pdf
 - 2014, High-speed Curve25519 on 8-bit, 16-bit, and 32-bit microcontrollers, https://link.springer.com/content/pdf/10.1007/s10623-015-0087-1.pdf
 - 2019, Curve25519 for the Cortex-M4 and Beyond, https://link.springer.com/chapter/10.1007/978-3-030-25283-0_6
 - Elliptic Curves Number Theory and Cryptography, http://people.cs.nctu.edu.tw/~rjchen/ECC2012S/Elliptic%20Curves%20Number%20Theory%20And%20Cryptography%202n.pdf


 ## Usefull/inspiration links

 - Original implementation by Bernstein, https://cr.yp.to/ecdh.html
 - Useful Crypto-related Resources, https://github.com/claucece/useful-crypto-resources#elliptic-curve
 - https://news.ycombinator.com/item?id=20382223
 - http://www.cs.ru.nl/bachelors-theses/2020/Mauk_Lemmen___4798937___Optimizing_Elephant_for_RISC-V.pdf
 - https://www.cs.ru.nl/bachelors-theses/2019/Lars_Jellema___4388747___Optimizing_Ascon_on_RISC-V.pdf
 - https://eprint.iacr.org/2020/836.pdf -->


 ## Credits
 
 - Thanks to [Mupq](https://github.com/mupq), for supplying the template. 