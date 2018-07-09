#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
//#include "measurement.h"
#include<malloc.h>
#include<string.h>
#include <inttypes.h>
#include <stdint.h>

#define align __attribute__ ((aligned (16)))

#if !defined (ALIGN16)
	# if defined (__GNUC__)
		# define ALIGN32 __attribute__ ( (aligned (16)))
	# else
		# define ALIGN32 __declspec (align (16))
	# endif
#endif

#define repeat 1000
typedef __m256i vec;
//typedef int8_t  u8;
//typedef int16_t u16;
//typedef int32_t u32;
//typedef int64_t u64;

const vec mask255 = {0xff000000ff,0xff000000ff,0xff000000ff,0xff000000ff};
const vec mask16b = {0xFFFF0000FFFF,0xFFFF0000FFFF,0xFFFF0000FFFF,0xFFFF0000FFFF};
const vec p257 = {0x10100000101,0x10100000101,0x10100000101,0x10100000101};
const vec p257_2 = {0x20200000202,0x20200000202,0x20200000202,0x20200000202};
const vec p257_3 = {0x80800000808,0x80800000808,0x80800000808,0x80800000808};
const vec p257_5 = {0x202000002020,0x202000002020,0x202000002020,0x202000002020};
const vec p257_7 = {0x808000008080,0x808000008080,0x808000008080,0x808000008080};
const vec p257_8 = {0x1010000010100,0x1010000010100,0x1010000010100,0x1010000010100};
const vec p257_9 = {0x2020000020200,0x2020000020200,0x2020000020200,0x2020000020200};
const vec p257_12 = {0x10100000101000,0x10100000101000,0x10100000101000,0x10100000101000,};
/*const vec p257_16 = {0x101000001010000,0x101000001010000,0x101000001010000,0x101000001010000};
const vec p257_17 = {0x202000002020000,0x202000002020000,0x202000002020000,0x202000002020000};
const vec p257_19 = {0x808000008080000,0x808000008080000,0x808000008080000,0x808000008080000};
const vec p257_21 = {0x2020000020200000,0x2020000020200000,0x2020000020200000,0x2020000020200000};*/
const vec p257_23 = {0x1010000010100000,0x1010000010100000,0x1010000010100000,0x1010000010100000};

const vec p257_16 = {0x202000002020000,0x202000002020000,0x202000002020000,0x202000002020000};
const vec p257_17 = {0x10100000101000,0x10100000101000,0x10100000101000,0x10100000101000};
const vec p257_19 = {0x1010000010100000,0x1010000010100000,0x1010000010100000,0x1010000010100000};
const vec p257_21 = {0x404000004040000,0x404000004040000,0x404000004040000,0x404000004040000};
//const vec p257_23 = {0x10100000101000000,,,};

const vec permute[8] = 	{{0x000000000,0x000000000,0x000000000,0x000000000},
			 {0x100000001,0x100000001,0x100000001,0x100000001},
			 {0x200000002,0x200000002,0x200000002,0x200000002},
		 	 {0x300000003,0x300000003,0x300000003,0x300000003},
			 {0x400000004,0x400000004,0x400000004,0x400000004},
			 {0x500000005,0x500000005,0x500000005,0x500000005},
			 {0x600000006,0x600000006,0x600000006,0x600000006},
			 {0x700000007,0x700000007,0x700000007,0x700000007}};
const vec k0i0[8] = {{ 0x100000001, 0x100000001, 0x100000001, 0x100000001 },
		 { 0x800000002, 0x8000000020, 0xF9000000FF, 0x81000000E1},
		 { 0x4000000004, 0xC1000000FD, 0x4000000004, 0xC1000000FD},
		 { 0xFF00000008, 0x2000000081, 0x2000000F9, 0xE100000080 },
		 { 0xF100000010, 0xF100000010, 0xF100000010, 0xF100000010 },
		 { 0x8100000020, 0x8000000FF, 0x80000000E1, 0xF900000002 },
		 { 0x400000040, 0xFD000000C1, 0x400000040, 0xFD000000C1 },
		 { 0x2000000080, 0x200000008, 0xE100000081, 0xFF000000F9 }};

