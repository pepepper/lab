#pragma pack(1)

#define readl(a) (*(volatile unsigned int *)(a))
#define writel(v, a) (*(volatile unsigned int *)(a) = (v))

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef struct _pixel_t {
    u8 r;
    u8 g;
    u8 b;
} pixel_t;

#define pack_pixel(p) (((u16)(p.r & 0x1f) << 11) | ((u16)(p.g & 0x3f) << 5) | ((u16)(p.b & 0x1f)))

#define WNATIVE 10
#define HNATIVE 854

void rot(u16* x, u16* y);
void prepare_fb(u16** fb);
void bitblt(u16** fb, pixel_t px, u16 x, u16 y);
