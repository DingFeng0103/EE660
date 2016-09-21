#include "Encoder.h"


Encoder::Encoder()
{
	num_of_times.resize(UCHAR_MAX+1);
	frequencies.resize(num_of_times.size());
}


Encoder::~Encoder()
{
}

Encoder::LeafNode::LeafNode(){
	character = 0;
	code = "";
};

Encoder::HuffmanBinaryTree::HuffmanBinaryTree()
{
	weight = 0;
	code = "";
	chars_inside.clear();
	branches[0] = NULL;
	branches[1] = NULL;
}

Encoder::AdaptiveHuffmanBinaryTree::AdaptiveHuffmanBinaryTree()
{
	ancester = NULL;
	weight = 0;
	code = "";
	chars_inside.clear();
	branches[0] = NULL;
	branches[1] = NULL;
}


void Encoder::SetString(string str)
{
	bufferstring = str;
	stringlength = str.size();
};
void Encoder::ComputeNumOfOccurences()
{
	bool newchar=true;
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
		if (true==newchar)
		{
			DistributionTable distrbutioncell;
			distrbutioncell.character = bufferstring[i];
			distrbutioncell.num_of_occurence++;
			char_prob_dist.push_back(distrbutioncell);

		}
		newchar = true;
	}

};

void Encoder::ComputeFrequencies()
{
	for (int i = 0; i <char_prob_dist.size(); i++){
		char_prob_dist[i].frequency = ((double)char_prob_dist[i].num_of_occurence) / stringlength;
	}

};

void Encoder::ComputeEntropy()
{
	entropy = 0;
	for (int i = 0; i < char_prob_dist.size(); i++){
		entropy -= char_prob_dist[i].frequency * log2(char_prob_dist[i].frequency);
		
	}
	cout << entropy << endl;
};

bool Encoder::GreaterProb(const DistributionTable& dt1, const DistributionTable& dt2)
{
	return dt1.frequency>dt2.frequency;

};

bool Encoder::SmallerProb(const DistributionTable& dt1, const DistributionTable& dt2)
{
	return dt1.frequency<dt2.frequency;

};


void Encoder::BuildShanonFanoTree()
{
	leafnodes.clear();
	leafnodes.resize(MAX_BYTE_SIZE);
	//find all valid characters, that is the frequency
	sort(char_prob_dist.begin(), char_prob_dist.end(),Encoder::GreaterProb);
	//use -1 to indicate that it is a root, a root doesn't containa code;
	shanonfano_tree_root.code.assign("");
	shanonfano_tree_root.frequencysum = 1;
	shanonfano_tree_root.chars_prob_dist_of_this_branch = char_prob_dist;
	GenerateShanonFanoBranches(shanonfano_tree_root.chars_prob_dist_of_this_branch, &shanonfano_tree_root);

};

bool Encoder::GenerateShanonFanoBranches(const vector<DistributionTable>& cpd, BinaryTree* branch)
{
	BinaryTree *leftbranch=NULL;
	BinaryTree *rightbranch=NULL;

	if (cpd.size() == 1){
		if (branch == &shanonfano_tree_root){
			shanonfano_tree_root.code = '0';

		}
		BinaryTree leftbranch;
		branch->branches[0] = NULL;
		branch->branches[1] = NULL;

		leafnodes[(unsigned char)(cpd[0].character)].character = cpd[0].character;
		leafnodes[(unsigned char)(cpd[0].character)].code = branch->code;

		return true;
	}
	else{
		double tempfrequenciessum = 0;
		for (int i = 0; i < cpd.size(); i++){
			tempfrequenciessum += cpd[i].frequency;
			if (tempfrequenciessum >= branch->frequencysum / 2){
				leftbranch = new BinaryTree;
				rightbranch = new BinaryTree;
				branch->branches[0] = leftbranch;
				branch->branches[1] = rightbranch;

				leftbranch->code.assign(branch->code);
				leftbranch->code.push_back('0');
				leftbranch->frequencysum = tempfrequenciessum;

				leftbranch->chars_prob_dist_of_this_branch.assign(cpd.begin(), cpd.begin() + i + 1);
				GenerateShanonFanoBranches(leftbranch->chars_prob_dist_of_this_branch, branch->branches[0]);

				rightbranch->code.assign(branch->code);
				rightbranch->code.push_back('1');
				rightbranch->frequencysum = branch->frequencysum - tempfrequenciessum;

				rightbranch->chars_prob_dist_of_this_branch.assign(cpd.begin() + i + 1, cpd.end());
				GenerateShanonFanoBranches(rightbranch->chars_prob_dist_of_this_branch, branch->branches[1]);

				break;
			}
		}
	}
	return true;
};