void create_x(int x[16][64]);
//extern inline void print_x(int x[16][64]);
extern inline void ReduceY(vec *Y);
extern inline void ReduceY_final(vec *Y);
extern inline void unpackY(vec Y[8],u8 *pk);
extern inline void unpack_rgY(vec Y[8],u32 *pk);
extern inline void outputY(u32 Y[8][8]);
extern inline void SWIFFT(int x[16][8],u8 *pk);
extern inline void gSWIFFT(int x[16][64],u32 *pk);
extern inline void print(vec Y);
extern inline void set_Key();
void read_Input();


const int p = 257;
const int omega = 42;
const vec allone = {-1,-1,-1,-1};
const vec zero = {0,0,0,0};
/*====Gobal variables====*/
const u32 pow_omega[128] align = { 1,  42, 222,  72, 197,  50,  44,  49,   2,  84, 187, 144, 137, 100,  88,  98,
			           4, 168, 117,  31,  17, 200, 176, 196,   8,  79, 234,  62,  34, 143,  95, 135,
			          16, 158, 211, 124,  68,  29, 190,  13,  32,  59, 165, 248, 136,  58, 123,  26,
			          64, 118,  73, 239,  15, 116, 246,  52, 128, 236, 146, 221,  30, 232, 235, 104,
			         256, 215,  35, 185,  60, 207, 213, 208, 255, 173,  70, 113, 120, 157, 169, 159,
			         253,  89, 140, 226, 240,  57,  81,  61, 249, 178,  23, 195, 223, 114, 162, 122,
			         241,  99,  46, 133, 189, 228,  67, 244, 225, 198,  92,   9, 121, 199, 134, 231,
			         193, 139, 184,  18, 242, 141,  11, 205, 129,  21, 111,  36, 227,  25,  22, 153};

