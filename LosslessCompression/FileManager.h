// This file is used to manage files, open, read, write
#pragma once
#include <iostream>
#include <fstream>
#include<string>

using namespace std;

class FileManager
{
private:
	string filestring;	//store the input or output file;
	ifstream inputfile;	//an input file class
	string inputfilepath;

	ofstream outputfile;//an output file class
	string outputfilepath;
public:
	void OpenFileToRead(string path);
	void OpenFileToWrite(string path);
	void CloseOutputFile();
	void CloseInputFile();
	string ReadFromFile(string path);
	void ReadFromFile();
	string GetFileString();
	string GetInputFilePath();
	FileManager();
	~FileManager();
};

