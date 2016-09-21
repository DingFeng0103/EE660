#include "JPEGCoder.h"



JPEGCoder::JPEGCoder()
{

}


JPEGCoder::~JPEGCoder()
{
}

void JPEGCoder::SubBlock(RAWDATA * in_data,int M,int N, RAWDATA *out_data,int x,int y,int m,int n)
{
	for (int i = 0; i < m; i++){
		for (int j = 0; j < n; j++){
			*(out_data + i*m + j) = *(in_data + (x + i)*M + (y + j));
		}
	}
}
void JPEGCoder::DisplayRaw(RAWDATA * in_data, int x, int y, int m, int n)
{
	for (int i = x; i <x+ m; i++){
		for (int j = y; j <y+ n; j++){
			cout << (unsigned int)*(in_data+i*m+j)<<' ';
		}
		cout << endl;
	}
}
void JPEGCoder::DisplayDct(DCTDATA * in_data, int x,int y,int m, int n)
{
	for (int i = x; i < x+m; i++){
		for (int j = y; j < y+n; j++){
			printf("%4d", (int)*(in_data + i*m + j));
		}
		cout << endl;
	}
}
void JPEGCoder::ReadRaw(string dir, RAWDATA *Imagedata, int n)
{
	FileManager file(dir, Read);
	const int BytesPerPixel = n;
	if (!file.Is_Open())
	{
		cout << "Cannot open file: " << dir << endl;
		exit(1);
	}
	
	file.ReadNBytes((unsigned char *)Imagedata, FILESIZE*FILESIZE*BytesPerPixel);
}

void JPEGCoder::ReadBMP(char * dir)
{
	FILE *f;

	fopen_s(&f, dir, "rb");
	unsigned char info[HEADERSIZE];
	if (f != NULL){
		cout << sizeof(BITMAPFILEHEADER) << endl;
		cout << sizeof(BITMAPINFOHEADER) << endl;
		fread(info, sizeof(unsigned char), HEADERSIZE, f);
		this->bmpfile.file_header = (PBITMAPFILEHEADER)&info[0];
		this->bmpfile.info_header = (PBITMAPINFOHEADER)(&info[0] + sizeof(BITMAPFILEHEADER));
		unsigned char buffer[IMAGESIZE*IMAGESIZE];
		fread(buffer, sizeof(unsigned char), this->bmpfile.file_header->bfOffBits - HEADERSIZE, f);
		fread(buffer, sizeof(unsigned char), this->bmpfile.info_header->biSizeImage, f);
		this->bmpfile.rawdata = buffer;
	}
	fclose(f);
}

