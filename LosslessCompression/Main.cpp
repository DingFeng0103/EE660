
#include"FileManager.h"
#include"Encoder.h"
#include<ctime>

using namespace std;

int main() 
{

	
	int start_s = clock();
	
	FileManager filemanager;
	Encoder encoder;
	filemanager.OpenFileToRead("data/binary.dat");
	filemanager.ReadFromFile();
	filemanager.CloseInputFile();

//	encoder.SetString(filemanager.GetFileString());
//	encoder.ComputeNumOfOccurences();
//	encoder.ComputeFrequencies();
//	encoder.ComputeEntropy();
	
//	encoder.BuildShanonFanoTree(); //must setstring and compute entropy before build shannon-fano tree and encoding
//	encoder.Encode("SFtext.dat");
	
//	encoder.BuildHuffmanTree(); //must setstring and compute entropy before build huffman tree and encoding
//	encoder.Encode("HMbinary.dat");

//	encoder.AdaptiveHuffmanEncoding("data/text.dat", "AHFtext.dat"); 

//	encoder.BasicRunLengthEncoding("data/text.dat", "RLtext.dat");
//	encoder.BasicRunLengthDecoding("Runlengthbinary.dat", "DecodedRunlengthbinary.dat");

//	encoder.ModifiedRunLengthEncoding("data/text.dat", "MRLtext.dat");
//	encoder.ModifiedRunLengthDecoding("ModifiedRunlengthaudio.dat", "DecodedModifiedRunlengthaudio.dat");

//	encoder.GolombRiceEncoding("data/GolombRice.dat","Golomb.dat");
//	encoder.GolombRiceDecoding("Golomb.dat", "Decodedgolomb.dat");
	cout << "All done press any key to exist" << endl;
	int stop_s = clock();
	cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) << endl;
	getchar();
	return 0;
}