#include<iostream>
#include"JPEGCoder.h"

using namespace std;
int main(){
	JPEGCoder jpegcoder;
	
	int N = 50;

	RAWDATA lenaface[BLOCKSIZE][BLOCKSIZE][1];
	RAWDATA block[DCTSIZE][DCTSIZE];
	DCTDATA DCTMaxtrix[DCTSIZE][DCTSIZE][1];
	jpegcoder.ReadRaw("data/lena_face.raw",(RAWDATA *)lenaface, BLOCKSIZE);
	
	cout << "Now input K: (1~99)" << endl;
	cin >> N;
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 2; j++){
			printf("For the (%d,%d) block\n", j + 1, i + 1);
			jpegcoder.SubBlock((RAWDATA *)lenaface, BLOCKSIZE, BLOCKSIZE, (RAWDATA *)block, j*DCTSIZE, i * DCTSIZE, DCTSIZE, DCTSIZE);
			jpegcoder.DCT_Init((int *)qtable_jpeg, N);
			jpegcoder.DCT((RAWDATA *)block, (DCTDATA *)DCTMaxtrix);
			jpegcoder.DisplayDct((DCTDATA *)DCTMaxtrix, 0, 0, DCTSIZE, DCTSIZE);
			cout << endl << endl;
		}
	}
	

	
	cout << "Writing raw into JPEG "<< endl;
	const int BytesPerPixel = 1;
	RAWDATA Imagedata[FILESIZE][FILESIZE][1];


	jpegcoder.ReadBMP("data/clock.bmp");
	jpegcoder.ReturnBMPRaw((RAWDATA *)Imagedata, IMAGESIZE);

	jpegcoder.Write_JPEG_File((JSAMPLE *)Imagedata, "Results/clock100.jpg", 256, 256,1, 100);
	jpegcoder.Write_JPEG_File((JSAMPLE *)Imagedata, "Results/clock60.jpg", 256, 256, 1, 60);
	jpegcoder.Write_JPEG_File((JSAMPLE *)Imagedata, "Results/clock40.jpg", 256, 256, 1, 40);
	jpegcoder.Write_JPEG_File((JSAMPLE *)Imagedata, "Results/clock10.jpg", 256, 256, 1, 10);
	jpegcoder.Write_JPEG_File((JSAMPLE *)Imagedata, "Results/clock1.jpg", 256, 256, 1, 1);


	cout << "Output pepper.jpg PSNR" << endl;
	
	RAWDATA Imagedata1[FILESIZE][FILESIZE][3];
	RAWDATA Imagedata2[FILESIZE][FILESIZE][3];
	jpegcoder.ReadRaw("data/pepper.raw", (RAWDATA *)Imagedata1, 3);
	jpegcoder.Read_JPEG_File((RAWDATA *)Imagedata2, "data/pepper1.jpg");
	cout << jpegcoder.ComputePSNR((RAWDATA *)Imagedata1, (RAWDATA *)Imagedata2, FILESIZE,3,3) << endl;
	jpegcoder.Read_JPEG_File((RAWDATA *)Imagedata2, "data/pepper2.jpg");
	cout << jpegcoder.ComputePSNR((RAWDATA *)Imagedata1, (RAWDATA *)Imagedata2, FILESIZE, 3, 3) << endl;
	jpegcoder.Read_JPEG_File((RAWDATA *)Imagedata2, "data/pepper3.jpg");
	cout << jpegcoder.ComputePSNR((RAWDATA *)Imagedata1, (RAWDATA *)Imagedata2, FILESIZE, 3, 3) << endl;
	jpegcoder.Read_JPEG_File((RAWDATA *)Imagedata2, "data/pepper4.jpg");
	cout << jpegcoder.ComputePSNR((RAWDATA *)Imagedata1, (RAWDATA *)Imagedata2, FILESIZE, 3, 3) << endl;
	jpegcoder.Read_JPEG_File((RAWDATA *)Imagedata2, "data/pepper5.jpg");
	cout << jpegcoder.ComputePSNR((RAWDATA *)Imagedata1, (RAWDATA *)Imagedata2, FILESIZE, 3, 3) << endl;


	getchar();
}