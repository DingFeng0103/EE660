#include"QMCoder.h"

int main(){
	QMCoder qmcoder;
	/*
	cout << "Enter relative file path to this directory with full name" << endl;
	char indir[512];
	cin >> indir;

	cout << "Now enter the filename you want to encode as" << endl;
	char outdir[512];
	cin >> outdir;

	cout << "Now set initial paramters MPS: " << endl;
	int MPS = 0;
	cin >> MPS;

	cout << "Now set initial paramters Current state: " << endl;
	int cs = 10;
	cin >> cs;

	cout << "Now Choose the Mapping function: " << endl;
	cout << "1. Huffman Coding " << endl;
	cout << "2. Run-length Coding " << endl;
	cout << "3. Golomb-Rice Coding " << endl;
	int method = 1;
	cin >> method;

	cout << "Please tell if you want to do basic QM coding or Context-based QM Coding" << endl;
	cout << "1. Basic QM Coding" << endl;
	cout << "2. Context-Based QM Coding" << endl;
	if (cin.get() == 1){
		qmcoder.ReadStateTransitionTable("QM_state_transition_table.txt");
		cout << "Now coding..." << endl;
		qmcoder.encode(indir, outdir, MPS, cs, method);
	}
	else if (cin.get() == 2){
		cout << "please input Number of n:" << endl;
		int n;
		cin >> n;
		qmcoder.ReadStateTransitionTable("QM_state_transition_table.txt");
		qmcoder.ContextBasedQMCoding(indir, outdir, MPS, cs, method,n);
	}
	*/
	/*For debug only*/
	/*
	string indir = "data/text.dat";
	string outdir = "Results/CBBAC/text1.dat";
	int MPS = 0;
	int cs = 10;
	int method = 1;
	int n =1;
	qmcoder.ReadStateTransitionTable("QM_state_transition_table.txt");
	qmcoder.ContextBasedQMCoding(indir, outdir, MPS, cs, method, n);
	*/
	/*
	cout << "Now doing pre-processing" << endl;
	cout << "Select preprocessing method:" << endl;
	cout << "1. Horizontal Zigzag preprocessing" << endl;
	cout << "2. Vertical Zigzag preprocessing" << endl;
	cout << "3. Slide Zigzag preprocessing" << endl;
	int m;
	cin >> m;
	m--;*/
	qmcoder.PreprocessingImage("data/image.dat", "data/image-preprocessed3.dat", preprocessing3);


	string indir = "data/image-preprocessed3.dat";
	string outdir = "Results/CBBAC/image-preprocessedQM3.dat";
	int MPS = 0;
	int cs = 10;
	int method = 1;
	qmcoder.ReadStateTransitionTable("QM_state_transition_table.txt");
	qmcoder.encode(indir, outdir, MPS, cs, method);
	

}
