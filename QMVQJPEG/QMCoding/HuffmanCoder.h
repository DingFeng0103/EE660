// This class is used as a tool class, it has the main function of encoding
#ifndef _HuffmanCoding_
#define _HuffmanCoding_
#include<string>
#include<vector>
#include<iostream>
#include<cmath>
#include<algorithm>
#include<functional>
#include<fstream>
#define MAX_BYTE_SIZE 256


using namespace std;
class HuffmanCoder
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
		int num_of_occurence = 0;
		double frequency;

	};
	vector<DistributionTable> char_prob_dist;

	class HuffmanBinaryTree{
	public:
		HuffmanBinaryTree();
		double weight;
		string code;
		string chars_inside;//store characters that belong to this tree.
		HuffmanBinaryTree *branches[2];

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
	void BuildHuffmanTree();
	void Encode(string outputfilename);// can be used for static huffman encoding and shanonfano encoding
	string HuffmanEncodetoBitstring(string inputfile);// can be used for static huffman encoding and shanonfano encoding

	HuffmanCoder();
	~HuffmanCoder();
};


#endif