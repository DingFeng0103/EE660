#include "QMCoder.h"


QMCoder::QMCoder()
{
}


QMCoder::~QMCoder()
{
}

QMCoder::State::State()
{
	index = 0;
	Q_e = 0;
	Increaseby = 0;
	Descreasby = 0;
}

int QMCoder::State::getIndex() const
{
	return this->index;
}
void QMCoder::State::setIndex(int index)
{
	this->index = index;
}
int QMCoder::State::getQ_e() const
{
	return this->Q_e;
}
void QMCoder::State::setQ_e(int Q_e)
{
	this->Q_e = Q_e;
}
int QMCoder::State::getIncrease() const
{
	return this->Increaseby;
}
void QMCoder::State::setIncrease(int inc)
{
	this->Increaseby = inc;
}
int QMCoder::State::getDecreas()const
{
	return this->Descreasby;
}
void QMCoder::State::setDecrease(int dec)
{
	this->Descreasby = dec;
}

void QMCoder::ReadStateTransitionTable(string dir)
{
	ifstream inputfile(dir,ios::binary);
	string temps;
	int count = 0;
	if (inputfile.is_open()){
		while (getline(inputfile, temps)){
			State tempstate;
			char buff[100];
			strcpy_s(buff, temps.c_str());
			char *pEnd;
			
			tempstate.setIndex(strtol(buff, &pEnd, 10));
			tempstate.setQ_e(strtol(pEnd, &pEnd, 16));
			strtof(pEnd, &pEnd);
			tempstate.setIncrease(strtol(pEnd, &pEnd, 10));
			tempstate.setDecrease(strtol(pEnd, NULL, 10));
			
			State_Transition_Table.push_back(tempstate);
		}
	}
	inputfile.close();
}

void QMCoder::initialize(string dir,int MPS, int CurrentState)
{
	outputfile.OpenFile(dir, Write);
	this->A = 0x10000;
	this->C = 0x0000;
	this->StateIndex = CurrentState;
	this->Q_e = State_Transition_Table[this->StateIndex].getQ_e();
	this->carry = false;
	this->MPS = MPS;
	this->LPS = 1 - MPS;
}

void QMCoder::QMCoding(string str)
{
	int index = 0;
	while (index != str.size())
	{
		if ((unsigned char)(str[index] - '0') == MPS)
		{
			A = A - Q_e;;
			if (A < 0x8000){
				if (A < Q_e){
					if (C + A > 0xFFFF){
						carry = true;
					}
					C = C + A;
					A = Q_e;
				}
				//Change Q_e
				StateIndex += State_Transition_Table[StateIndex].getIncrease();
				Q_e = State_Transition_Table[StateIndex].getQ_e();
				//Renormalize
				renormalize();
			}
		}
		else if ((unsigned char)(str[index] - '0') == LPS)
		{
			A = A - Q_e;
			if (A >= Q_e){
				if (C + A > 0xFFFF){
					carry = true;
				}
				C = C + A;
				A = Q_e;
			}
			//Change Q_e
			StateIndex -= State_Transition_Table[StateIndex].getDecreas();
			if (State_Transition_Table[StateIndex].getDecreas() == -1){
				swap(LPS, MPS);
			}
			Q_e = State_Transition_Table[StateIndex].getQ_e();
			//Renomalize
			renormalize();
		}
		index++;
	}

}
void QMCoder::encode(string source,string destination,int currentMPS,int CurrentState,int mapfnc)
{
	// Initial
	initialize(destination, currentMPS, CurrentState);
	//Map symbols to binary sequence
	Encoder encoder;
	string str;
	cout << "Now Converting symbols to bits stream" << endl;
	if (mapfnc > 3 || mapfnc < 1){
		cout << "Error: there are only 3 method, please input method as 1 ~3" << endl;
		return;
	}
	else{
		switch (mapfnc){
		case 1: str = encoder.HuffmanEncodetoBitstring(source); break;
		case 2: str = encoder.RunLengthEncodetoBitstring(source); break;
		case 3: str = encoder.GolombRiceEncodetoBitstring(source); break;
		}
	}
	
	
	cout << "Now Qm coding" << endl;
	QMCoding(str);
	return;
}