void Encoder::Encode(string outputfilename)
{
	ofstream outputfile(outputfilename);
	string outputstringbuffer;
	string outputstring;
	for (int i = 0; i < bufferstring.size(); i++){
		outputstringbuffer.append(leafnodes[(unsigned char)bufferstring.at(i)].code);
	}
	char tempbyte=0;
	int j = 0;
	for (int i = 0; i < outputstringbuffer.size(); i++){
		tempbyte <<=1;
		tempbyte|= outputstringbuffer[i] - '0';
		if ((i + 1) % 8 == 0 || i == outputstringbuffer.size() - 1){
			outputstring.push_back(tempbyte);
			tempbyte = 0;
			j = i+1;

		}

	}
	outputfile << outputstring; 
	outputfile.close();

};

int Encoder::getPositionInsertingHuffmantreeVector(const HuffmanBinaryTree& ht, const vector<HuffmanBinaryTree> hfvector){
	//hf for huffman tree, hfvector for huffmantree vector.
	if (hfvector.size() == 0)
		return 0;
	for (int i= hfvector.size()-1; i>=0 ; i--){
		if (ht.weight<hfvector[i].weight)
			return i+1;

	}
	return 0;
};
void Encoder::BuildHuffmanTree()
{
	leafnodes.clear();
	leafnodes.resize(MAX_BYTE_SIZE);
	sort(char_prob_dist.begin(), char_prob_dist.end(), Encoder::GreaterProb);
	int subtree = char_prob_dist.size();
	HuffmanBinaryTree *leftbranch;
	HuffmanBinaryTree *rightbranch;
	vector<HuffmanBinaryTree> huffman_tree_vector;
	HuffmanBinaryTree *temphuffmantreeptr;
	for (int i = 0; i < char_prob_dist.size();i++)
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

		huffman_tree_vector.insert(huffman_tree_vector.begin()+getPositionInsertingHuffmantreeVector(*temphuffmantreeptr, huffman_tree_vector), *temphuffmantreeptr);
		subtree--;

	}
	HuffmanTreeRoot =huffman_tree_vector[0];

};

void Encoder::UpdateHuffmanTreeCode(const string& newc, const HuffmanBinaryTree *ht)
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
		leafnodes[(unsigned char)ht->chars_inside.back()].code=ht->code;
	}
};