void JPEGCoder::ReturnBMPRaw(RAWDATA *Imagedata, int n)
{
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			*(Imagedata + (n - i - 1)*n + j) = (RAWDATA)(*(bmpfile.rawdata + i*n + j));
		}
	}
}
void JPEGCoder::DCT(RAWDATA * in_data, DCTDATA * out_data)
{
	//This function is modified from http://www.cbloom.com/src/index_im.html#A1.2
	double tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
	double tmp10, tmp11, tmp12, tmp13;
	double z1, z2, z3, z4, z5, z11, z13;
	double *dataptr;
	double data[DCTSIZE2];
	int ctr, i;

	/* step 0 : read in doubles , make data signed */

	dataptr = data;
	for (i = 0; i<DCTSIZE; i++) {
		*dataptr++ = (double)(*in_data++ - RAWCENTER);
		*dataptr++ = (double)(*in_data++ - RAWCENTER);
		*dataptr++ = (double)(*in_data++ - RAWCENTER);
		*dataptr++ = (double)(*in_data++ - RAWCENTER);
		*dataptr++ = (double)(*in_data++ - RAWCENTER);
		*dataptr++ = (double)(*in_data++ - RAWCENTER);
		*dataptr++ = (double)(*in_data++ - RAWCENTER);
		*dataptr++ = (double)(*in_data++ - RAWCENTER);
	}

	/* Pass 1: process rows. */

	dataptr = data;
	for (ctr = DCTSIZE - 1; ctr >= 0; ctr--) {
		tmp0 = dataptr[0] + dataptr[7];
		tmp7 = dataptr[0] - dataptr[7];
		tmp1 = dataptr[1] + dataptr[6];
		tmp6 = dataptr[1] - dataptr[6];
		tmp2 = dataptr[2] + dataptr[5];
		tmp5 = dataptr[2] - dataptr[5];
		tmp3 = dataptr[3] + dataptr[4];
		tmp4 = dataptr[3] - dataptr[4];

		/* Even part */

		tmp10 = tmp0 + tmp3;	/* phase 2 */
		tmp13 = tmp0 - tmp3;
		tmp11 = tmp1 + tmp2;
		tmp12 = tmp1 - tmp2;

		dataptr[0] = tmp10 + tmp11; /* phase 3 */
		dataptr[4] = tmp10 - tmp11;

		z1 = (tmp12 + tmp13) * ((double) 0.707106781); /* c4 */
		dataptr[2] = tmp13 + z1;	/* phase 5 */
		dataptr[6] = tmp13 - z1;

		/* Odd part */

		tmp10 = tmp4 + tmp5;	/* phase 2 */
		tmp11 = tmp5 + tmp6;
		tmp12 = tmp6 + tmp7;

		/* The rotator is modified from fig 4-8 to avoid extra negations. */
		z5 = (tmp10 - tmp12) * ((double) 0.382683433); /* c6 */
		z2 = ((double) 0.541196100) * tmp10 + z5; /* c2-c6 */
		z4 = ((double) 1.306562965) * tmp12 + z5; /* c2+c6 */
		z3 = tmp11 * ((double) 0.707106781); /* c4 */

		z11 = tmp7 + z3;		/* phase 5 */
		z13 = tmp7 - z3;

		dataptr[5] = z13 + z2;	/* phase 6 */
		dataptr[3] = z13 - z2;
		dataptr[1] = z11 + z4;
		dataptr[7] = z11 - z4;

		dataptr += DCTSIZE;		/* advance pointer to next row */
	}

	/* Pass 2: process columns. */

	dataptr = data;
	for (ctr = DCTSIZE - 1; ctr >= 0; ctr--) {
		tmp0 = dataptr[DCTSIZE * 0] + dataptr[DCTSIZE * 7];
		tmp7 = dataptr[DCTSIZE * 0] - dataptr[DCTSIZE * 7];
		tmp1 = dataptr[DCTSIZE * 1] + dataptr[DCTSIZE * 6];
		tmp6 = dataptr[DCTSIZE * 1] - dataptr[DCTSIZE * 6];
		tmp2 = dataptr[DCTSIZE * 2] + dataptr[DCTSIZE * 5];
		tmp5 = dataptr[DCTSIZE * 2] - dataptr[DCTSIZE * 5];
		tmp3 = dataptr[DCTSIZE * 3] + dataptr[DCTSIZE * 4];
		tmp4 = dataptr[DCTSIZE * 3] - dataptr[DCTSIZE * 4];

		/* Even part */

		tmp10 = tmp0 + tmp3;	/* phase 2 */
		tmp13 = tmp0 - tmp3;
		tmp11 = tmp1 + tmp2;
		tmp12 = tmp1 - tmp2;

		dataptr[DCTSIZE * 0] = tmp10 + tmp11; /* phase 3 */
		dataptr[DCTSIZE * 4] = tmp10 - tmp11;

		z1 = (tmp12 + tmp13) * ((double) 0.707106781); /* c4 */
		dataptr[DCTSIZE * 2] = tmp13 + z1; /* phase 5 */
		dataptr[DCTSIZE * 6] = tmp13 - z1;

		/* Odd part */

		tmp10 = tmp4 + tmp5;	/* phase 2 */
		tmp11 = tmp5 + tmp6;
		tmp12 = tmp6 + tmp7;

		/* The rotator is modified from fig 4-8 to avoid extra negations. */
		z5 = (tmp10 - tmp12) * ((double) 0.382683433); /* c6 */
		z2 = ((double) 0.541196100) * tmp10 + z5; /* c2-c6 */
		z4 = ((double) 1.306562965) * tmp12 + z5; /* c2+c6 */
		z3 = tmp11 * ((double) 0.707106781); /* c4 */

		z11 = tmp7 + z3;		/* phase 5 */
		z13 = tmp7 - z3;

		dataptr[DCTSIZE * 5] = z13 + z2; /* phase 6 */
		dataptr[DCTSIZE * 3] = z13 - z2;
		dataptr[DCTSIZE * 1] = z11 + z4;
		dataptr[DCTSIZE * 7] = z11 - z4;

		dataptr++;			/* advance pointer to next column */
	}

	/* step 3: Quantize/descale the coefficients, and store into coef_blocks[] */
	for (i = 0; i < DCTSIZE2; i++) {
		/* Apply the quantization and scaling factor */
		out_data[i] = (DCTDATA)(data[i] * divisors[i]);

		/* Round to nearest integer.
		* Since C does not specify the direction of rounding for negative
		* quotients, we have to force the dividend positive for portability.
		* The maximum coefficient size is +-16K (for 12-bit data), so this
		* code should work for either 16-bit or 32-bit ints.
		*/
		/** out_data[i] = (DCTDATA) ((int) (data[i] * divisors[i] + (double) 16384.5)
		*		 - 16384);
		**/
	}

}

