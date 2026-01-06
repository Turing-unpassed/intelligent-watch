#ifndef __OLED_DATA_H
#define __OLED_DATA_H

#include <stdint.h>

/*字符集定义*/
/*以下两个宏定义只可解除其中一个的注释*/
// #define OLED_CHARSET_UTF8			//定义字符集为UTF8
#define OLED_CHARSET_GB2312		//定义字符集为GB2312

/*字模基本单元*/
typedef struct 
{
	
#ifdef OLED_CHARSET_UTF8			//定义字符集为UTF8
	char Index[5];					//汉字索引，空间为5字节
#endif
	
#ifdef OLED_CHARSET_GB2312			//定义字符集为GB2312
	char Index[3];					//汉字索引，空间为3字节
#endif
	
	uint8_t Data[32];				//字模数据
} ChineseCell_t;

/*ASCII字模数据声明*/
extern const uint8_t OLED_F8x16[][16];
extern const uint8_t OLED_F6x8[][6];

typedef struct {
	uint32_t Address;  // 图像在 W25Q128 中的起始地址
	uint8_t Width;     // 图像宽度，单位：像素
	uint8_t Height;    // 图像高度，单位：像素
	uint16_t Len;      // 图像数据长度，单位：字节
}Image_t;

/*图像数据声明*/
extern const uint8_t Diode[];

extern const Image_t t0;
extern const Image_t t1;
extern const Image_t t2;
extern const Image_t t3;
extern const Image_t t4;
extern const Image_t t5;
extern const Image_t t6;
extern const Image_t t7;
extern const Image_t t8;
extern const Image_t t9;

extern const Image_t i0;
extern const Image_t i1;
extern const Image_t i2;
extern const Image_t i3;
extern const Image_t i4;
extern const Image_t i5;
extern const Image_t i6;
extern const Image_t i7;
extern const Image_t i8;
extern const Image_t i9;

extern const uint8_t Batery[];
/*按照上面的格式，在这个位置加入新的图像数据声明*/
//...

#endif


/*****************江协科技|版权所有****************/
/*****************jiangxiekeji.com*****************/