void QMCoder::renormalize()
{
	if (carry){
		outputfile.WriteABit(1);
		carry = false;
	}
	while (A < 0x8000){
		A = A << 1;
		if ((C&(0x8000)) == 0){
			outputfile.WriteABit(0);
		}
		else{
			outputfile.WriteABit(1);
		}
		C <<= 1;

	}
}
vector<string> QMCoder::ParseString(string str, int n)
{
	if (n == 0){
		cout << "n=0, This is not ContextAdaptive QM coding, please do normal QM Coding" << endl;
	}else if(n>3){
		cout << "n is too big, please input n from 1 to 3" << endl;
	}
	string temps(n,'0');
	str = temps + str;
	vector<string> strs(pow(2,n));
	int index = 0;
	for (int i = n; i < str.size(); i++){
		index=stoi(str.substr(i - n, n), nullptr, 2);
		strs[index] = strs[index] + str[i];
	}
	return strs;

}
void QMCoder::ContextBasedQMCoding(string source, string destination, int currentMPS, int CurrentState,int mapfnc, int n)
{
	// Initial
	initialize(destination, currentMPS, CurrentState);
	//Map symbols to binary sequence
	Encoder encoder;
	string str;
	cout << "Now Converting symbols to bits stream" << endl;
	if (mapfnc > 3 || mapfnc < 1){
		cout << "Error: there are only 3 method, please input method as 1 ~3" << endl;
		return;
	}
	else{
		switch (mapfnc){
		case 1: str = encoder.HuffmanEncodetoBitstring(source); break;
		case 2: str = encoder.RunLengthEncodetoBitstring(source); break;
		case 3: str = encoder.GolombRiceEncodetoBitstring(source); break;
		}
	}
	vector<string> strs=ParseString(str,n);

	for (int i = 0; i < strs.size(); i++){
		QMCoding(strs[i]);
	}
	return;
}

void QMCoder::PreprocessingImage(string source,string destination,int instruction)
{
	FileManager inputfile(source, Read);
	int totallength=inputfile.GetSize();
	int m=sqrt(totallength);
	char *buffer=new char[totallength];
	
	inputfile.ReadBytes(buffer, totallength);


	if (instruction == preprocessing1){
		int x = 0;
		int y = 0;
		for (x = 1; x < m; x += 2){
			for (y = 0; y < m / 2; y++){
				swap(*(buffer + x*m + y), *(buffer + x*m + m - y - 1));
			}
		}
		FileManager outputfile(destination, Write);
		outputfile.WriteString(buffer, totallength);
	}
	else if (instruction == preprocessing2){
		char *outputbuffer = new char[totallength];
		int x = 0;
		int y = 0;
		int count = 0;
		for (y = 0; y < m; y++){
			if (y % 2 == 0){
				for (x = 0; x < m; x++){
					*(outputbuffer+count)=*(buffer + x*m + y);
					count++;
				}
			}
			else{
				for (x = 0; x < m; x++){
					*(outputbuffer+count)=*(buffer + (m-1-x)*m + y);
				}
			}
		}
		FileManager outputfile(destination, Write);
		outputfile.WriteString(outputbuffer, totallength);
	}
	else if (instruction == preprocessing3){
		int count = 0;
		for (count = 1; count < m * 2 - 2; count++){
			if (count < m){
				for (int i = 0; i <= count / 2; i++){
					swap(*(buffer + (count - i) + i*m), *(buffer + (count - i)*m + i));
				}
			}
			else{
				for (int i = 0; i <= (m*2-2-count) / 2; i++){
					swap(*(buffer + (m - 1 - i) + (count + 1 - m + i)*m), *(buffer + (m - 1 - i)*m + count + 1 - m + i));
				}
			}
		}
		FileManager outputfile(destination, Write);
		outputfile.WriteString(buffer, totallength);
	}
	//inputfile.
}