void JPEGCoder::DCT_Init(int * qtbl,int N)
{
	// This function id modified from http://www.cbloom.com/src/index_im.html#A1.2
	int row, col, i;
	static const double aanscalefactor[DCTSIZE] = {
		1.0, 1.387039845, 1.306562965, 1.175875602,
		1.0, 0.785694958, 0.541196100, 0.275899379
	};

	/* For double AA&N IDCT method, divisors are equal to quantization
	* coefficients scaled by scalefactor[row]*scalefactor[col], where
	*   scalefactor[0] = 1
	*   scalefactor[k] = cos(k*PI/16) * sqrt(2)    for k=1..7
	* We apply a further scale factor of 8.
	* What's actually stored is 1/divisor so that the inner loop can
	* use a multiplication rather than a division.
	*/

	i = 0;
	double k = 0;
	for (row = 0; row < DCTSIZE; row++) {
		for (col = 0; col < DCTSIZE; col++) {
			if (N>50)
				k = (double)(100 - N) / 50;
			else
				k = (double)50 / N;
			divisors[i] = (double)
				(1.0 / (((double)qtbl[i] * aanscalefactor[row] * aanscalefactor[col] * 8.0)));
			divisors[i] = divisors[i] / k;
			i++;
		}
	}
}

bool JPEGCoder::Write_JPEG_File(JSAMPLE * image_buffer, char * filename, int image_height, int image_width,int ncomponent, int quality)
{
	/* Step 1: allocate and initialize JPEG compression object */

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	FILE *outfile;
	JSAMPROW row_pointer[1];
	int row_stride = image_width * ncomponent;
	/* Step 2: specify data destination (eg, a file) */
	
	if ((fopen_s(&outfile,filename, "wb")))
	{
		cout << "Can't open " << filename << endl;
		return false;
	}
	jpeg_stdio_dest(&cinfo, outfile);
	
	/* Step 3: set parameters for compression */
	cinfo.image_width = 256;
	cinfo.image_height = 256;
	cinfo.input_components = ncomponent;
	if (ncomponent == 1){
		cinfo.in_color_space = JCS_GRAYSCALE;
	}
	else if (ncomponent == 3){
		cinfo.in_color_space = JCS_RGB;
	}
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, true);

	/* Step 4: Start compressor */
	jpeg_start_compress(&cinfo, true);

	/* Step 5: while (scan lines remain to be written) */
	/*           jpeg_write_scanlines(...); */
	
	
	while (cinfo.next_scanline < cinfo.image_height)
	{
		row_pointer[0] = &image_buffer[cinfo.next_scanline*row_stride];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}
	/* Step 6: Finish compression */
	jpeg_finish_compress(&cinfo);
	fclose(outfile);

	/* Step 7: release JPEG compression object */
	jpeg_destroy_compress(&cinfo);
	return true;
}

struct my_error_mgr {
	struct jpeg_error_mgr pub;	/* "public" fields */

	jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

/*
* Here's the routine that will replace the standard error_exit method:
*/

METHODDEF(void)
my_error_exit(j_common_ptr cinfo)
{
	/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
	my_error_ptr myerr = (my_error_ptr)cinfo->err;

	/* Always display the message. */
	/* We could postpone this until after returning, if we chose. */
	(*cinfo->err->output_message) (cinfo);

	/* Return control to the setjmp point */
	longjmp(myerr->setjmp_buffer, 1);
}

int JPEGCoder::Read_JPEG_File(JSAMPLE * image_buffer,char *filename)
{
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	FILE * infile;		/* source file */
	JSAMPARRAY buffer;		/* Output row buffer */
	int row_stride;

	if ((fopen_s(&infile,filename, "rb"))) {
		fprintf(stderr, "can't open %s\n", filename);
		return 0;
	}
	/* Step 1: allocate and initialize JPEG decompression object */

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	/* Establish the setjmp return context for my_error_exit to use. */
	if (setjmp(jerr.setjmp_buffer)) {
		/* If we get here, the JPEG code has signaled an error.
		* We need to clean up the JPEG object, close the input file, and return.
		*/
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return 0;
	}
	/* Now we can initialize the JPEG decompression object. */
	jpeg_create_decompress(&cinfo);

	/* Step 2: specify data source (eg, a file) */

	jpeg_stdio_src(&cinfo, infile);

	/* Step 3: read file parameters with jpeg_read_header() */

	(void)jpeg_read_header(&cinfo, TRUE);
	/* We can ignore the return value from jpeg_read_header since
	*   (a) suspension is not possible with the stdio data source, and
	*   (b) we passed TRUE to reject a tables-only JPEG file as an error.
	* See libjpeg.txt for more info.
	*/

	/* Step 4: set parameters for decompression */

	/* In this example, we don't need to change any of the defaults set by
	* jpeg_read_header(), so we do nothing here.
	*/

	/* Step 5: Start decompressor */

	(void)jpeg_start_decompress(&cinfo);
	/* We can ignore the return value since suspension is not possible
	* with the stdio data source.
	*/

	/* We may need to do some setup of our own at this point before reading
	* the data.  After jpeg_start_decompress() we have the correct scaled
	* output image dimensions available, as well as the output colormap
	* if we asked for color quantization.
	* In this example, we need to make an output work buffer of the right size.
	*/
	/* JSAMPLEs per row in output buffer */
	row_stride = cinfo.output_width * cinfo.output_components;
	/* Make a one-row-high sample array that will go away when done with image */
	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

	/* Step 6: while (scan lines remain to be read) */
	/*           jpeg_read_scanlines(...); */

	/* Here we use the library's state variable cinfo.output_scanline as the
	* loop counter, so that we don't have to keep track ourselves.
	*/
	while (cinfo.output_scanline < cinfo.output_height) {
		/* jpeg_read_scanlines expects an array of pointers to scanlines.
		* Here the array is only one element long, but you could ask for
		* more than one scanline at a time if that's more convenient.
		*/
		(void)jpeg_read_scanlines(&cinfo, buffer, 1);
		/* Assume put_scanline_someplace wants a pointer and sample count. */
		put_scanline_someplace(image_buffer + cinfo.output_scanline*cinfo.image_width, buffer[0], row_stride);
	}

	/* Step 7: Finish decompression */

	(void)jpeg_finish_decompress(&cinfo);
	/* We can ignore the return value since suspension is not possible
	* with the stdio data source.
	*/

	/* Step 8: Release JPEG decompression object */

	/* This is an important step since it will release a good deal of memory. */
	jpeg_destroy_decompress(&cinfo);

	/* After finish_decompress, we can close the input file.
	* Here we postpone it until after no more JPEG errors are possible,
	* so as to simplify the setjmp error logic above.  (Actually, I don't
	* think that jpeg_destroy can do an error exit, but why assume anything...)
	*/
	fclose(infile);

	/* At this point you may want to check to see whether any corrupt-data
	* warnings occurred (test whether jerr.pub.num_warnings is nonzero).
	*/

	/* And we're done! */
	return 1;
}

void JPEGCoder::put_scanline_someplace(JSAMPLE *image_buffer, JSAMPLE *buffer, int row_stride)
{
	for (int i = 0; i < row_stride; i++){
		*(image_buffer + i) = *(buffer + i);
	}
}

double JPEGCoder::ComputePSNR(RAWDATA * data1, RAWDATA * data2, int size,int bit1,int bit2)
{
	double mse = 0;
	if (bit1 == 1 && bit2 == 1){
		
		for (int i = 0; i < size; i++){
			for (int j = 0; j < size; j++){
				mse += pow(*(data1 + i*size + j) - *(data2 + i*size + j), 2);
			}
		}
		mse = mse / size / size;
	}
	else if (bit1 == 3 && bit2 == 3){
		for (int i = 0; i < size; i++){
			for (int j = 0; j < size; j++){
				for (int k = 0; k < bit1;k++){
					mse += pow(*(data1 + i*size*bit1 + j*bit1 + k) - *(data2 + i*size*bit1 + j*bit1 + k), 2);
				}

			}
		}
		mse = mse / size / size/bit1;
	}
	else if (bit1 == 1 && bit2 == 3){
		for (int i = 0; i < size; i++){
			for (int j = 0; j < size; j++){
				mse += pow(*(data1 + i*size + j) - *(data2 + i*size*bit2 + j*bit2+1), 2);
			}
		}
		mse = mse / size / size;
	}
	
	return 10 * log10((double)pow(255, 2) / mse);
}

