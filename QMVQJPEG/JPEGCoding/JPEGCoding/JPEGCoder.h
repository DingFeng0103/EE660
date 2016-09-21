#ifndef _JPEGCoder_
#define _JPEGCoder_

#include<vector>
#include<jpeglib.h>
#include"FileManager.h"
#include"BMP.h"

using namespace std;
#define RAWCENTER	0x80

#define FILESIZE 256
#define BLOCKSIZE 16
#define DCTSIZE2 64
#define DCTSIZE	 8

#define DCTLINE  DCTSIZE
#define DCTBLOCK DCTSIZE2
typedef unsigned char	RAWDATA;
typedef int				DCTDATA;

static const int qtable_jpeg[DCTBLOCK] =
{ 16,  11,  10,  16,  24,  40,  51,  61,
12,  12,  14,  19,  26,  58,  60,  55,
14,  13,  16,  24,  40,  57,  69,  56,
14,  17,  22,  29,  51,  87,  80,  62,
18,  22,  37,  56,  68, 109, 103,  77,
24,  35,  55,  64,  81, 104, 113,  92,
49,  64,  78,  87, 103, 121, 120, 101,
72,  92,  95,  98, 112, 100, 103,  99 };


class JPEGCoder
{
private:
	double divisors[DCTSIZE2];
	BMP bmpfile;
public:
	JPEGCoder();
	~JPEGCoder();
	void ReadRaw(string dir, RAWDATA *Imagedata, int n);
	void DCT(RAWDATA * in_data, DCTDATA * out_data);
	void SubBlock(RAWDATA * in_data, int M, int N, RAWDATA *out_data, int x, int y, int m, int n);
	void DisplayRaw(RAWDATA * in_data, int x, int y, int m, int n);
	void DisplayDct(DCTDATA * in_data, int x, int y, int m, int n);
	void DCT_Init(int * qtbl,int N);
	bool Write_JPEG_File(JSAMPLE * image_buffer, char * filename, int image_height, int image_width, int numcomponent,int quality);
	int Read_JPEG_File(JSAMPLE * image_buffer,char *filename);
	void put_scanline_someplace(JSAMPLE *image_buffer,JSAMPLE *buffer, int row_stride);
	void ReadBMP(char * dir);
	double ComputePSNR(RAWDATA * data1,RAWDATA * data2,int size,int bit1,int bit2);


	void ReturnBMPRaw(RAWDATA * Imagedata,int n);

};
#endif

