#ifndef	_QM_CODING_
#define	_QM_CODING_

#include<vector>
#include<iostream>
#include<string>
#include<fstream>
#include"FileManager.h"
#include"Encoder.h"
#define preprocessing1 0 //horizontal zigzag way processing
#define preprocessing2 1// vertical zigzag way processing
#define preprocessing3 2// slide zigzag way processing
using namespace std;

class QMCoder
{
	class State{
		int index;
		int Q_e;
		int Increaseby;	
		int Descreasby; // 0 means switch state
	public:
		State();
		int getIndex() const;
		void setIndex(int index);
		int getQ_e() const;
		void setQ_e(int Q_e);
		int getIncrease() const;
		void setIncrease(int inc);
		int getDecreas() const;
		void setDecrease(int dec);
	};
	vector<State> State_Transition_Table;
	unsigned int A;
	unsigned int Q_e;
	unsigned int C;
	int StateIndex;
	bool carry;
	int MPS;
	int LPS;
	void initialize(string dir,int MPS,int CurrentState);
	void renormalize();
//	void QMCoding(string str);
	vector<string> ParseString(string str,int n);
	FileManager outputfile;
public:
	void QMCoding(string str);
	void ReadStateTransitionTable(string dir);
	void PreprocessingImage(string dir,string destination,int instruction);
	void encode(string source,string destination, int MPS, int CurrentState,int mapfnc);
	void ContextBasedQMCoding(string source, string destination, int MPS, int CurrentState, int mfnc, int n);
	QMCoder();
	~QMCoder();
};

#endif // _QM_CODING_