#include "FileManager.h"


FileManager::FileManager()
{
}


FileManager::~FileManager()
{
}

void FileManager::OpenFileToRead(string path)
{
	//Open a file, prepare for input
	inputfilepath = path;
	inputfile.open(path,ifstream::binary);
};
void FileManager::OpenFileToWrite(string path)
{
	//Open a file, prepare for output
	outputfilepath = path;
	outputfile.open(path, ofstream::binary);
};

string FileManager::ReadFromFile(string path)
{
	//read from a new file with path

	inputfile.open(path);
	string line;
	if (inputfile.is_open()){
		while (getline(inputfile, line)){
			filestring.append(line);
			filestring.append("\n");
		}
	}
	return filestring;

};

void FileManager::ReadFromFile()
{
	//read from the file stored in inputfile
	string line;

	if (inputfile.is_open()){
		while (getline(inputfile, line)){
			filestring.append(line); 
			filestring.append("\n");
		}
	}
	else{
		cout << "Please input the path of file" << endl;
	}

};

string FileManager::GetFileString()
{
	if (filestring.empty()){
		//in case filestring is empty now.
		cout << "Error: the file string is currently empty, read from file first" << endl;
		cout << "Now reading from file: " << inputfilepath << endl;
		ReadFromFile();
		cout << "Please re-getfilestring again" << endl;
	}
	else{
		return filestring;
	}
};

string FileManager::GetInputFilePath()
{
	return inputfilepath;

};
void FileManager::CloseOutputFile()
{
	outputfile.close();
};
void FileManager::CloseInputFile()
{
	inputfile.close();
};