/* Ian McGregor
 *  Data Lab
 *   CSCI 2400 

/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {

	int z = (~(~x|~y));
	//demorgans law with one distributed tilde
	return z;
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
  
	int byte = (x >> 8*n) & 0xff;
	// hex values are 8 bits per character, or 1 byte. so if we want to extract the nth bit, we must right shift by 8 * n


  return byte;
}
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {

	int testN = !n + (~0);
	//~0 = 0xFFFFFFFF and !n returns 0 if non-zero integer, therefore creating 0xFFFFFFFF
	int shift = (x >> n);
	// right shift by n bits
	int mask = ((~n+1)+32);
	int shiftMask = shift & (~((~0) << mask));
	int result = (testN & shiftMask)| (~testN & shift);

return result;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {

    return x;
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {

	int negx = ~x + 1;
	// compute negative x
	// ~x flips bits resulting in x being -x-1
	// +1 gets us to -x
	x = (x | negx) >> 31;
	// | of x and -x results in MSB being 1 if x > 0 and 0 if x = 0
	// >> 31 extracts MSB for a 32 bit (4 byte) int
	return x + 1;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 1 << 31;
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
	//compute -1 since we cant use '-'
	int negOne = ~1 + 1;
	//compute n-1
	int nMinus1 = n + negOne;
	//shift x by n - 1
	int shiftX = x >> nMinus1;
	//add one to shifted x since we now have the MSB in range
    //shift right by 1 after adding 1
	int bin = (shiftX + 1) >> 1;
    //if 1 it does not fit
    //if 0 it does fit
    //therefore we need a !
	int ret = !bin;

	return ret;
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
	// returns 0 if positive, and 1 if negative
    int sign = !((x>>31)+1);
    // n # of 1's
    //~(~0 << n) -> 00...011 with n number of 1's
    int nOnes= ~(~0 << n); 
    // returns 0 if does not need rounding, 1 if needs rounding toward zero
    int toRound = !(!(nOnes & x));
    // if sign is positive default rounding ensues, and no rounding is necessary
    //if sign is negative(sign = 1), and rounding is necessary(toRound = 1)
    //then we add one to the negative number that needs rounding toward zero!
    int ret = (sign & toRound) + (x >> n); 
    return ret;
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x){
 	
	x = ~x + 1;
	//flips bits then adds 1, resulting in the negative of 
    //x for a two's compliment int (signed int)
	return x;
}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
  
  int bangX = !x;
  //for comparison
  int result = !(x >> 31 | bangX);;

  return result;
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
	//extract 31st bit for sign representation
	//0xFFFFFFFF if - 0x0 if +
	int signX = x >> 31;
	int signY = y >> 31;

	// Check for overflow....
	// returns 0 if x & y are the same sign or if only y is negative
	// returns 1 if x is negative and y is positive
	int negX = ~(signX & ~signY) + 1;
	int lessThan = signX | ~signY;

	// Compute twos compliment -(x)
	int xTwosCompliment = (~x + 1);

	//compute y-x, then extract sign bit
	int yMinusX = ((y + xTwosCompliment) >> 31) + 1;

	int ret = lessThan & (negX | yMinusX);
	return ret;
}
/*
 * extra credit
 */
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
  return 2;
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
 return 2;
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
  return 2;
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
  return 2;
}
