#include "HuffmanCoder.h"

HuffmanCoder::HuffmanCoder()
{
	num_of_times.resize(UCHAR_MAX + 1);
	frequencies.resize(num_of_times.size());

}


HuffmanCoder::~HuffmanCoder()
{
}

HuffmanCoder::LeafNode::LeafNode(){
	character = 0;
	code = "";
};

HuffmanCoder::HuffmanBinaryTree::HuffmanBinaryTree()
{
	weight = 0;
	code = "";
	chars_inside.clear();
	branches[0] = NULL;
	branches[1] = NULL;
}



void HuffmanCoder::SetString(string str)
{
	bufferstring = str;
	stringlength = str.size();
};
void HuffmanCoder::ComputeNumOfOccurences()
{
	bool newchar = true;
	for (int i = 0; i <bufferstring.size(); i++){
		if ((unsigned char)bufferstring[i] > 255){
			cout << "Error: index out of range in num_of_times[]" << endl;
			cout << "the Maximum character must be \255" << endl;
		}
		for (int j = 0; j < char_prob_dist.size(); j++)
		{
			if (char_prob_dist[j].character == bufferstring[i]){
				char_prob_dist[j].num_of_occurence++;
				newchar = false;
			}
		}
		if (true == newchar)
		{
			DistributionTable distrbutioncell;
			distrbutioncell.character = bufferstring[i];
			distrbutioncell.num_of_occurence++;
			char_prob_dist.push_back(distrbutioncell);

		}
		newchar = true;
	}

};

void HuffmanCoder::ComputeFrequencies()
{
	for (int i = 0; i <char_prob_dist.size(); i++){
		char_prob_dist[i].frequency = ((double)char_prob_dist[i].num_of_occurence) / stringlength;
	}

};

void HuffmanCoder::ComputeEntropy()
{
	entropy = 0;
	for (int i = 0; i < char_prob_dist.size(); i++){
		entropy -= char_prob_dist[i].frequency * log2(char_prob_dist[i].frequency);

	}
};

bool HuffmanCoder::GreaterProb(const DistributionTable& dt1, const DistributionTable& dt2)
{
	return dt1.frequency>dt2.frequency;

};

bool HuffmanCoder::SmallerProb(const DistributionTable& dt1, const DistributionTable& dt2)
{
	return dt1.frequency<dt2.frequency;

};


void HuffmanCoder::Encode(string outputfilename)
{
	ofstream outputfile(outputfilename);
	string outputstringbuffer;
	string outputstring;
	for (int i = 0; i < bufferstring.size(); i++){
		outputstringbuffer.append(leafnodes[(unsigned char)bufferstring.at(i)].code);
	}
	char tempbyte = 0;
	int j = 0;
	for (int i = 0; i < outputstringbuffer.size(); i++){
		tempbyte <<= 1;
		tempbyte |= outputstringbuffer[i] - '0';
		if ((i + 1) % 8 == 0 || i == outputstringbuffer.size() - 1){
			outputstring.push_back(tempbyte);
			tempbyte = 0;
			j = i + 1;

		}

	}
	outputfile << outputstring;
	outputfile.close();

};

string HuffmanCoder::HuffmanEncodetoBitstring(string inputfilepath)
{
	ifstream inputfile(inputfilepath, ios::binary);
	string line;
	string filestring;
	if (inputfile.is_open()){
		while (getline(inputfile, line)){
			filestring.append(line);
			filestring.append("\n");
		}
	}
	SetString(filestring);

	filestring.clear();
	line.clear();

	ComputeNumOfOccurences();
	ComputeFrequencies();
	ComputeEntropy();
	BuildHuffmanTree();


	for (int i = 0; i < bufferstring.size(); i++){
		filestring.append(leafnodes[(unsigned char)bufferstring.at(i)].code);
	}
	return filestring;
};

int HuffmanCoder::getPositionInsertingHuffmantreeVector(const HuffmanBinaryTree& ht, const vector<HuffmanBinaryTree> hfvector){
	//hf for huffman tree, hfvector for huffmantree vector.
	if (hfvector.size() == 0)
		return 0;
	for (int i = hfvector.size() - 1; i >= 0; i--){
		if (ht.weight<hfvector[i].weight)
			return i + 1;

	}
	return 0;
};
void HuffmanCoder::BuildHuffmanTree()
{
	leafnodes.clear();
	leafnodes.resize(MAX_BYTE_SIZE);
	sort(char_prob_dist.begin(), char_prob_dist.end(), HuffmanCoder::GreaterProb);
	int subtree = char_prob_dist.size();
	HuffmanBinaryTree *leftbranch;
	HuffmanBinaryTree *rightbranch;
	vector<HuffmanBinaryTree> huffman_tree_vector;
	HuffmanBinaryTree *temphuffmantreeptr;
	for (int i = 0; i < char_prob_dist.size(); i++)
	{
		temphuffmantreeptr = new HuffmanBinaryTree();
		temphuffmantreeptr->code = "";
		temphuffmantreeptr->weight = char_prob_dist[i].frequency;
		temphuffmantreeptr->chars_inside.push_back(char_prob_dist[i].character);
		temphuffmantreeptr->branches[0] = NULL;
		temphuffmantreeptr->branches[1] = NULL;
		huffman_tree_vector.push_back(*temphuffmantreeptr);
	}
	while (subtree > 1){
		temphuffmantreeptr = new HuffmanBinaryTree();
		leftbranch = new HuffmanBinaryTree();
		rightbranch = new HuffmanBinaryTree();

		*leftbranch = huffman_tree_vector.back();
		leftbranch->code = "0";
		UpdateHuffmanTreeCode("0", leftbranch);
		huffman_tree_vector.pop_back();

		*rightbranch = huffman_tree_vector.back();
		rightbranch->code = "1";
		UpdateHuffmanTreeCode("1", rightbranch);
		huffman_tree_vector.pop_back();

		temphuffmantreeptr->branches[0] = leftbranch;
		temphuffmantreeptr->branches[1] = rightbranch;
		temphuffmantreeptr->chars_inside.append(leftbranch->chars_inside);
		temphuffmantreeptr->chars_inside.append(rightbranch->chars_inside);
		temphuffmantreeptr->weight = leftbranch->weight + rightbranch->weight;
		temphuffmantreeptr->code = "";

		huffman_tree_vector.insert(huffman_tree_vector.begin() + getPositionInsertingHuffmantreeVector(*temphuffmantreeptr, huffman_tree_vector), *temphuffmantreeptr);
		subtree--;

	}
	HuffmanTreeRoot = huffman_tree_vector[0];

};

void HuffmanCoder::UpdateHuffmanTreeCode(const string& newc, const HuffmanBinaryTree *ht)
{
	if (ht->branches[0] != NULL&&ht->branches[1] != NULL)
	{
		ht->branches[0]->code = newc + ht->branches[0]->code;
		UpdateHuffmanTreeCode(newc, ht->branches[0]);
		ht->branches[1]->code = newc + ht->branches[1]->code;
		UpdateHuffmanTreeCode(newc, ht->branches[1]);
	}
	else
	{
		leafnodes[(unsigned char)ht->chars_inside.back()].character = ht->chars_inside.back();
		leafnodes[(unsigned char)ht->chars_inside.back()].code = ht->code;
	}
};


