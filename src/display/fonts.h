#ifndef FONTS_H
#define FONTS_H

#include <Arduino.h>


struct Font {
    uint8_t first_ch;
    uint8_t last_ch;
    const uint16_t *data;
    const uint16_t *offsets;
};



const uint8_t F1_first_ch = 32;
const uint8_t F1_last_ch = 126;

const uint16_t F1_chardata[] PROGMEM = {
    0x0,0x0,0x0,    //32
    0x0,0xbe,0x0,   //33 !
    0xe,0x0,0xe,0x0,        //34 "
    0xe8,0x38,0xee,0x38,0x2e,0x0,   //35 #
    0x4c,0x8a,0x1fe,0x92,0x64,0x0,  //36 $
    0x0,0xc,0x12,0xcc,0x30,0x6c,0x92,0x60,0x0,      //37 %
    0x60,0x94,0x9a,0xaa,0x46,0xe0,0xa0,     //38 &
    0xe,0x0,        //39 '
    0xf8,0x104,0x202,       //40 (
    0x202,0x104,0xf8,       //41 )
    0xa,0x6,0xa,0x0,        //42 *
    0x10,0x10,0x7c,0x10,0x10,0x0,   //43 +
    0x0,0x180,0x0,  //44 ,
    0x20,0x20,0x0,  //45 -
    0x0,0x80,0x0,   //46 .
    0xc0,0x38,0x6,  //47 /
    0x7c,0x82,0x82,0x82,0x7c,0x0,   //48 0
    0x0,0x8,0x4,0xfe,0x0,0x0,       //49 1
    0x84,0xc2,0xa2,0x92,0x8c,0x0,   //50 2
    0x44,0x82,0x92,0x92,0x6c,0x0,   //51 3
    0x30,0x28,0x24,0x22,0xfe,0x20,  //52 4
    0x58,0x8e,0x8a,0x8a,0x72,0x0,   //53 5
    0x7c,0x8a,0x8a,0x8a,0x74,0x0,   //54 6
    0x2,0xc2,0x32,0xe,0x2,0x0,      //55 7
    0x6c,0x92,0x92,0x92,0x6c,0x0,   //56 8
    0x5c,0xa2,0xa2,0xa2,0x7c,0x0,   //57 9
    0x0,0x88,0x0,   //58 :
    0x0,0x188,0x0,  //59 ;
    0x0,0x10,0x28,0x28,0x44,0x0,    //60 <
    0x28,0x28,0x28,0x28,0x28,0x0,   //61 =
    0x0,0x44,0x28,0x28,0x10,0x0,    //62 >
    0x4,0x2,0x2,0xb2,0x12,0xc,      //63 ?
    0x0,0xf0,0x10c,0x274,0x28a,0x28a,0x2f2,0x29a,0x144,0x138,       //64 @
    0x80,0x70,0x2c,0x22,0x2c,0x70,0x80,     //65 A
    0x0,0xfe,0x92,0x92,0x92,0x7c,0x0,       //66 B
    0x0,0x7c,0x82,0x82,0x82,0x44,0x0,       //67 C
    0x0,0xfe,0x82,0x82,0x44,0x38,0x0,       //68 D
    0x0,0xfe,0x92,0x92,0x92,0x92,0x0,       //69 E
    0x0,0xfe,0x12,0x12,0x2,0x0,     //70 F
    0x0,0x38,0x44,0x82,0x92,0x54,0x30,0x0,  //71 G
    0x0,0xfe,0x10,0x10,0x10,0xfe,0x0,       //72 H
    0x0,0xfe,0x0,   //73 I
    0xc0,0x80,0x80,0x7e,0x0,        //74 J
    0x0,0xfe,0x20,0x18,0x24,0x42,0x80,      //75 K
    0x0,0xfe,0x80,0x80,0x80,0x80,   //76 L
    0x0,0xfe,0xc,0x70,0x80,0x70,0xc,0xfe,   //77 M
    0x0,0xfe,0xc,0x10,0x60,0xfe,0x0,        //78 N
    0x0,0x7c,0x82,0x82,0x82,0x82,0x7c,0x0,  //79 O
    0x0,0xfe,0x12,0x12,0x12,0xc,0x0,        //80 P
    0x0,0x7c,0x82,0x82,0xc2,0xc2,0xbc,0x0,  //81 Q
    0x0,0xfe,0x12,0x12,0x32,0xcc,0x0,       //82 R
    0x0,0x4c,0x92,0x92,0x92,0x64,0x0,       //83 S
    0x0,0x2,0x2,0xfe,0x2,0x2,       //84 T
    0x0,0x7e,0x80,0x80,0x80,0x7e,0x0,       //85 U
    0x2,0x1c,0x60,0x80,0x60,0x1c,0x2,       //86 V
    0x6,0x78,0x80,0x7c,0x2,0x7c,0x80,0x78,0x6,      //87 W
    0x80,0x42,0x24,0x18,0x24,0x42,0x80,     //88 X
    0x2,0x4,0x8,0xf0,0x8,0x4,0x2,   //89 Y
    0xc2,0xa2,0xb2,0x9a,0x8e,0x86,  //90 Z
    0x0,0x3fe,0x202,        //91 [
    0x6,0x38,0xc0,          //92 backslash
    0x202,0x3fe,0x0,        //93 ]
    0x10,0xc,0x2,0xc,0x10,  //94 ^
    0x200,0x200,0x200,0x200,0x200,0x200,    //95 _
    0x2,0x4,0x0,    //96 `
    0x0,0xc0,0xa8,0xa8,0xf8,0x0,    //97 a
    0x0,0xfe,0x88,0x88,0x70,0x0,    //98 b
    0x0,0x70,0x88,0x88,0x50,        //99 c
    0x0,0x70,0x88,0x88,0xfe,0x0,    //100 d
    0x0,0x70,0xa8,0xa8,0xb0,0x0,    //101 e
    0x8,0xfc,0xa,   //102 f
    0x0,0x170,0x288,0x288,0x1f8,0x0,        //103 g
    0x0,0xfe,0x8,0x8,0xf0,0x0,      //104 h
    0x0,0xfa,       //105 i
    0x200,0x1fa,    //106 j
    0xfe,0x20,0x70,0x88,0x0,        //107 k
    0x0,0xfe,       //108 l
    0x0,0xf8,0x8,0x8,0xf8,0x8,0x8,0xf0,     //109 m
    0x0,0xf8,0x8,0x8,0xf0,0x0,      //110 n
    0x0,0x70,0x88,0x88,0x70,0x0,    //111 o
    0x0,0x3f8,0x88,0x88,0x70,0x0,   //112 p
    0x0,0x70,0x88,0x88,0x3f8,0x0,   //113 q
    0x0,0xf8,0x8,   //114 r
    0x90,0xa8,0xa8,0x48,0x0,        //115 s
    0x8,0xfe,0x88,  //116 t
    0x0,0x78,0x80,0x80,0xf8,0x0,    //117 u
    0x8,0x70,0x80,0x70,0x8, //118 v
    0x78,0x80,0x70,0x8,0x70,0x80,0x78,      //119 w
    0x88,0x50,0x20,0x50,0x88,       //120 x
    0x8,0x270,0x180,0x70,0x8,       //121 y
    0x88,0xc8,0xa8,0x98,0x88,       //122 z
    0x20,0x3de,0x202,       //123 {
    0x0,0x3fe,0x0,  //124 |
    0x202,0x3de,0x20,       //125 }
    0x30,0x10,0x30,0x20,0x30,0x0   //126 ~
};