void Encoder::AdaptiveHuffmanEncoding(string sourcefilename, string destinationfilename)
{

	ifstream inputfilestream(sourcefilename,ifstream::binary);
	ofstream outputfilestream(destinationfilename,ifstream::binary);
	vector<AdaptiveHuffmanBinaryTree*>  huffman_tree_vector;
	AdaptiveHuffmanBinaryTree adaptive_huffmantreeRoot_root;
	AdaptiveHuffmanBinaryTree *initialeof_huffmantree = NULL;
	AdaptiveHuffmanBinaryTree* new_huffmantree = NULL;
	AdaptiveHuffmanBinaryTree* temp_huffmantree = NULL;
	AdaptiveHuffmanBinaryTree* father_huffmantree = NULL;
	AdaptiveHuffmanBinaryTree* incomingcharaceter = NULL;// This tree pointer points to the incoming character
	int		index = 0;			//index point to an already exist character.
	int		indexofswapped = 0;	//index point to a node to be swapped with temp_huffmantree.
	int		indexofswap=0;		//index point to a node to swap with swapped.
	int		tempnum=0;			//this variable is used for swap their num.
	string	buffer;				//this buffer is used for output
	string	charactercode="";		//this string is used for fetching the code of this character.
	char tempbyte;				//this char used for output
	char tempchar=0;					//this char is used to help convert char to binary. so that newchar will not change.
	int i = 0, j = 0;			//used for all for-loop

	if (inputfilestream.is_open())
	{

		if (inputfilestream.eof()){
			cout << "This is an empty file" << endl;
			return;
		}
		// Initialize 
		initialeof_huffmantree = new AdaptiveHuffmanBinaryTree();
		initialeof_huffmantree->code = "0";
		initialeof_huffmantree->num = 0;
		initialeof_huffmantree->weight = 0;
		initialeof_huffmantree->ancester = &adaptive_huffmantreeRoot_root;
		huffman_tree_vector.push_back(initialeof_huffmantree);

		char newchar=0;
		inputfilestream.get(newchar);

		new_huffmantree = new AdaptiveHuffmanBinaryTree();
		new_huffmantree->chars_inside.push_back(newchar);
		new_huffmantree->weight++;
		new_huffmantree->code = "1";
		new_huffmantree->num = 1;
		new_huffmantree->ancester = &adaptive_huffmantreeRoot_root;
		huffman_tree_vector.push_back(new_huffmantree);

		adaptive_huffmantreeRoot_root.ancester = NULL;
		adaptive_huffmantreeRoot_root.weight = 1;
		adaptive_huffmantreeRoot_root.branches[0] = initialeof_huffmantree;
		adaptive_huffmantreeRoot_root.branches[1] = new_huffmantree;
		//1.output initial path
		buffer = "1";
		//2.output initial character in binary
		charactercode.clear();
		tempchar = newchar;
		for (i = 0; i < 8; i++){
			charactercode.insert(0, 1, '0' + (tempchar & 1));
			tempchar >>= 1;
		}
		buffer.append(charactercode);

		while (!inputfilestream.eof())
		{

			inputfilestream.get(newchar);

			for (index = 0; index < huffman_tree_vector.size(); index++){
				if (huffman_tree_vector[index]->chars_inside.size() == 1 && huffman_tree_vector[index]->chars_inside[0] == newchar)
					break;
			}
			if (index == huffman_tree_vector.size())//no such character exist, need to build a new node
			{
				//output first then update the tree
				//1. output its path
				//search from initial_tree which is also called NYT node.
				charactercode.clear();
				temp_huffmantree = initialeof_huffmantree;
				while (temp_huffmantree != &adaptive_huffmantreeRoot_root){
					charactercode = temp_huffmantree->code + charactercode;
					temp_huffmantree = temp_huffmantree->ancester;
				}
				buffer.append(charactercode);
				//2. output char in binary
				charactercode.clear();
				tempchar = newchar;
				for (i = 0; i < 8; i++){
					charactercode.insert(0, 1, '0' + (tempchar & 1));
					tempchar >>= 1;
				}
				buffer.append(charactercode);

				father_huffmantree = initialeof_huffmantree;

				new_huffmantree = new AdaptiveHuffmanBinaryTree();
				new_huffmantree->chars_inside.push_back(newchar);
				new_huffmantree->weight++;
				new_huffmantree->code = "1";
				new_huffmantree->num = 1;
				new_huffmantree->ancester = father_huffmantree;
				incomingcharaceter = new_huffmantree;
				

				initialeof_huffmantree = new AdaptiveHuffmanBinaryTree();
				initialeof_huffmantree->code="0";
				initialeof_huffmantree->weight = 0;
				initialeof_huffmantree->num = 0;
				initialeof_huffmantree->ancester = father_huffmantree;
				father_huffmantree->branches[0] = initialeof_huffmantree;
				father_huffmantree->branches[1] = new_huffmantree;
				father_huffmantree->weight++;

				huffman_tree_vector.insert(huffman_tree_vector.begin(), new_huffmantree);
				huffman_tree_vector.insert(huffman_tree_vector.begin(), initialeof_huffmantree);
				//this father huffmantree's weight will always be 1, so there is no swap. Directly go to nest ancester
	//			father_huffmantree = father_huffmantree->ancester;
		//		father_huffmantree->weight++;

				
			}
			else
			{ 
				// if it already exist output path first then update the tree.
				charactercode.clear();
				temp_huffmantree = huffman_tree_vector[index];
				while (temp_huffmantree != &adaptive_huffmantreeRoot_root){
					charactercode = temp_huffmantree->code + charactercode;
					temp_huffmantree = temp_huffmantree->ancester;
				}
				buffer.append(charactercode);
				
				// increase the weight of this node and update tree
				huffman_tree_vector[index]->weight++;
				father_huffmantree = huffman_tree_vector[index];
				incomingcharaceter = huffman_tree_vector[index];
			}
			//find the right up node and swap with temp_huffmantree 
			while (father_huffmantree != &adaptive_huffmantreeRoot_root){
				for (i = 0; i < huffman_tree_vector.size(); i++)
				{
					if (huffman_tree_vector[i] == father_huffmantree)
					{
						indexofswap = i;
						for (j = i+1; j < huffman_tree_vector.size(); j++)
						{
							if (huffman_tree_vector[j]->weight < father_huffmantree->weight&&father_huffmantree->ancester != huffman_tree_vector[j]){
								indexofswapped = j;
							}
						}
						break;
					}
				}
				if (0 == indexofswapped)
				{
					father_huffmantree = father_huffmantree->ancester;
					father_huffmantree->weight++;
				}
				else
				{
					//swap their pointer in huffmantree
				//	change of ancesster will automatically happen when changing their point value.
					huffman_tree_vector[indexofswap]->ancester->branches[huffman_tree_vector[indexofswap]->num] = huffman_tree_vector[indexofswapped];
					huffman_tree_vector[indexofswapped]->ancester->branches[huffman_tree_vector[indexofswapped]->num] = huffman_tree_vector[indexofswap];

					temp_huffmantree = huffman_tree_vector[indexofswapped]->ancester;
					huffman_tree_vector[indexofswapped]->ancester = huffman_tree_vector[indexofswap]->ancester;
					huffman_tree_vector[indexofswap]->ancester = temp_huffmantree;

					//swap their pointer in huffman_tree_vector, but num and first code remains still.
					temp_huffmantree = huffman_tree_vector[indexofswapped];
					tempnum = huffman_tree_vector[indexofswapped]->num;
					huffman_tree_vector[indexofswapped] = huffman_tree_vector[indexofswap];					
					temp_huffmantree->num = huffman_tree_vector[indexofswap]->num;
					huffman_tree_vector[indexofswap] = temp_huffmantree;
					huffman_tree_vector[indexofswapped]->num = tempnum;
					huffman_tree_vector[indexofswap]->code.front() = (char)huffman_tree_vector[indexofswap]->num + 48;
					huffman_tree_vector[indexofswapped]->code.front() = (char)huffman_tree_vector[indexofswapped]->num + 48;

					father_huffmantree = huffman_tree_vector[indexofswapped]->ancester;
					father_huffmantree->weight++;
					//reset this to initial values;
					indexofswap = 0;
					indexofswapped = 0;
					tempnum = 0;
				}
			};
			
			while(buffer.size() >= 8){
				tempbyte = 0;

				for (int i = 0; i <8; i++){
					tempbyte <<= 1;
					tempbyte |= buffer[i] - '0';

				}
				buffer.erase(0, 8);
				outputfilestream.put(tempbyte);
			}
		}
		outputfilestream.close();
		inputfilestream.close();
	}
	delete adaptive_huffmantreeRoot_root.branches[0];
	delete adaptive_huffmantreeRoot_root.branches[1];
};



