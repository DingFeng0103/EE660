#include "FileManager.h"

FileManager::FileManager()
{
	countbit = 8;
}
FileManager::FileManager(string dir, int instruction)
{
	if (instruction == Write){
		file.open(dir,ios::out|ios::binary);
	}
	else if (instruction == Read){
		file.open(dir, ios::in|ios::binary);
	}
	else{
		cout << "Wrong instruction: Please tell to read or write" << endl;
	}
	countbit = 8;
}


FileManager::~FileManager()
{
}

char FileManager::ReadAByte()
{
	return file.get();
}
int FileManager::GetSize()
{
	int size=0;
	if (!file.is_open())
		perror("Error opening file");
	else{
		file.seekg(0, file.end);
		size = file.tellg();
		file.seekg(0, file.beg);
	}
	return size;
}
void FileManager::ReadBytes(char* str,int n)
{
	file.read(str,n);
}
void FileManager::WriteABit(bool MSB)
{
	bufferbyte <<= 1;
	bufferbyte |= MSB;
	countbit--;
	if (countbit == 0){
		file.put(bufferbyte);
		countbit = 8;
		bufferbyte = 0;
	}
	else
		return;

}

bool FileManager::EndofFile()
{
	return file.eof();
}

void FileManager::OpenFile(string dir, int instruction)
{
	if (instruction == Write){
		file.open(dir, ios::out | ios::binary);
	}
	else if (instruction == Read){
		file.open(dir, ios::in | ios::binary);
	}
	else{
		cout << "Wrong instruction: Please tell to read or write" << endl;
	}
}

void FileManager::WriteString(char *str,int n)
{
	file.write(str,n);
}

string FileManager::ReadStrings()
{

	int l = this->GetSize();
	char *buffer = new char[l];
	this->ReadBytes(buffer, l);
	string str(buffer,l);
	return str;
}

void FileManager::WriteNBits(int x, int n)
{
	if (x >= pow(2, n))
		perror("Error: X can't be larger then 2^n.");
	int temp;
	n--;
	while(n>=0){
		temp = pow(2, n);
		this->WriteABit(x/temp);
		x %= temp;
		n--;
	}
}

bool FileManager::Is_Open()
{
	if (this->file.is_open())
		return true;
	else
		return false;
}

void FileManager::ReadNBytes(unsigned char *buffer, int n)
{
	this->file.read((char *)buffer, n);
}

void FileManager::Close()
{
	this->file.close();
}