const uint16_t F1_charoffsets[] PROGMEM = {
    0x00, 0x03, 0x06, 0x0a, 0x10, 0x16, 0x1f, 0x26, 0x28, 0x2b, 0x2e, 0x32, 0x38, 0x3b, 0x3e, 0x41, 
    0x44, 0x4a, 0x50, 0x56, 0x5c, 0x62, 0x68, 0x6e, 0x74, 0x7a, 0x80, 0x83, 0x86, 0x8c, 0x92, 0x98, 
    0x9e, 0xa8, 0xaf, 0xb6, 0xbd, 0xc4, 0xcb, 0xd1, 0xd9, 0xe0, 0xe3, 0xe8, 0xef, 0xf5, 0xfd, 0x104, 
    0x10c, 0x113, 0x11b, 0x122, 0x129, 0x12f, 0x136, 0x13d, 0x146, 0x14d, 0x154, 0x15a, 0x15d, 0x160, 0x163, 0x168, 
    0x16e, 0x171, 0x177, 0x17d, 0x182, 0x188, 0x18e, 0x191, 0x197, 0x19d, 0x19f, 0x1a1, 0x1a6, 0x1a8, 0x1b0, 0x1b6, 
    0x1bc, 0x1c2, 0x1c8, 0x1cb, 0x1d0, 0x1d3, 0x1d9, 0x1de, 0x1e5, 0x1ea, 0x1ef, 0x1f4, 0x1f7, 0x1fa, 0x1fd, 
    0x203   //end of table
};