void Encoder::BasicRunLengthEncoding(string inputfilename, string outputfilename)
{
	ifstream inputfile(inputfilename, ifstream::binary);
	ofstream outputfile(outputfilename, ofstream::binary);

	char lastchar;
	char thischar;
	unsigned char count;
	if (inputfile.is_open()){
		inputfile.get(lastchar);
		count = 1;
		while (inputfile.get(thischar)){
			if (inputfile.eof()){
				cout << "Error: has reached the end of file, should continue any more." << endl;
			}
			else if (thischar == lastchar&&count < 255){// the maximum of a char counter can only be 255
														// when it reaches 255 it should output
				count++;
			}
			else{
				outputfile.put(count);
				outputfile.put(lastchar);
				lastchar = thischar;
				count = 1;
			}

		}
		outputfile.put(count);
		outputfile.put(lastchar);
	}

	inputfile.close();
	outputfile.close();

};

void Encoder::BasicRunLengthDecoding(string inputfilename, string outputfilename)
{
	ifstream inputfile(inputfilename, ifstream::binary);
	ofstream outputfile(outputfilename, ofstream::binary);

	string repetingstring;			
	char j;		//To record repeting times
	char tempchar;	//To store the repeting character.
	if (inputfile.is_open()){
		while (inputfile.get(j)){
			inputfile.get(tempchar);
			repetingstring.assign((unsigned char)j, tempchar);
			outputfile<<(repetingstring);
		}
	}

	inputfile.close();
	outputfile.close();
};

