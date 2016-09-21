#ifndef _FileManager_
#define _FileManager_

#include<string>
#include<fstream>
#include<iostream>

#define Read	0
#define Write	1
using namespace std;
class FileManager
{
	fstream file;
	char bufferbyte;
	int countbit;
public:
	FileManager();
	FileManager(string dir,int instruction);
	~FileManager();
	void OpenFile(string dir, int instruction);
	void Close();
	void WriteABit(bool MSB);
	void WriteNBits(int x, int n);
	void WriteString(char *str,int n);
	char ReadAByte();
	int	 GetSize();
	void ReadBytes(char *str,int n);
	void ReadNBytes(unsigned char *buffer, int n);
	bool Is_Open();
	string ReadStrings();
	bool EndofFile();
};

#endif 