const uint8_t F2_first_ch = 32;
const uint8_t F2_last_ch = 126;


const uint16_t F2_chardata[] PROGMEM = {
    0x0,0x0,0x0,0x0,        //32
    0x0,0x0,0xbff,0x0,      //33 !
    0x0,0xf,0x0,0x0,0xf,0x0,        //34 "
    0x110,0xf10,0x1f8,0x117,0x110,0xf10,0x1f8,0x117,0x110,  //35 #
    0x0,0x21c,0x422,0x821,0x1fff,0x841,0x442,0x384,0x0,     //36 $
    0x0,0x1e,0x21,0x21,0xc21,0x31e,0xc0,0x38,0x786,0x841,0x840,0x840,0x780,0x0,     //37 %
    0x0,0x380,0x44e,0x831,0x861,0x891,0x50e,0x200,0x500,0x880,0x0,  //38 &
    0x0,0xf,0x0,    //39 '
    0x0,0x7f0,0x380e,0x4001,0x0,    //40 (
    0x0,0x4001,0x380e,0x7f0,0x0,    //41 )
    0x2,0x1a,0x7,0x1a,0x2,0x0,      //42 *
    0x0,0x40,0x40,0x40,0x3f8,0x40,0x40,0x40,0x0,    //43 +
    0x0,0x0,0x3800,0x0,     //44 ,
    0x100,0x100,0x100,0x100,0x0,    //45 -
    0x0,0x0,0x800,0x0,      //46 .
    0xc00,0x3c0,0x3c,0x3,   //47 /
    0x0,0x3fc,0x402,0x801,0x801,0x801,0x402,0x3fc,0x0,      //48 0
    0x0,0x0,0x8,0x4,0x2,0xfff,0x0,0x0,0x0,  //49 1
    0x0,0x804,0xc02,0xa01,0x901,0x881,0x862,0x81c,0x0,      //50 2
    0x0,0x204,0x402,0x821,0x821,0x831,0x45e,0x380,0x0,      //51 3
    0x180,0x140,0x120,0x118,0x104,0x102,0xfff,0x100,0x0,    //52 4
    0x0,0x238,0x417,0x811,0x811,0x811,0x421,0x3c1,0x0,      //53 5
    0x0,0x3fc,0x422,0x811,0x811,0x811,0x422,0x3c4,0x0,      //54 6
    0x0,0x1,0x1,0xf01,0xe1,0x19,0x5,0x3,0x0,        //55 7
    0x0,0x38c,0x452,0x821,0x821,0x821,0x452,0x38c,0x0,      //56 8
    0x0,0x23c,0x442,0x881,0x881,0x881,0x442,0x3fc,0x0,      //57 9
    0x0,0x808,0x0,0x0,      //58 :
    0x0,0x3808,0x0,0x0,     //59 ;
    0x0,0x40,0xa0,0xa0,0x110,0x110,0x110,0x208,0x0, //60 <
    0x0,0x110,0x110,0x110,0x110,0x110,0x110,0x110,0x0,      //61 =
    0x0,0x208,0x110,0x110,0x110,0xa0,0xa0,0x40,0x0, //62 >
    0x0,0xc,0x2,0x1,0xb81,0x41,0x22,0x1c,0x0,       //63 ?
    0x0,0x7e0,0x818,0x1004,0x23c2,0x2422,0x4811,0x4809,0x4809,0x4409,0x4f91,0x4879,0x4802,0x2402,0x220c,0x11f0,     //64 @
    0x0,0xc00,0x380,0xf0,0x8e,0x81,0x8e,0xf0,0x380,0xc00,0x0,       //65 A
    0x0,0xfff,0x821,0x821,0x821,0x821,0x821,0x821,0x452,0x38c,0x0,  //66 B
    0x0,0x1f8,0x204,0x402,0x801,0x801,0x801,0x801,0x801,0x402,0x204,0x0,    //67 C
    0x0,0xfff,0x801,0x801,0x801,0x801,0x801,0x801,0x402,0x204,0x1f8,0x0,    //68 D
    0x0,0xfff,0x821,0x821,0x821,0x821,0x821,0x821,0x821,0x801,0x0,  //69 E
    0x0,0xfff,0x41,0x41,0x41,0x41,0x41,0x41,0x1,0x0,        //70 F
    0x0,0x1f8,0x204,0x402,0x801,0x801,0x841,0x841,0x442,0x244,0x1c0,0x0,    //71 G
    0x0,0xfff,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0xfff,0x0,0x0,     //72 H
    0x0,0xfff,0x0,0x0,      //73 I
    0x0,0x700,0x800,0x800,0x800,0x800,0x7ff,0x0,    //74 J
    0x0,0xfff,0x80,0x40,0x20,0x70,0x88,0x304,0x402,0x801,0x0,       //75 K
    0x0,0xfff,0x800,0x800,0x800,0x800,0x800,0x800,0x0,      //76 L
    0x0,0xfff,0x6,0x18,0x60,0x380,0xc00,0x380,0x60,0x18,0x6,0xfff,0x0,      //77 M
    0x0,0xfff,0x2,0xc,0x10,0x60,0x80,0x300,0x400,0xfff,0x0,0x0,     //78 N
    0x0,0x1f8,0x204,0x402,0x801,0x801,0x801,0x801,0x402,0x204,0x1f8,0x0,    //79 O
    0x0,0xfff,0x41,0x41,0x41,0x41,0x41,0x41,0x22,0x1c,0x0,  //80 P
    0x0,0x1f8,0x204,0x402,0x801,0x801,0xa01,0xa01,0x402,0xe04,0x9f8,0x0,    //81 Q
    0x0,0xfff,0x41,0x41,0x41,0x41,0xc1,0x341,0x422,0x81c,0x0,0x0,   //82 R
    0x0,0x20c,0x412,0x821,0x821,0x821,0x841,0x841,0x442,0x384,0x0,  //83 S
    0x1,0x1,0x1,0x1,0xfff,0x1,0x1,0x1,0x1,0x0,      //84 T
    0x0,0x3ff,0x400,0x800,0x800,0x800,0x800,0x800,0x400,0x3ff,0x0,0x0,      //85 U
    0x0,0x3,0x1c,0xe0,0x300,0xc00,0x300,0xe0,0x1c,0x3,0x0,  //86 V
    0x3,0x3c,0x3c0,0xc00,0x380,0x70,0xe,0x1,0xe,0x70,0x380,0xc00,0x3c0,0x3c,0x3,    //87 W
    0x800,0x401,0x202,0x18c,0x50,0x20,0x50,0x18c,0x202,0x401,0x800, //88 X
    0x1,0x6,0x8,0x30,0xfc0,0x30,0x8,0x6,0x1,0x0,0x0,        //89 Y
    0x800,0xc01,0xb01,0x881,0x861,0x811,0x80d,0x803,0x801,0x0,      //90 Z
    0x0,0x7fff,0x4001,0x4001,       //91 [
    0x3,0x3c,0x3c0,0xc00,   //92 bs
    0x4001,0x4001,0x7fff,0x0,       //93 ]
    0x20,0x18,0x6,0x1,0x6,0x18,0x20,0x0,    //94 ^
    0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000, //95 _
    0x0,0x1,0x2,0x0,0x0,    //96 `
    0x0,0x720,0x890,0x888,0x888,0x848,0x448,0xff0,0x0,      //97 a
    0x0,0xfff,0x410,0x808,0x808,0x808,0x410,0x3e0,0x0,      //98 b
    0x0,0x3e0,0x410,0x808,0x808,0x808,0x410,0x0,    //99 c
    0x0,0x3e0,0x410,0x808,0x808,0x808,0x410,0xfff,0x0,      //100 d
    0x0,0x3e0,0x490,0x888,0x888,0x888,0x490,0x2e0,0x0,      //101 e
    0x8,0xffe,0x9,0x9,      //102 f
    0x0,0x23e0,0x4410,0x4808,0x4808,0x4808,0x2410,0x1ff8,0x0,       //103 g
    0x0,0xfff,0x10,0x8,0x8,0x8,0xff0,0x0,0x0,       //104 h
    0x0,0xff9,0x0,0x0,      //105 i
    0x4000,0x3ff9,0x0,0x0,  //106 j
    0x0,0xfff,0x100,0x80,0xc0,0x320,0x410,0x808,    //107 k
    0x0,0xfff,0x0,0x0,      //108 l
    0x0,0xff8,0x10,0x8,0x8,0x8,0xff0,0x10,0x8,0x8,0x8,0xff0,0x0,    //109 m
    0x0,0xff8,0x10,0x8,0x8,0x8,0xff0,0x0,0x0,       //110 n
    0x0,0x3e0,0x410,0x808,0x808,0x808,0x410,0x3e0,0x0,      //111 o
    0x0,0x7ff8,0x410,0x808,0x808,0x808,0x410,0x3e0,0x0,     //112 p
    0x0,0x3e0,0x410,0x808,0x808,0x808,0x410,0x7ff8,0x0,     //113 q
    0x0,0xff8,0x10,0x8,0x8, //114 r
    0x0,0x470,0x888,0x888,0x888,0x888,0x710,0x0,    //115 s
    0x8,0xffe,0x808,0x808,  //116 t
    0x0,0x7f8,0x800,0x800,0x800,0x400,0xff8,0x0,0x0,        //117 u
    0x18,0x60,0x380,0xc00,0x380,0x60,0x18,0x0,      //118 v
    0x18,0x3e0,0xc00,0x380,0x60,0x18,0x60,0x380,0xc00,0x3e0,0x18,0x0,       //119 w
    0x808,0x410,0x360,0x80,0x360,0x410,0x808,0x0,   //120 x
    0x38,0x40c0,0x4700,0x3c00,0x700,0xe0,0x18,0x0,  //121 y
    0x808,0xc08,0xb08,0x888,0x868,0x818,0x808,0x0,  //122 z
    0x80,0x80,0x3f7e,0x4001,0x4001, //123 {
    0x0,0x7fff,0x0,0x0,     //124 |
    0x4001,0x4001,0x3f7e,0x80,0x80, //125 }
    0x40,0x20,0x20,0x20,0x40,0x40,0x40,0x20,0x0,    //126 ~
    0x06,0x09,0x09,0x06,0x00            //127 degree
};