void Encoder::ModifiedRunLengthEncoding(string inputfilename, string outputfilename)
{
	ifstream inputfile(inputfilename, ifstream::binary);
	ofstream outputfile(outputfilename, ofstream::binary);

	char lastchar;	
	char thischar;
	unsigned char count;
	if (inputfile.is_open()){
		inputfile.get(lastchar);
		count = 1;
		while (inputfile.get(thischar)){
			if (inputfile.eof()){
				cout << "Error: has reached the end of file, should continue any more." << endl;
			}
			if (thischar == lastchar&&count <0x7f){
				count++;
			}
			else{
				if (1 == count){
					if ((unsigned char)lastchar >0x80){
						outputfile.put((count|0x80));
						outputfile.put(lastchar);						
					}
					else{
						outputfile.put(lastchar);
					}
					lastchar = thischar;
					count = 1;
				}
				else{
					outputfile.put(count|0x80);
					outputfile.put(lastchar);
					lastchar = thischar;
					count = 1;
				}
			}			
		}
		//handling the last character
		if (1 == count){
			if ((unsigned char)lastchar >0x80){
				outputfile.put((count | 0x80));
				outputfile.put(lastchar);
			}
			else{
				outputfile.put(lastchar);
			}

		}
		else{
			outputfile.put(count | 0x80);
			outputfile.put(lastchar);
		}
	}

	inputfile.close();
	outputfile.close();

};

void Encoder::ModifiedRunLengthDecoding(string inputfilename, string outputfilename)
{	
	ifstream inputfile(inputfilename, ifstream::binary);
	ofstream outputfile(outputfilename, ofstream::binary);

	string repetingstring;
	char j;		//To record repeting times
	char tempchar;	//To store the repeting character.
	if (inputfile.is_open()){
		while (inputfile.get(j)){
			if ((unsigned char)j >0x80){
				inputfile.get(tempchar);
				repetingstring.assign((unsigned char)j-0x80, tempchar);
				outputfile << (repetingstring);
			}
			else{
				repetingstring.push_back(j);
				outputfile << (repetingstring);
			}
			repetingstring.clear();
		}
	}

	inputfile.close();
	outputfile.close();
	
};

