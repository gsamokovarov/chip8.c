#define BYTE4(blob) ((blob & 0xF000) >> 16)
#define BYTE3(blob) ((blob & 0x0F00) >> 8)
#define BYTE2(blob) ((blob & 0x00F0) >> 4)
#define BYTE1(blob) ((blob & 0x000F))
