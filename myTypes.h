#ifndef myTypes
#define myTypes

typedef char I8;          // from -128 to 127
typedef unsigned char U8; // from 0 to 255

typedef short int I16;          // from -32768 to 32767
typedef unsigned short int U16; // from 0 to 65535

typedef int I32;          // from -2147483648 to 2147483647
typedef unsigned int U32; // from 0 to 4294967295

typedef long int I64;          // from -9223372036854775808 to 9223372036854775807
typedef unsigned long int U64; // from 0 to 18446744073709551616

typedef float R32;        // from -3,4028235E+38 to 3.4028235E+38
typedef double R64;       // from 2.2250738585072014E-308 to 1,7976931348623158E+308
typedef long double R128; // from 0.24651903288156618919116517665087070E-31 to 0.1797693134862315907729305190789002575e+309

typedef enum
{
    false,
    true
} boolean;

#endif