void Encoder::GolombRiceEncoding(string outputfilename)
{
	ofstream outputfile(outputfilename, ofstream::binary);

	//bufferstring is in Encoder
	int M = 16;
	int sizeofremainder = 4;
	unsigned char mask = 1 << (sizeofremainder - 1);
	int q = 0;				//quotient
	int r = 0;				//remainder
	int i = 0;				//index i is used to access bufferstring
	int j = 0;				//index j is used to convert binary to char so that it can be stored in buffer
	string buffer;			//to store bits if there is more than 8, output
	char tempbyte=0;
	for (i = 0; i < bufferstring.size(); i++)
	{
		q = (unsigned char)bufferstring[i] / M;
		r = (unsigned char)bufferstring[i] % M;
		//add quotient to the code
		while (q > 0){
			buffer.push_back('0');
			q--;
		}
		//add a '1' to seperate quotient and remainder
		buffer.push_back('1');
		//add remaider to the code
		for (j = 0; j < sizeofremainder; j++){
			if ((r&mask)== 0)
				buffer.push_back('0');
			else
				buffer.push_back('1');
			mask >>= 1;
		}
		mask = 1 << (sizeofremainder - 1);

		//output buffer if it is more then 8
		while (buffer.size() >= 8){
			tempbyte = 0;

			for (int i = 0; i <8; i++){
				tempbyte <<= 1;
				tempbyte |= buffer[i] - '0';

			}
			buffer.erase(0, 8);
			outputfile.put(tempbyte);
		}
	}
	outputfile.close();
};
//overwrite to encoding another file except that has already been set.
void Encoder::GolombRiceEncoding(string inputfilename, string outputfilename)
{
	ofstream outputfile(outputfilename, ofstream::binary);
	ifstream inputfile(inputfilename, ifstream::binary);
	//bufferstring is in Encoder
	int M = 16;
	int sizeofremainder = 4;	//since M is 16, m is 4.
	unsigned char mask = 1 << (sizeofremainder - 1);
	int q = 0;				//quotient
	int r = 0;				//remainder
	int j = 0;				//index j is used to convert binary to char so that it can be stored in buffer
	string buffer;			//to store bits if there is more than 8, output
	char tempchar;			//used to store the input char
	char tempbyte = 0;		// used for output 

	//for debug only
	int count = 0;
	while (!inputfile.eof())
	{

		inputfile.get(tempchar);
		q = (unsigned char)tempchar / M;
		r = (unsigned char)tempchar % M;
		//add quotient to the code
		while (q > 0){
			buffer.push_back('0');
			q--;
		}
		//add a '1' to seperate quotient and remainder
		buffer.push_back('1');
		//add remaider to the code
		for (j = 0; j < sizeofremainder; j++){
			if ((r&mask) == 0)
				buffer.push_back('0');
			else
				buffer.push_back('1');
			mask >>= 1;
		}
		mask = 1 << (sizeofremainder - 1);

		//output buffer if it is more then 8

		//for debug only
		count++;
		if (count == 1251)
			count = count;
		while (buffer.size() >= 8){
			tempbyte = 0;

			for (int i = 0; i <8; i++){
				tempbyte <<= 1;
				tempbyte |= buffer[i] - '0';

			}
			buffer.erase(0, 8);
			outputfile.put(tempbyte);
		}
	}

	//deal with code that is left in the buffer
	// fill the end with zeros
	while(8>buffer.size())
	{
		buffer.push_back('0');
	}
	// output the last word.
	tempbyte = 0;

	for (int i = 0; i <8; i++){
		tempbyte <<= 1;
		tempbyte |= buffer[i] - '0';

	}
	buffer.erase(0, 8);
	outputfile.put(tempbyte);


};

void Encoder::GolombRiceDecoding(string inputfilename, string outputfilename)
{
	ifstream inputfile(inputfilename, ifstream::binary);
	ofstream outputfile(outputfilename, ofstream::binary);
	
	char tempchar = 0;			//used to store input file
	char tempbyte = 0;			//used to output.
	int j = 0;
	int M = 16;
	int sizeofremainder = 4;	//since M is 16, m is 4.
	unsigned char mask = 1 << (sizeof(char)*8 - 1);
	string buffer;
	while (!inputfile.eof())
	{
		inputfile.get(tempchar);
		for (j = 0; j < sizeof(char) * 8; j++){
			if ((tempchar&mask) == 0)
				buffer.push_back('0');
			else
				buffer.push_back('1');
			mask >>= 1;
		}
		mask = 1 << (sizeof(char) * 8 - 1);

		for (int i = 0; i < buffer.size(); i++){
			if (buffer[i] == '1' && (i + 4) < buffer.size())
			{				
				tempbyte = (char)i << sizeofremainder;
				for (j = 1; j <= sizeofremainder; j++)
				{
					tempbyte = tempbyte | ((buffer[i + j] - '0') << sizeofremainder - j);
				}
				outputfile.put(tempbyte);
				tempbyte = 0;
				buffer.erase(0, i + sizeofremainder+1);
				i = -1;
			}
		}
	};
};
