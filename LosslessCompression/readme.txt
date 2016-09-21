This Project contains 5 files:
Encoder.cpp
Encoder.h
FileManager.cpp
FileManager.h
Main.cpp

They are developed using Visual Studio 2013.

FileManager as its name suggest is a class used to manage file. 
Encoder is a tool class, it contains all kind of encoding method.

To do huffman or Shannon-Fano encoding, you should:

1.Change the path in:
filemanager.OpenFileToRead("data/binary.dat"); 
to any file you wish the encode, this is a relative path to the file folder.

2. Delete the comment symbol "//" in these lines.

//	encoder.SetString(filemanager.GetFileString());
//	encoder.ComputeNumOfOccurences();
//	encoder.ComputeFrequencies();
//	encoder.ComputeEntropy();

3. For ShannonFano encoding, delete commment symbol "//" in:
//	encoder.BuildShanonFanoTree(); //must setstring and compute entropy before build shannon-fano tree and encoding
//	encoder.Encode("SFtext.dat");

For Huffman encoding, delete commment symbol "//" in:
//	encoder.BuildHuffmanTree(); //must setstring and compute entropy before build huffman tree and encoding
//	encoder.Encode("HMbinary.dat");

4. Change the path in:
//	encoder.Encode("SFtext.dat"); 
or
//	encoder.Encode("HMbinary.dat");
to any name you want.

The parameter of is the path of outputfile.

To do adaptive huffman encoding or any other kind of encoding and decoding,
delete the comment symbol "//" in the corresponding code:
For adaptive huffman:
//	encoder.AdaptiveHuffmanEncoding("data/text.dat", "AHFtext.dat"); 
For basic run length:
//	encoder.BasicRunLengthEncoding("data/text.dat", "RLtext.dat");
//	encoder.BasicRunLengthDecoding("Runlengthbinary.dat", "DecodedRunlengthbinary.dat");
For Modifyed run length:
//	encoder.ModifiedRunLengthEncoding("data/text.dat", "MRLtext.dat");
//	encoder.ModifiedRunLengthDecoding("ModifiedRunlengthaudio.dat", "DecodedModifiedRunlengthaudio.dat");
For GolombRice:
//	encoder.GolombRiceEncoding("data/GolombRice.dat","Golomb.dat");
//	encoder.GolombRiceDecoding("Golomb.dat", "Decodedgolomb.dat");

First parameter of all functions are the path of input file. The second parameter are the path
of output file.


How to compile:
g++ -c FileManager.cpp
g++ -c Encoder.cpp
g++ -c Main.cpp
g++ -o Main Encoder.o FileManager.o Main.o
How to run:
./main