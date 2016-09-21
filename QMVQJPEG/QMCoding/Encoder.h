// This class is used as a tool class, it has the main function of encoding
#pragma once
#include<string>
#include<vector>
#include<iostream>
#include<cmath>
#include<algorithm>
#include<functional>
#include<bitset>
#include"FileManager.h"

#define MAX_BYTE_SIZE 256


using namespace std;
class Encoder
{
private:
	string bufferstring;		//used to store being processed filestring.
	int stringlength;			//store the length of the string.
	vector<int> num_of_times;	//store the num of times of each character.
	vector<double> frequencies;	//store the frequency distribution of each character.
	
	double entropy;				//store the entropy.

	class DistributionTable
	{
		//set member to public for easy manipulate
	public:
		char character;
		int num_of_occurence=0;
		double frequency;

	};
	vector<DistributionTable> char_prob_dist;

	class BinaryTree
	{	
		//set member to public for easy manipulate
	public:
		double frequencysum;
		string code;
		vector<DistributionTable> chars_prob_dist_of_this_branch;
		BinaryTree *branches[2];
	};
	BinaryTree shanonfano_tree_root;

	class HuffmanBinaryTree{
	public:
		HuffmanBinaryTree();
		double weight;
		string code;
		string chars_inside;//store characters that belong to this tree.
		HuffmanBinaryTree *branches[2];

	};

	class AdaptiveHuffmanBinaryTree
	{
	public:
		AdaptiveHuffmanBinaryTree();
		static bool SmallerAdaptiveHuffmanTree(const AdaptiveHuffmanBinaryTree* aht1, const AdaptiveHuffmanBinaryTree* aht2) 
		{
			return ((aht1->weight)<(aht2->weight));	
		};
		int weight;
		int num;//to indicate code of current branch
		string code;// to indicate code from top to this branch
		string chars_inside;
		AdaptiveHuffmanBinaryTree* ancester;
		AdaptiveHuffmanBinaryTree *branches[2];
	};
	HuffmanBinaryTree HuffmanTreeRoot;

	
	class LeafNode{
	public:
		char character;
		string code;
		LeafNode();

	};
	//leafnodes is used for quick search the char can be directly used as its index for lookup.
	vector<LeafNode> leafnodes;
	bool GenerateShanonFanoBranches(const vector<DistributionTable>& cpd, BinaryTree* branch);
	static bool GreaterProb(const DistributionTable& dt1, const DistributionTable& dt2);
	static bool SmallerProb(const DistributionTable& dt1, const DistributionTable& dt2);
	static bool MatchHuffmanTree(const HuffmanBinaryTree& ht1, const HuffmanBinaryTree& ht2);
	int getPositionInsertingHuffmantreeVector(const HuffmanBinaryTree& ht, const vector<HuffmanBinaryTree> htvector);
	void UpdateHuffmanTreeCode(const string& newchar, const HuffmanBinaryTree *ht);
public:
	void SetString(string str);
	void ComputeNumOfOccurences();
	void ComputeFrequencies();
	void ComputeEntropy();
	void BuildShanonFanoTree();
	void Encode(string outputfilename);// can be used for static huffman encoding and shanonfano encoding
	void BuildHuffmanTree();

	void BasicRunLengthEncoding(string inputfilename,string outputfilename);
	void BasicRunLengthDecoding(string inputfilename,string outputfilename);
	void ModifiedRunLengthEncoding(string inputfilename, string outputfilename);
	void ModifiedRunLengthDecoding(string inputfilename, string outputfilename);
	void GolombRiceEncoding(string outputfilename);
	void GolombRiceEncoding(string inputfilename, string outputfilename);
	void GolombRiceDecoding(string inputfilename, string outputfilename);
	string HuffmanEncodetoBitstring(string inputfilepath);
	string RunLengthEncodetoBitstring(string inputfilepath);
	string GolombRiceEncodetoBitstring(string inputfilepath);
	Encoder();
	~Encoder();
};


