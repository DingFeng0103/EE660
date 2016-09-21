#ifndef _Lloyd_Max_Quantizer_
#define _Lloyd_Max_Quantizer_

#include<vector>
#include<iostream>
#include"FileManager.h"
#define Scaler 0
#define Vector 1
#define MaxSymbol 255
#define Tolerence 0.001

using namespace std;
class LDQuantizer
{
	vector<vector<double>> codebook;
	vector<double> dividers;
	vector<double> frequencies;
	vector<double> distribution;
	int num_symbols;
	int n;
	void Histogram(string str);// compute the histrogram. For convenience here use frequency.
	vector<vector<double>> LBGSplitInitial(string str);
	int  AssignCentroid(vector<double> word, vector<vector<double>> centroids);
	double distance(vector<double> word, vector<double> centroid);
	void UpdateCentroids(vector<vector<double>> words, vector<vector<double>> &centroids, vector<int> &assignments);
	void LBGSplitCodebook(vector<vector<double>> words, vector<vector<double>> &centroids);
	void LBGSplit(vector<vector<double>> words, vector<vector<double>> &centroids, vector<int> &assignments);
public:
	LDQuantizer();
	~LDQuantizer();
	void Initialize(int n,int instruction, int d);
	void Initialize(int n, int instruction);

	double ComputePSNR();
	double ComputePSNR(string str);
	double ComputeEntropy(string str);
	double ComputeEntropy(string dir,int n);
	void ScalerTraining(string sourcedir);
	void ScalerQuantize(string sourcedir,string destination);
	void ScalerQuantizeEncode(string sourcedir, string destination);
	void VectorQuantize(string sourcedir);

};

#endif