const uint16_t F2_charoffsets[] PROGMEM = {
    0x0, 0x4, 0x8, 0xe, 0x17, 0x20, 0x2e, 0x39, 0x3c, 0x41, 0x46, 0x4c, 0x55, 0x59, 0x5e, 0x62, 
    0x66, 0x6f, 0x78, 0x81, 0x8a, 0x93, 0x9c, 0xa5, 0xae, 0xb7, 0xc0, 0xc4, 0xc8, 0xd1, 0xda, 0xe3, 
    0xec, 0xfc, 0x107, 0x112, 0x11e, 0x12a, 0x135, 0x13f, 
    0x14b, 0x157, 0x15b, 0x163, 0x16e, 0x177, 0x184, 0x190, 
    0x19c, 0x1a7, 0x1b3, 0x1bf, 0x1ca, 0x1d4, 0x1e0, 0x1eb, 
    0x1fa, 0x205, 0x210, 0x21a, 0x21e, 0x222, 0x226, 0x22e, 
    0x237, 0x23c, 0x245, 0x24e, 0x256, 0x25f, 0x268, 0x26c, 
    0x275, 0x27e, 0x282, 0x286, 0x28e, 0x292, 0x29f, 0x2a8, 
    0x2b1, 0x2ba, 0x2c3, 0x2c8, 0x2d0, 0x2d4, 0x2dd, 0x2e5, 
    0x2f1, 0x2f9, 0x301, 0x309, 0x30e, 0x312, 0x317, 0x320,
    0x325   //end of table
};


Font F1{32,126,F1_chardata,F1_charoffsets};
Font F2{32,127,F2_chardata,F2_charoffsets};

Font *fonts[] = {&F1,&F2};

#endif
