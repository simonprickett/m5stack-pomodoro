#ifndef _ICON_H_
#define _ICON_H_

#include "utility/Ink_Sprite.h"

typedef enum {
    kRGBImage,
}ImangePixMode_t;

typedef struct {
    int width;
    int height;
    int bitCount;
    unsigned char *ptr;
}image_t;

// Num40x55
extern unsigned char image_num_01[275];   //"0"  W:40 H:55
extern unsigned char image_num_02[275];   //"1"  W:40 H:55
extern unsigned char image_num_03[275];   //"2"  W:40 H:55
extern unsigned char image_num_04[275];   //"3"  W:40 H:55
extern unsigned char image_num_05[275];   //"4"  W:40 H:55
extern unsigned char image_num_06[275];   //"5"  W:40 H:55
extern unsigned char image_num_07[275];   //"6"  W:40 H:55
extern unsigned char image_num_08[275];   //"7"  W:40 H:55
extern unsigned char image_num_09[275];   //"8"  W:40 H:55
extern unsigned char image_num_10[275];   //"9"  W:40 H:55
extern unsigned char image_num_11[165];   //":"  W:24 H:55


extern image_t num55[11];

#endif
