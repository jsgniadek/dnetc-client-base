// Copyright distributed.net 1997 - All Rights Reserved
// For use in distributed.net projects only.
// Any other distribution or use of this source violates copyright.
//
// $Log: csc-common.cpp,v $
// Revision 1.1.2.3  1999/11/01 17:23:23  cyp
// renamed transX(...) to csc_transX(...) to avoid potential (future) symbol
// collisions.
//
// Revision 1.1.2.2  1999/10/24 23:54:54  remi
// Use Problem::core_membuffer instead of stack for CSC cores.
// Align frequently used memory to 16-byte boundary in CSC cores.
//
// Revision 1.1.2.1  1999/10/07 18:41:14  cyp
// sync'd from head
//
// Revision 1.1  1999/07/23 02:43:06  fordbr
// CSC cores added
//
//

#define __IN_CSC_COMMON_CPP

#include "csc-common.h"

// ------------------------------------------------------------------
// bitslice version of c0..c8
const ulong csc_tabc[9][64]
#if defined(__GNUC__)
__attribute ((aligned (16)))
#endif
 = {
  { // c0 == 0x290d61409ceb9e8f
    _1, _1, _1, _1, _0, _0, _0, _1, _0, _1, _1, _1, _1, _0, _0, _1, 
    _1, _1, _0, _1, _0, _1, _1, _1, _0, _0, _1, _1, _1, _0, _0, _1, 
    _0, _0, _0, _0, _0, _0, _1, _0, _1, _0, _0, _0, _0, _1, _1, _0, 
    _1, _0, _1, _1, _0, _0, _0, _0, _1, _0, _0, _1, _0, _1, _0, _0  },
  { // c1 == 0x1f855f585b013986
    _0, _1, _1, _0, _0, _0, _0, _1, _1, _0, _0, _1, _1, _1, _0, _0, 
    _1, _0, _0, _0, _0, _0, _0, _0, _1, _1, _0, _1, _1, _0, _1, _0, 
    _0, _0, _0, _1, _1, _0, _1, _0, _1, _1, _1, _1, _1, _0, _1, _0, 
    _1, _0, _1, _0, _0, _0, _0, _1, _1, _1, _1, _1, _1, _0, _0, _0  },
  { // c2 == 0x972ed7d635ae1716
    _0, _1, _1, _0, _1, _0, _0, _0, _1, _1, _1, _0, _1, _0, _0, _0, 
    _0, _1, _1, _1, _0, _1, _0, _1, _1, _0, _1, _0, _1, _1, _0, _0, 
    _0, _1, _1, _0, _1, _0, _1, _1, _1, _1, _1, _0, _1, _0, _1, _1, 
    _0, _1, _1, _1, _0, _1, _0, _0, _1, _1, _1, _0, _1, _0, _0, _1  },
  { // c3 == 0x21b6694ea5728708
    _0, _0, _0, _1, _0, _0, _0, _0, _1, _1, _1, _0, _0, _0, _0, _1, 
    _0, _1, _0, _0, _1, _1, _1, _0, _1, _0, _1, _0, _0, _1, _0, _1, 
    _0, _1, _1, _1, _0, _0, _1, _0, _1, _0, _0, _1, _0, _1, _1, _0, 
    _0, _1, _1, _0, _1, _1, _0, _1, _1, _0, _0, _0, _0, _1, _0, _0  },
  { // c4 == 0x3c18e6e7faadb889
    _1, _0, _0, _1, _0, _0, _0, _1, _0, _0, _0, _1, _1, _1, _0, _1, 
    _1, _0, _1, _1, _0, _1, _0, _1, _0, _1, _0, _1, _1, _1, _1, _1, 
    _1, _1, _1, _0, _0, _1, _1, _1, _0, _1, _1, _0, _0, _1, _1, _1, 
    _0, _0, _0, _1, _1, _0, _0, _0, _0, _0, _1, _1, _1, _1, _0, _0  },
  { // c5 == 0xb700f76f73841163
    _1, _1, _0, _0, _0, _1, _1, _0, _1, _0, _0, _0, _1, _0, _0, _0, 
    _0, _0, _1, _0, _0, _0, _0, _1, _1, _1, _0, _0, _1, _1, _1, _0, 
    _1, _1, _1, _1, _0, _1, _1, _0, _1, _1, _1, _0, _1, _1, _1, _1, 
    _0, _0, _0, _0, _0, _0, _0, _0, _1, _1, _1, _0, _1, _1, _0, _1  },
  { // c6 == 0x3f967f6ebf149dac
    _0, _0, _1, _1, _0, _1, _0, _1, _1, _0, _1, _1, _1, _0, _0, _1, 
    _0, _0, _1, _0, _1, _0, _0, _0, _1, _1, _1, _1, _1, _1, _0, _1, 
    _0, _1, _1, _1, _0, _1, _1, _0, _1, _1, _1, _1, _1, _1, _1, _0, 
    _0, _1, _1, _0, _1, _0, _0, _1, _1, _1, _1, _1, _1, _1, _0, _0  },
  { // c7 == 0xa40e7ef6204a6230
    _0, _0, _0, _0, _1, _1, _0, _0, _0, _1, _0, _0, _0, _1, _1, _0, 
    _0, _1, _0, _1, _0, _0, _1, _0, _0, _0, _0, _0, _0, _1, _0, _0, 
    _0, _1, _1, _0, _1, _1, _1, _1, _0, _1, _1, _1, _1, _1, _1, _0, 
    _0, _1, _1, _1, _0, _0, _0, _0, _0, _0, _1, _0, _0, _1, _0, _1  },
  { // c8 == 0x03c54b5a46a34465
    _1, _0, _1, _0, _0, _1, _1, _0, _0, _0, _1, _0, _0, _0, _1, _0, 
    _1, _1, _0, _0, _0, _1, _0, _1, _0, _1, _1, _0, _0, _0, _1, _0, 
    _0, _1, _0, _1, _1, _0, _1, _0, _1, _1, _0, _1, _0, _0, _1, _0, 
    _1, _0, _1, _0, _0, _0, _1, _1, _1, _1, _0, _0, _0, _0, _0, _0  } 
};

