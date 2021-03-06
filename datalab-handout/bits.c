/* 
 * CS:APP Data Lab 
 * 
 * Stuart
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  return ~(~x | ~y);
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  int n_bytes = n << 3;
  int whole_byte = 0xff;
  int mask = whole_byte << n_bytes;
  int isolated = x & mask;
  int truncated = isolated >> n_bytes;
  int last_byte_mask = 0x000000ff;

  return truncated & last_byte_mask;
}
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
  int shifted = x >> n;
  int leftmost_one = 0x1 << 31;
  int n_minus_one_ones = (leftmost_one >> n) << 1;
  int mask = ~n_minus_one_ones;

  return shifted & mask;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  int two_bits = (0x55 << 24) + (0x55 << 16) + (0x55 << 8) + 0x55;
  int four_bits = (0x33 << 24) + (0x33 << 16) + (0x33 << 8) + 0x33;
  int eight_bits = (0x0f << 24) + (0x0f << 16) + (0x0f << 8) + 0x0f;
  int sixteen_bits = (0xff << 16) + 0xff;
  int thirty_two_bits = (0xff << 8) + 0xff;

  int count = x;
  count = (count & two_bits) + ((count >> 1) & two_bits);
  count = (count & four_bits) + ((count >> 2) & four_bits);
  count = (count + (count >> 4)) & eight_bits;
  count = (count + (count >> 8)) & sixteen_bits;
  count = (count + (count >> 16)) & thirty_two_bits;

  return count;
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  int present = x;
  present = present | (present >> 1);
  present = present | (present >> 2);
  present = present | (present >> 4);
  present = present | (present >> 8);
  present = present | (present >> 16);

  return (present & 0x1) ^ 0x1;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 0x1 << 31;
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  int negative_one = ~0;
  int sign_bit = 0x1 << 31 & x;
  int extended_sign_bit = sign_bit >> 31 << (n + negative_one);
  int mask_n_bits = ~0 << (n + negative_one);
  return !((x & mask_n_bits) ^ extended_sign_bit);
}
/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
  int negative_one = ~0;
  int odd_mask = (0x1 << n) + negative_one;
  int odd = !!(odd_mask & x);
  int negative = (x >> 31) & 0x1;
  int round_up = odd & negative & (!!n);
  return (x >> n) + round_up;
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return ~x + 1;
}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
  int sign_bit = x & (0x1 << 31);
  int zero = !x;
  return !sign_bit & !zero;
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  int sign_bit = 1 << 31;
  int xor = x ^ y;
  int equal = !xor;
  int different_signs = !!(xor & sign_bit);
  int x_less_than_zero = !!(x & sign_bit);
  int x_minus_y = x + (~y + 1);
  int difference_less_than_zero = !!(x_minus_y & sign_bit);

  return equal |
         (!different_signs & difference_less_than_zero) |
         (different_signs & x_less_than_zero);
}
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
  int negative_one = ~0;
  int take_sixteen = (0xff << 8) + 0xff;
  int take_eight = 0xff;
  int take_four = 0xf;
  int take_two = 0x3;
  int take_one = 0x1;


  int working_bits = x;
  int top_half = (working_bits >> 16) & take_sixteen;
  int bottom_half = working_bits & take_sixteen;
  int top_mask = !top_half + negative_one;
  int bottom_mask = ~top_mask;
  int base = base + (top_mask & 0x10);
  working_bits = (top_half & top_mask) | (bottom_half & bottom_mask);


  top_half = (working_bits >> 8) & take_eight;
  bottom_half = working_bits & take_eight;
  top_mask = !top_half + negative_one;
  bottom_mask = ~top_mask;
  base = base + (top_mask & 0x8);
  working_bits = (top_half & top_mask) | (bottom_half & bottom_mask);


  top_half = (working_bits >> 4) & take_four;
  bottom_half = working_bits & take_four;
  top_mask = !top_half + negative_one;
  bottom_mask = ~top_mask;
  base = base + (top_mask & 0x4);
  working_bits = (top_half & top_mask) | (bottom_half & bottom_mask);


  top_half = (working_bits >> 2) & take_two;
  bottom_half = working_bits & take_two;
  top_mask = !top_half + negative_one;
  bottom_mask = ~top_mask;
  base = base + (top_mask & 0x2);
  working_bits = (top_half & top_mask) | (bottom_half & bottom_mask);


  top_half = (working_bits >> 1) & take_one;
  bottom_half = working_bits & take_one;
  top_mask = !top_half + negative_one;
  base = base + (top_mask & 0x1);
  working_bits = (top_half & top_mask) | (bottom_half & bottom_mask);

  return base;
}
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
  int exponent_mask = 0xff << 23;
  int fraction_mask = (0x1 << 23) - 1;
  if (((exponent_mask & uf) == exponent_mask) && (fraction_mask & uf)) {
    return uf;
  } else {
    return uf ^ (0x1 << 31);
  }
}
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
  int sign_bit = 0x80000000;

  if (x == 0) {
    return 0x0;
  } else if (x == sign_bit) {
    return 0xcf000000;
  }

  int sign, abs, exp, frac;
  int truncated_bits, pad, num_digits, mask, first_truncated_bit;
  sign = x & sign_bit;
  abs = sign? -x : x;
  frac = abs;
  exp = 0;
  truncated_bits = 0;
  pad = 0;

  while (abs >>= 1) {
      exp ++;
  }

  num_digits = exp - 23;

  if (exp > 23) {
      mask = ~(sign_bit >> (31 - num_digits));
      truncated_bits = frac & mask;
      first_truncated_bit = 0x1 << (num_digits - 1);
      if (truncated_bits == first_truncated_bit) {
        if (frac >> num_digits & 0x1) {
              pad = 1;
        }
      } else {
          pad = truncated_bits > first_truncated_bit;
      }

      frac = (frac >> num_digits) + pad;

      if (frac & 0x01000000) {
          exp++;
      }
  }
  else {
      frac = frac << (-num_digits);
  }

  frac = frac & 0x7fffff; /* set the sign and exponent bits to zero */ 

  exp += 127; /* add the bias to the exp */

  return sign | (exp << 23) | frac;
}
/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
  int exponent_mask = 0xff << 23;
  int fraction_mask = (0x1 << 23) - 1;
  int exponent = exponent_mask & uf;

  if (exponent == exponent_mask) {
    return uf;
  }
  if (uf == 0x0) {
    return uf;
  }
  if (uf == 0x80000000) {
    return uf;
  }

  if (exponent == 0 && (uf & fraction_mask) < (fraction_mask + 1)) {
    return (uf & ~fraction_mask) | ((uf & fraction_mask) * 2);
  }
  return uf + (0x1 << 23);
}