const vec T_K0_I0[256] align = {{ 0x0, 0x0, 0x0, 0x0 },
    { 0x100000001, 0x100000001, 0x100000001, 0x100000001 },
    { 0x800000002, 0x8000000020, 0xF9000000FF, 0x81000000E1 },
    { 0x900000003, 0x8100000021, 0xFA00000100, 0x82000000E2 },
    { 0x4000000004, 0xC1000000FD, 0x4000000004, 0xC1000000FD },
    { 0x4100000005, 0xC2000000FE, 0x4100000005, 0xC2000000FE },
    { 0x4800000006, 0x400000001C, 0x3800000002, 0x41000000DD },
    { 0x4900000007, 0x410000001D, 0x3900000003, 0x42000000DE },
    { 0xFF00000008, 0x2000000081, 0x2000000F9, 0xE100000080 },
    { 0x10000000009, 0x2100000082, 0x3000000FA, 0xE200000081 },
    { 0x60000000A, 0xA0000000A1, 0xFB000000F7, 0x6100000060 },
    { 0x70000000B, 0xA1000000A2, 0xFC000000F8, 0x6200000061 },
    { 0x3E0000000C, 0xE10000007D, 0x42000000FD, 0xA10000007C },
    { 0x3F0000000D, 0xE20000007E, 0x43000000FE, 0xA20000007D },
    { 0x460000000E, 0x600000009D, 0x3A000000FB, 0x210000005C },
    { 0x470000000F, 0x610000009E, 0x3B000000FC, 0x220000005D },
    { 0xF100000010, 0xF100000010, 0xF100000010, 0xF100000010 },
    { 0xF200000011, 0xF200000011, 0xF200000011, 0xF200000011 },
    { 0xF900000012, 0x7000000030, 0xE90000000E, 0x71000000F1 },
    { 0xFA00000013, 0x7100000031, 0xEA0000000F, 0x72000000F2 },
    { 0x3000000014, 0xB10000000C, 0x3000000014, 0xB10000000C },
    { 0x3100000015, 0xB20000000D, 0x3100000015, 0xB20000000D },
    { 0x3800000016, 0x300000002C, 0x2800000012, 0x31000000ED },
    { 0x3900000017, 0x310000002D, 0x2900000013, 0x32000000EE },
    { 0xEF00000018, 0x1000000091, 0xF300000008, 0xD100000090 },
    { 0xF000000019, 0x1100000092, 0xF400000009, 0xD200000091 },
    { 0xF70000001A, 0x90000000B1, 0xEB00000006, 0x5100000070 },
    { 0xF80000001B, 0x91000000B2, 0xEC00000007, 0x5200000071 },
    { 0x2E0000001C, 0xD10000008D, 0x320000000C, 0x910000008C },
    { 0x2F0000001D, 0xD20000008E, 0x330000000D, 0x920000008D },
    { 0x360000001E, 0x50000000AD, 0x2A0000000A, 0x110000006C },
    { 0x370000001F, 0x51000000AE, 0x2B0000000B, 0x120000006D },
    { 0x8100000020, 0x8000000FF, 0x80000000E1, 0xF900000002 },
    { 0x8200000021, 0x900000100, 0x81000000E2, 0xFA00000003 },
    { 0x8900000022, 0x880000001E, 0x78000000DF, 0x79000000E3 },
    { 0x8A00000023, 0x890000001F, 0x79000000E0, 0x7A000000E4 },
    { 0xC100000024, 0xC9000000FB, 0xC0000000E5, 0xB9000000FF },
    { 0xC200000025, 0xCA000000FC, 0xC1000000E6, 0xBA00000100 },
    { 0xC900000026, 0x480000001A, 0xB8000000E3, 0x39000000DF },
    { 0xCA00000027, 0x490000001B, 0xB9000000E4, 0x3A000000E0 },
    { 0x7F00000028, 0x280000007F, 0x82000000D9, 0xD900000082 },
    { 0x8000000029, 0x2900000080, 0x83000000DA, 0xDA00000083 },
    { 0x870000002A, 0xA80000009F, 0x7A000000D7, 0x5900000062 },
    { 0x880000002B, 0xA9000000A0, 0x7B000000D8, 0x5A00000063 },
    { 0xBF0000002C, 0xE90000007B, 0xC2000000DD, 0x990000007E },
    { 0xC00000002D, 0xEA0000007C, 0xC3000000DE, 0x9A0000007F },
    { 0xC70000002E, 0x680000009B, 0xBA000000DB, 0x190000005E },
    { 0xC80000002F, 0x690000009C, 0xBB000000DC, 0x1A0000005F },
    { 0x7100000030, 0xF90000000E, 0x70000000F1, 0xE900000012 },
    { 0x7200000031, 0xFA0000000F, 0x71000000F2, 0xEA00000013 },
    { 0x7900000032, 0x780000002E, 0x68000000EF, 0x69000000F3 },
    { 0x7A00000033, 0x790000002F, 0x69000000F0, 0x6A000000F4 },
    { 0xB100000034, 0xB90000000A, 0xB0000000F5, 0xA90000000E },
    { 0xB200000035, 0xBA0000000B, 0xB1000000F6, 0xAA0000000F },
    { 0xB900000036, 0x380000002A, 0xA8000000F3, 0x29000000EF },
    { 0xBA00000037, 0x390000002B, 0xA9000000F4, 0x2A000000F0 },
    { 0x6F00000038, 0x180000008F, 0x72000000E9, 0xC900000092 },
    { 0x7000000039, 0x1900000090, 0x73000000EA, 0xCA00000093 },
    { 0x770000003A, 0x98000000AF, 0x6A000000E7, 0x4900000072 },
    { 0x780000003B, 0x99000000B0, 0x6B000000E8, 0x4A00000073 },
    { 0xAF0000003C, 0xD90000008B, 0xB2000000ED, 0x890000008E },
    { 0xB00000003D, 0xDA0000008C, 0xB3000000EE, 0x8A0000008F },
    { 0xB70000003E, 0x58000000AB, 0xAA000000EB, 0x90000006E },
    { 0xB80000003F, 0x59000000AC, 0xAB000000EC, 0xA0000006F },
    { 0x400000040, 0xFD000000C1, 0x400000040, 0xFD000000C1 },
    { 0x500000041, 0xFE000000C2, 0x500000041, 0xFE000000C2 },
    { 0xC00000042, 0x7C000000E1, 0xFD0000003E, 0x7D000000A1 },
    { 0xD00000043, 0x7D000000E2, 0xFE0000003F, 0x7E000000A2 },
    { 0x4400000044, 0xBD000000BD, 0x4400000044, 0xBD000000BD },
    { 0x4500000045, 0xBE000000BE, 0x4500000045, 0xBE000000BE },
    { 0x4C00000046, 0x3C000000DD, 0x3C00000042, 0x3D0000009D },
    { 0x4D00000047, 0x3D000000DE, 0x3D00000043, 0x3E0000009E },
    { 0x200000048, 0x1C00000041, 0x600000038, 0xDD00000040 },
    { 0x300000049, 0x1D00000042, 0x700000039, 0xDE00000041 },
    { 0xA0000004A, 0x9C00000061, 0xFF00000036, 0x5D00000020 },
    { 0xB0000004B, 0x9D00000062, 0x10000000037, 0x5E00000021 },
    { 0x420000004C, 0xDD0000003D, 0x460000003C, 0x9D0000003C },
    { 0x430000004D, 0xDE0000003E, 0x470000003D, 0x9E0000003D },
    { 0x4A0000004E, 0x5C0000005D, 0x3E0000003A, 0x1D0000001C },
    { 0x4B0000004F, 0x5D0000005E, 0x3F0000003B, 0x1E0000001D },
    { 0xF500000050, 0xED000000D1, 0xF500000050, 0xED000000D1 },
    { 0xF600000051, 0xEE000000D2, 0xF600000051, 0xEE000000D2 },
    { 0xFD00000052, 0x6C000000F1, 0xED0000004E, 0x6D000000B1 },
    { 0xFE00000053, 0x6D000000F2, 0xEE0000004F, 0x6E000000B2 },
    { 0x3400000054, 0xAD000000CD, 0x3400000054, 0xAD000000CD },
    { 0x3500000055, 0xAE000000CE, 0x3500000055, 0xAE000000CE },
    { 0x3C00000056, 0x2C000000ED, 0x2C00000052, 0x2D000000AD },
    { 0x3D00000057, 0x2D000000EE, 0x2D00000053, 0x2E000000AE },
    { 0xF300000058, 0xC00000051, 0xF700000048, 0xCD00000050 },
    { 0xF400000059, 0xD00000052, 0xF800000049, 0xCE00000051 },
    { 0xFB0000005A, 0x8C00000071, 0xEF00000046, 0x4D00000030 },
    { 0xFC0000005B, 0x8D00000072, 0xF000000047, 0x4E00000031 },
    { 0x320000005C, 0xCD0000004D, 0x360000004C, 0x8D0000004C },
    { 0x330000005D, 0xCE0000004E, 0x370000004D, 0x8E0000004D },
    { 0x3A0000005E, 0x4C0000006D, 0x2E0000004A, 0xD0000002C },
    { 0x3B0000005F, 0x4D0000006E, 0x2F0000004B, 0xE0000002D },
    { 0x8500000060, 0x4000000BF, 0x8400000020, 0xF5000000C3 },
    { 0x8600000061, 0x5000000C0, 0x8500000021, 0xF6000000C4 },
    { 0x8D00000062, 0x84000000DF, 0x7C0000001E, 0x75000000A3 },
    { 0x8E00000063, 0x85000000E0, 0x7D0000001F, 0x76000000A4 },
    { 0xC500000064, 0xC5000000BB, 0xC400000024, 0xB5000000BF },
    { 0xC600000065, 0xC6000000BC, 0xC500000025, 0xB6000000C0 },
    { 0xCD00000066, 0x44000000DB, 0xBC00000022, 0x350000009F },
    { 0xCE00000067, 0x45000000DC, 0xBD00000023, 0x36000000A0 },
    { 0x8300000068, 0x240000003F, 0x8600000018, 0xD500000042 },
    { 0x8400000069, 0x2500000040, 0x8700000019, 0xD600000043 },
    { 0x8B0000006A, 0xA40000005F, 0x7E00000016, 0x5500000022 },
    { 0x8C0000006B, 0xA500000060, 0x7F00000017, 0x5600000023 },
    { 0xC30000006C, 0xE50000003B, 0xC60000001C, 0x950000003E },
    { 0xC40000006D, 0xE60000003C, 0xC70000001D, 0x960000003F },
    { 0xCB0000006E, 0x640000005B, 0xBE0000001A, 0x150000001E },
    { 0xCC0000006F, 0x650000005C, 0xBF0000001B, 0x160000001F },
    { 0x7500000070, 0xF5000000CF, 0x7400000030, 0xE5000000D3 },
    { 0x7600000071, 0xF6000000D0, 0x7500000031, 0xE6000000D4 },
    { 0x7D00000072, 0x74000000EF, 0x6C0000002E, 0x65000000B3 },
    { 0x7E00000073, 0x75000000F0, 0x6D0000002F, 0x66000000B4 },
    { 0xB500000074, 0xB5000000CB, 0xB400000034, 0xA5000000CF },
    { 0xB600000075, 0xB6000000CC, 0xB500000035, 0xA6000000D0 },
    { 0xBD00000076, 0x34000000EB, 0xAC00000032, 0x25000000AF },
    { 0xBE00000077, 0x35000000EC, 0xAD00000033, 0x26000000B0 },
    { 0x7300000078, 0x140000004F, 0x7600000028, 0xC500000052 },
    { 0x7400000079, 0x1500000050, 0x7700000029, 0xC600000053 },
    { 0x7B0000007A, 0x940000006F, 0x6E00000026, 0x4500000032 },
    { 0x7C0000007B, 0x9500000070, 0x6F00000027, 0x4600000033 },
    { 0xB30000007C, 0xD50000004B, 0xB60000002C, 0x850000004E },
    { 0xB40000007D, 0xD60000004C, 0xB70000002D, 0x860000004F },
    { 0xBB0000007E, 0x540000006B, 0xAE0000002A, 0x50000002E },
    { 0xBC0000007F, 0x550000006C, 0xAF0000002B, 0x60000002F },
    { 0x2000000080, 0x200000008, 0xE100000081, 0xFF000000F9 },
    { 0x2100000081, 0x300000009, 0xE200000082, 0x100000000FA },
    { 0x2800000082, 0x8200000028, 0xD90000007F, 0x7F000000D9 },
    { 0x2900000083, 0x8300000029, 0xDA00000080, 0x80000000DA },
    { 0x6000000084, 0xC300000004, 0x2000000085, 0xBF000000F5 },
    { 0x6100000085, 0xC400000005, 0x2100000086, 0xC0000000F6 },
    { 0x6800000086, 0x4200000024, 0x1800000083, 0x3F000000D5 },
    { 0x6900000087, 0x4300000025, 0x1900000084, 0x40000000D6 },
    { 0x1E00000088, 0x2200000089, 0xE300000079, 0xDF00000078 },
    { 0x1F00000089, 0x230000008A, 0xE40000007A, 0xE000000079 },
    { 0x260000008A, 0xA2000000A9, 0xDB00000077, 0x5F00000058 },
    { 0x270000008B, 0xA3000000AA, 0xDC00000078, 0x6000000059 },
    { 0x5E0000008C, 0xE300000085, 0x220000007D, 0x9F00000074 },
    { 0x5F0000008D, 0xE400000086, 0x230000007E, 0xA000000075 },
    { 0x660000008E, 0x62000000A5, 0x1A0000007B, 0x1F00000054 },
    { 0x670000008F, 0x63000000A6, 0x1B0000007C, 0x2000000055 },
    { 0x1000000090, 0xF300000018, 0xD100000091, 0xEF00000008 },
    { 0x1100000091, 0xF400000019, 0xD200000092, 0xF000000009 },
    { 0x1800000092, 0x7200000038, 0xC90000008F, 0x6F000000E9 },
    { 0x1900000093, 0x7300000039, 0xCA00000090, 0x70000000EA },
    { 0x5000000094, 0xB300000014, 0x1000000095, 0xAF00000004 },
    { 0x5100000095, 0xB400000015, 0x1100000096, 0xB000000005 },
    { 0x5800000096, 0x3200000034, 0x800000093, 0x2F000000E5 },
    { 0x5900000097, 0x3300000035, 0x900000094, 0x30000000E6 },
    { 0xE00000098, 0x1200000099, 0xD300000089, 0xCF00000088 },
    { 0xF00000099, 0x130000009A, 0xD40000008A, 0xD000000089 },
    { 0x160000009A, 0x92000000B9, 0xCB00000087, 0x4F00000068 },
    { 0x170000009B, 0x93000000BA, 0xCC00000088, 0x5000000069 },
    { 0x4E0000009C, 0xD300000095, 0x120000008D, 0x8F00000084 },
    { 0x4F0000009D, 0xD400000096, 0x130000008E, 0x9000000085 },
    { 0x560000009E, 0x52000000B5, 0xA0000008B, 0xF00000064 },
    { 0x570000009F, 0x53000000B6, 0xB0000008C, 0x1000000065 },
    { 0xA1000000A0, 0xA00000006, 0x6000000061, 0xF7000000FB },
    { 0xA2000000A1, 0xB00000007, 0x6100000062, 0xF8000000FC },
    { 0xA9000000A2, 0x8A00000026, 0x580000005F, 0x77000000DB },
    { 0xAA000000A3, 0x8B00000027, 0x5900000060, 0x78000000DC },
    { 0xE1000000A4, 0xCB00000002, 0xA000000065, 0xB7000000F7 },
    { 0xE2000000A5, 0xCC00000003, 0xA100000066, 0xB8000000F8 },
    { 0xE9000000A6, 0x4A00000022, 0x9800000063, 0x37000000D7 },
    { 0xEA000000A7, 0x4B00000023, 0x9900000064, 0x38000000D8 },
    { 0x9F000000A8, 0x2A00000087, 0x6200000059, 0xD70000007A },
    { 0xA0000000A9, 0x2B00000088, 0x630000005A, 0xD80000007B },
    { 0xA7000000AA, 0xAA000000A7, 0x5A00000057, 0x570000005A },
    { 0xA8000000AB, 0xAB000000A8, 0x5B00000058, 0x580000005B },
    { 0xDF000000AC, 0xEB00000083, 0xA20000005D, 0x9700000076 },
    { 0xE0000000AD, 0xEC00000084, 0xA30000005E, 0x9800000077 },
    { 0xE7000000AE, 0x6A000000A3, 0x9A0000005B, 0x1700000056 },
    { 0xE8000000AF, 0x6B000000A4, 0x9B0000005C, 0x1800000057 },
    { 0x91000000B0, 0xFB00000016, 0x5000000071, 0xE70000000A },
    { 0x92000000B1, 0xFC00000017, 0x5100000072, 0xE80000000B },
    { 0x99000000B2, 0x7A00000036, 0x480000006F, 0x67000000EB },
    { 0x9A000000B3, 0x7B00000037, 0x4900000070, 0x68000000EC },
    { 0xD1000000B4, 0xBB00000012, 0x9000000075, 0xA700000006 },
    { 0xD2000000B5, 0xBC00000013, 0x9100000076, 0xA800000007 },
    { 0xD9000000B6, 0x3A00000032, 0x8800000073, 0x27000000E7 },
    { 0xDA000000B7, 0x3B00000033, 0x8900000074, 0x28000000E8 },
    { 0x8F000000B8, 0x1A00000097, 0x5200000069, 0xC70000008A },
    { 0x90000000B9, 0x1B00000098, 0x530000006A, 0xC80000008B },
    { 0x97000000BA, 0x9A000000B7, 0x4A00000067, 0x470000006A },
    { 0x98000000BB, 0x9B000000B8, 0x4B00000068, 0x480000006B },
    { 0xCF000000BC, 0xDB00000093, 0x920000006D, 0x8700000086 },
    { 0xD0000000BD, 0xDC00000094, 0x930000006E, 0x8800000087 },
    { 0xD7000000BE, 0x5A000000B3, 0x8A0000006B, 0x700000066 },
    { 0xD8000000BF, 0x5B000000B4, 0x8B0000006C, 0x800000067 },
    { 0x24000000C0, 0xFF000000C9, 0xE5000000C1, 0xFB000000B9 },
    { 0x25000000C1, 0x100000000CA, 0xE6000000C2, 0xFC000000BA },
    { 0x2C000000C2, 0x7E000000E9, 0xDD000000BF, 0x7B00000099 },
    { 0x2D000000C3, 0x7F000000EA, 0xDE000000C0, 0x7C0000009A },
    { 0x64000000C4, 0xBF000000C5, 0x24000000C5, 0xBB000000B5 },
    { 0x65000000C5, 0xC0000000C6, 0x25000000C6, 0xBC000000B6 },
    { 0x6C000000C6, 0x3E000000E5, 0x1C000000C3, 0x3B00000095 },
    { 0x6D000000C7, 0x3F000000E6, 0x1D000000C4, 0x3C00000096 },
    { 0x22000000C8, 0x1E00000049, 0xE7000000B9, 0xDB00000038 },
    { 0x23000000C9, 0x1F0000004A, 0xE8000000BA, 0xDC00000039 },
    { 0x2A000000CA, 0x9E00000069, 0xDF000000B7, 0x5B00000018 },
    { 0x2B000000CB, 0x9F0000006A, 0xE0000000B8, 0x5C00000019 },
    { 0x62000000CC, 0xDF00000045, 0x26000000BD, 0x9B00000034 },
    { 0x63000000CD, 0xE000000046, 0x27000000BE, 0x9C00000035 },
    { 0x6A000000CE, 0x5E00000065, 0x1E000000BB, 0x1B00000014 },
    { 0x6B000000CF, 0x5F00000066, 0x1F000000BC, 0x1C00000015 },
    { 0x14000000D0, 0xEF000000D9, 0xD5000000D1, 0xEB000000C9 },
    { 0x15000000D1, 0xF0000000DA, 0xD6000000D2, 0xEC000000CA },
    { 0x1C000000D2, 0x6E000000F9, 0xCD000000CF, 0x6B000000A9 },
    { 0x1D000000D3, 0x6F000000FA, 0xCE000000D0, 0x6C000000AA },
    { 0x54000000D4, 0xAF000000D5, 0x14000000D5, 0xAB000000C5 },
    { 0x55000000D5, 0xB0000000D6, 0x15000000D6, 0xAC000000C6 },
    { 0x5C000000D6, 0x2E000000F5, 0xC000000D3, 0x2B000000A5 },
    { 0x5D000000D7, 0x2F000000F6, 0xD000000D4, 0x2C000000A6 },
    { 0x12000000D8, 0xE00000059, 0xD7000000C9, 0xCB00000048 },
    { 0x13000000D9, 0xF0000005A, 0xD8000000CA, 0xCC00000049 },
    { 0x1A000000DA, 0x8E00000079, 0xCF000000C7, 0x4B00000028 },
    { 0x1B000000DB, 0x8F0000007A, 0xD0000000C8, 0x4C00000029 },
    { 0x52000000DC, 0xCF00000055, 0x16000000CD, 0x8B00000044 },
    { 0x53000000DD, 0xD000000056, 0x17000000CE, 0x8C00000045 },
    { 0x5A000000DE, 0x4E00000075, 0xE000000CB, 0xB00000024 },
    { 0x5B000000DF, 0x4F00000076, 0xF000000CC, 0xC00000025 },
    { 0xA5000000E0, 0x6000000C7, 0x64000000A1, 0xF3000000BB },
    { 0xA6000000E1, 0x7000000C8, 0x65000000A2, 0xF4000000BC },
    { 0xAD000000E2, 0x86000000E7, 0x5C0000009F, 0x730000009B },
    { 0xAE000000E3, 0x87000000E8, 0x5D000000A0, 0x740000009C },
    { 0xE5000000E4, 0xC7000000C3, 0xA4000000A5, 0xB3000000B7 },
    { 0xE6000000E5, 0xC8000000C4, 0xA5000000A6, 0xB4000000B8 },
    { 0xED000000E6, 0x46000000E3, 0x9C000000A3, 0x3300000097 },
    { 0xEE000000E7, 0x47000000E4, 0x9D000000A4, 0x3400000098 },
    { 0xA3000000E8, 0x2600000047, 0x6600000099, 0xD30000003A },
    { 0xA4000000E9, 0x2700000048, 0x670000009A, 0xD40000003B },
    { 0xAB000000EA, 0xA600000067, 0x5E00000097, 0x530000001A },
    { 0xAC000000EB, 0xA700000068, 0x5F00000098, 0x540000001B },
    { 0xE3000000EC, 0xE700000043, 0xA60000009D, 0x9300000036 },
    { 0xE4000000ED, 0xE800000044, 0xA70000009E, 0x9400000037 },
    { 0xEB000000EE, 0x6600000063, 0x9E0000009B, 0x1300000016 },
    { 0xEC000000EF, 0x6700000064, 0x9F0000009C, 0x1400000017 },
    { 0x95000000F0, 0xF7000000D7, 0x54000000B1, 0xE3000000CB },
    { 0x96000000F1, 0xF8000000D8, 0x55000000B2, 0xE4000000CC },
    { 0x9D000000F2, 0x76000000F7, 0x4C000000AF, 0x63000000AB },
    { 0x9E000000F3, 0x77000000F8, 0x4D000000B0, 0x64000000AC },
    { 0xD5000000F4, 0xB7000000D3, 0x94000000B5, 0xA3000000C7 },
    { 0xD6000000F5, 0xB8000000D4, 0x95000000B6, 0xA4000000C8 },
    { 0xDD000000F6, 0x36000000F3, 0x8C000000B3, 0x23000000A7 },
    { 0xDE000000F7, 0x37000000F4, 0x8D000000B4, 0x24000000A8 },
    { 0x93000000F8, 0x1600000057, 0x56000000A9, 0xC30000004A },
    { 0x94000000F9, 0x1700000058, 0x57000000AA, 0xC40000004B },
    { 0x9B000000FA, 0x9600000077, 0x4E000000A7, 0x430000002A },
    { 0x9C000000FB, 0x9700000078, 0x4F000000A8, 0x440000002B },
    { 0xD3000000FC, 0xD700000053, 0x96000000AD, 0x8300000046 },
    { 0xD4000000FD, 0xD800000054, 0x97000000AE, 0x8400000047 },
    { 0xDB000000FE, 0x5600000073, 0x8E000000AB, 0x300000026 },
    { 0xDC000000FF, 0x5700000074, 0x8F000000AC, 0x400000027 }};

const vec M_K0_I0[8] align = {{ 0x100000001, 0x100000001, 0x100000001, 0x100000001 },
    { 0x480000002A, 0x3100000032, 0x9000000054, 0x6200000064 },
    { 0x2C000000DE, 0x58000000BB, 0xB000000075, 0x5F000000EA },
    { 0x5400000048, 0xC800000062, 0x9E0000003E, 0x3A0000000D },
    { 0x89000000C5, 0x2200000011, 0x8800000044, 0x1E0000000F },
    { 0x6200000032, 0x7C0000004F, 0x340000003A, 0x71000000D7 },
    { 0x750000002C, 0xA50000005F, 0x23000000F6, 0x17000000A9 },
    { 0xC800000031, 0x760000007C, 0x9D00000068, 0xC6000000C3 }};



