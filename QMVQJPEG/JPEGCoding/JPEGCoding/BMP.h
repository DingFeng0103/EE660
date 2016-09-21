#ifndef _BMP_
#define _BMP_
#pragma pack(2)
typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;
#define IMAGESIZE 256
#define HEADERSIZE 54

typedef struct tagBITMAPFILEHEADER {
	WORD  bfType;
	DWORD bfSize;
	WORD  bfReserved1;
	WORD  bfReserved2;
	DWORD bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	DWORD biSize;
	LONG  biWidth;
	LONG  biHeight;
	WORD  biPlanes;
	WORD  biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG  biXPelsPerMeter;
	LONG  biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

class BMP{
public:
	PBITMAPFILEHEADER file_header;
	PBITMAPINFOHEADER info_header;
	unsigned char * rawdata;
};
#endif 