// bitslice version of e and e'
const ulong csc_tabe[2][64]
#if defined(__GNUC__)
__attribute ((aligned (16)))
#endif
 = {
  { // e == 0xb7e151628aed2a6a
    _0, _1, _0, _1, _0, _1, _1, _0, _0, _1, _0, _1, _0, _1, _0, _0, 
    _1, _0, _1, _1, _0, _1, _1, _1, _0, _1, _0, _1, _0, _0, _0, _1, 
    _0, _1, _0, _0, _0, _1, _1, _0, _1, _0, _0, _0, _1, _0, _1, _0, 
    _1, _0, _0, _0, _0, _1, _1, _1, _1, _1, _1, _0, _1, _1, _0, _1  },
  { // e' == 0xbf7158809cf4f3c7
    _1, _1, _1, _0, _0, _0, _1, _1, _1, _1, _0, _0, _1, _1, _1, _1, 
    _0, _0, _1, _0, _1, _1, _1, _1, _0, _0, _1, _1, _1, _0, _0, _1, 
    _0, _0, _0, _0, _0, _0, _0, _1, _0, _0, _0, _1, _1, _0, _1, _0, 
    _1, _0, _0, _0, _1, _1, _1, _0, _1, _1, _1, _1, _1, _1, _0, _1  },
};

// table-lookup implementation of csc_transP()
const u8 csc_tabp[256]
#if defined(__GNUC__)
__attribute ((aligned (16)))
#endif
 = {
  0x29,0x0d,0x61,0x40,0x9c,0xeb,0x9e,0x8f, 0x1f,0x85,0x5f,0x58,0x5b,0x01,0x39,0x86,
  0x97,0x2e,0xd7,0xd6,0x35,0xae,0x17,0x16, 0x21,0xb6,0x69,0x4e,0xa5,0x72,0x87,0x08,
  0x3c,0x18,0xe6,0xe7,0xfa,0xad,0xb8,0x89, 0xb7,0x00,0xf7,0x6f,0x73,0x84,0x11,0x63,
  0x3f,0x96,0x7f,0x6e,0xbf,0x14,0x9d,0xac, 0xa4,0x0e,0x7e,0xf6,0x20,0x4a,0x62,0x30,
  0x03,0xc5,0x4b,0x5a,0x46,0xa3,0x44,0x65, 0x7d,0x4d,0x3d,0x42,0x79,0x49,0x1b,0x5c,
  0xf5,0x6c,0xb5,0x94,0x54,0xff,0x56,0x57, 0x0b,0xf4,0x43,0x0c,0x4f,0x70,0x6d,0x0a,
  0xe4,0x02,0x3e,0x2f,0xa2,0x47,0xe0,0xc1, 0xd5,0x1a,0x95,0xa7,0x51,0x5e,0x33,0x2b,
  0x5d,0xd4,0x1d,0x2c,0xee,0x75,0xec,0xdd, 0x7c,0x4c,0xa6,0xb4,0x78,0x48,0x3a,0x32,
  0x98,0xaf,0xc0,0xe1,0x2d,0x09,0x0f,0x1e, 0xb9,0x27,0x8a,0xe9,0xbd,0xe3,0x9f,0x07,
  0xb1,0xea,0x92,0x93,0x53,0x6a,0x31,0x10, 0x80,0xf2,0xd8,0x9b,0x04,0x36,0x06,0x8e,
  0xbe,0xa9,0x64,0x45,0x38,0x1c,0x7a,0x6b, 0xf3,0xa1,0xf0,0xcd,0x37,0x25,0x15,0x81,
  0xfb,0x90,0xe8,0xd9,0x7b,0x52,0x19,0x28, 0x26,0x88,0xfc,0xd1,0xe2,0x8c,0xa0,0x34,
  0x82,0x67,0xda,0xcb,0xc7,0x41,0xe5,0xc4, 0xc8,0xef,0xdb,0xc3,0xcc,0xab,0xce,0xed,
  0xd0,0xbb,0xd3,0xd2,0x71,0x68,0x13,0x12, 0x9a,0xb3,0xc2,0xca,0xde,0x77,0xdc,0xdf,
  0x66,0x83,0xbc,0x8d,0x60,0xc6,0x22,0x23, 0xb2,0x8b,0x91,0x05,0x76,0xcf,0x74,0xc9,
  0xaa,0xf1,0x99,0xa8,0x59,0x50,0x3b,0x2a, 0xfe,0xf9,0x24,0xb0,0xba,0xfd,0xf8,0x55,
};

