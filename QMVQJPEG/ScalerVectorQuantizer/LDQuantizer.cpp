#include "LDQuantizer.h"

LDQuantizer::LDQuantizer()
{
}


LDQuantizer::~LDQuantizer()
{
}

void LDQuantizer::Initialize(int n, int instruction, int d)
// n is the number of codewords;
//d is the dimension of codewords;
//instruction denote to do scaler quantize or vector quantize.
{
	if (instruction == Scaler&&d != 1){
		cout << "Error: for scaler quantize, dimension can't be more than 1" << endl;
	}
	else if (instruction == Vector&&d == 1){
		Initialize(n, instruction);
	}
	else if (instruction == Vector&&d!=1){
		codebook.clear();

		codebook.resize(n);
	}
}

void LDQuantizer::Initialize(int n, int instruction)
{
	this->n = n;
	if (instruction != Scaler){
		cout << "Error: Not enough arguments, please input the dimension of vectors" << endl;		
	}
	else{
		this->codebook.clear();
		this->dividers.clear();
		vector<double> *codeword;
		double divider;
		int i = 0;
		unsigned char temp;
		for (i = 0; i < n; i++){
			temp = (double)(i + i + 1) / 2 * ((MaxSymbol + 1) / n);
			codeword = new vector < double > ;
			codeword->push_back(temp);
			this->codebook.push_back(*codeword);
			delete codeword;
			divider = i*(MaxSymbol + 1) / n ;
			this->dividers.push_back(divider);
		}
		divider = i*(MaxSymbol + 1) / n ;
		this->dividers.push_back(divider);
		this->num_symbols = 0;
		this->distribution.clear();
		this->distribution.assign(256, 0);
		this->frequencies.clear();
		this->frequencies.assign(256, 0);

	}
}

void LDQuantizer::Histogram(string str)
{

	this->num_symbols+= str.size();
	for (int i = 0; i < str.size(); i++){
		this->frequencies[(unsigned char)str[i]]++;
	}
	for (int i = 0; i < distribution.size(); i++){
		this->distribution[i] = frequencies[i] / (this->num_symbols);
	}

}
void LDQuantizer::ScalerTraining(string sourcedir)
{

	FileManager file(sourcedir, Read);
	string str=file.ReadStrings();
	cout << "Training: " << sourcedir <<" for "<<log2(this->n)<< " bit quantizer "<<"......."<< endl;

	this->Histogram(str);

	int j;
	double epre = 0;
	double enow = 0;
	for (int i = 0; i < codebook.size(); i++){
		for (int j = 0; j <= MaxSymbol; j++){
			if (j >= dividers[i] && j <= dividers[i + 1]){
				enow +=this->distribution[j] * (pow(j - codebook[i][0], 2));
			}
		}
	}
	//Update reconstructino level
	int count = 0;
	double weightedSum = 0;
	double Sum = 0;
	do{
		epre = enow;
		enow = 0;

		for (int i = 0; i < codebook.size(); i++){
			for (int j = 0; j <= MaxSymbol; j++){
				if (j >= dividers[i] && j < dividers[i + 1]){
					weightedSum += j*(this->distribution[j]);
					Sum += (this->distribution[j]);
				}
			}

			if (Sum != 0){
				codebook[i][0] = weightedSum / Sum;
			}
			weightedSum = 0;
			Sum = 0;
		}
		//Update decision level
		for (int i = 1; i < dividers.size() - 1; i++){
			dividers[i] = (codebook[i - 1][0] + codebook[i][0]) / 2;
		}
		for (int i = 0; i < codebook.size(); i++){
			for (int j = 0; j <= MaxSymbol; j++){
				if (j >= dividers[i] && j <= dividers[i + 1]){
					enow += (this->distribution[j]) * (pow(j - codebook[i][0], 2));
				}
			}
		}
		count++;
		cout <<"In this iteration PSNR:" <<this->ComputePSNR() << endl;
	}while (((epre-enow)/enow>Tolerence) && (count < 100));
	cout << "Converges after " << count << "iteratoins" << endl;
}
double LDQuantizer::ComputePSNR()
{
	double mse=0;
	for (int i = 0; i < codebook.size(); i++){
		for (int j = 0; j <= MaxSymbol; j++){
			if (j >= dividers[i] && j <= dividers[i + 1]){
				mse += (this->distribution[j]) * (pow(j - codebook[i][0], 2));
			}
		}
	}
	return 10 * log10((double)(pow(MaxSymbol, 2)) / mse);
}
double LDQuantizer::ComputePSNR(string str)
{
	double mse = 0;
	for (int i = 0; i < codebook.size(); i++){
		for (int j = 0; j < str.size(); j++){
			if ((unsigned char)str[j] >= dividers[i] && (unsigned char)str[j] <= dividers[i + 1]){
				mse += (pow((unsigned char)str[j] - codebook[i][0], 2));
			}
		}
	}
	mse = mse / str.size();
	return 10 * log10((double)(pow(MaxSymbol,2)) / mse);
}

double LDQuantizer::ComputeEntropy(string str)
{
	vector<double> nums;
	vector<double> dists;
	double entropy=0;
	int l = str.size();
	nums.assign(256, 0);
	dists.clear();
	dists.resize(256);
	for (int i = 0; i < str.size(); i++){
		nums[(unsigned char)str[i]]++;
	}
	for (int i = 0; i < dists.size(); i++){
		dists[i] = nums[i] / l;
	}
	for (int i = 0; i < dists.size(); i++){
		if (dists[i]>0){
			entropy += -dists[i] * log(dists[i]);
		}
	}
	return entropy;

}

double LDQuantizer::ComputeEntropy(string dir,int n)
{
	FileManager f;
	f.OpenFile(dir, Read);
	string str = f.ReadStrings();
	vector<double> nums;
	vector<double> dists;
	double entropy = 0;
	int l = str.size();
	nums.assign(256, 0);
	dists.clear();
	dists.resize(256);
	for (int i = 0; i < str.size(); i++){
		nums[(unsigned char)str[i]]++;
	}
	for (int i = 0; i < dists.size(); i++){
		dists[i] = nums[i] / l;
	}
	for (int i = 0; i < dists.size(); i++){
		if (dists[i]>0){
			entropy += -dists[i] * log(dists[i]);
		}
	}
	cout << n << " bits file " << dir << " has entropy " << entropy <<endl;
	return entropy;

}
void LDQuantizer::ScalerQuantize(string sourcedir,string destination)
{
	FileManager inputfile(sourcedir, Read);
	FileManager outputfile(destination, Write);
	string str = inputfile.ReadStrings();
	cout << log2(this->n) << "-bits quantize: " << sourcedir << "has PSNR " << this->ComputePSNR(str) << endl;
	int j = 0;
	int nbits=log2(n);
	int tempn = this->n;

	for (int i = 0; i < str.size(); i++){
		for (j = 1; j < dividers.size(); j++){
			if ((unsigned char)str[i] <= dividers[j]){
				outputfile.WriteNBits((unsigned char)codebook[j - 1][0], 8);
				break;
			}
		}
	}
}

void LDQuantizer::ScalerQuantizeEncode(string sourcedir, string destination)
{
	FileManager inputfile(sourcedir, Read);
	FileManager outputfile(destination, Write);
	string str = inputfile.ReadStrings();
	int j = 0;
	int nbits = log2(n);
	int tempn = this->n;

	for (int i = 0; i < str.size(); i++){
		for (j = 1; j < dividers.size(); j++){
			if ((unsigned char)str[i] <= dividers[j]){
				outputfile.WriteNBits(j - 1, nbits);
				break;
			}
		}
	}
}
vector<vector<double>> LDQuantizer::LBGSplitInitial(string str)
{
	//compute mean
	double mean1= 0;
	double mean2 = 0;

	double sum1 = 0;
	double sum2 = 0;
	int l = str.size()/2;
	for (int i = 0; i < str.size(); i+=2){
		sum1 += (unsigned char)str[i];
	}
	for (int i = 1; i < str.size(); i += 2){
		sum2 += (unsigned char)str[i];
	}
	mean1 = sum1 / l;
	mean2 = sum2 / l;

	double sd1 = 0;
	double sd2 = 0;
	double e1 = 0;
	double e2 = 0;
	for (int i = 0; i < str.size(); i+=2){
		e1 += pow(((unsigned char)str[i]-mean1),2);
	}
	for (int i = 1; i < str.size(); i += 2){
		e2 += pow(((unsigned char)str[i] - mean2), 2);
	}
	sd1 = sqrt(e1 / l);
	sd2 = sqrt(e2 / l);
	vector<double> *codeword = new vector<double>;
	vector<vector<double>> centroids;
	codeword->push_back(mean1);
	codeword->push_back(mean2);
	centroids.push_back(*codeword);
	codeword = new vector < double >;
	codeword->push_back(mean1+sd1);
	codeword->push_back(mean2+sd2);
	centroids.push_back(*codeword);
	return centroids;
}

void LDQuantizer::VectorQuantize(string str)
{
	vector<vector<double>> centroids = this->LBGSplitInitial(str);
	vector<vector<double>> words;
	vector<double> * word;
	vector<int> assignments;

	for (int i = 0; i < str.size(); i+=2){
		word = new vector < double >;
		word->push_back((unsigned char)str[i]);
		word->push_back((unsigned char)str[i + 1]);
		words.push_back(*word);		
		assignments.push_back(this->AssignCentroid(*word, centroids));
	}

	
	for (int i = 0; i <= 3; i++){
		this->UpdateCentroids(words, centroids, assignments);
		for (int j = 0; j < words.size(); j++){
			assignments[j] = this->AssignCentroid(words[j], centroids);
		}
	}
	while (centroids.size() < 8){
		LBGSplit(words, centroids, assignments);
		for (int j = 0; j < words.size(); j++){
			assignments[j] = this->AssignCentroid(words[j], centroids);
		}
		for (int i = 0; i <= 3; i++){
			this->UpdateCentroids(words, centroids, assignments);
			for (int j = 0; j < words.size(); j++){
				assignments[j] = this->AssignCentroid(words[j], centroids);
			}
		}
	}

	//Print out the final code words
	cout << "Final code words are:" << endl;
	for (int i = 0; i < centroids.size(); i++){
		cout << "(" << centroids[i][0] << "," << centroids[i][1] << ")" << endl;
	}

}
int  LDQuantizer::AssignCentroid(vector<double> word, vector<vector<double>> centroids)
{
	int index = 0;
	int d = this->distance(word, centroids[0]);
	for (int i = 1; i < centroids.size(); i++){
		if (distance(word, centroids[i]) < d)
			index = i;
	}
	return index;
}

double LDQuantizer::distance(vector<double> word, vector<double> centroid)
{
	double d = 0;
	for (int i = 0; i < word.size(); i++){
		d+=pow((word[i] - centroid[i]),2);
	}

	return sqrt(d);
}

void LDQuantizer::UpdateCentroids(vector<vector<double>> words, vector<vector<double>> &centroids, vector<int> &assignments)
{

	vector<vector<double>> sums(centroids.size(),vector<double> (2,0));
	vector<double > counts(centroids.size(),0);

	for (int i = 0; i < words.size(); i++){
		for (int j = 0; j < centroids.size(); j++){
			if (assignments[i] == j){
				sums[j][0] += words[i][0];
				sums[j][1] += words[i][1];
				counts[j]++;
			}
		}
	}
	for (int j = 0; j < centroids.size(); j++){
		if (counts[j] == 0 && sums[j][0] == 0 && sums[j][1]==0){
			continue;
		}
		else if (counts[j] == 0 && ((sums[j][0] != 0) || (sums[j][1] != 0))){
			perror("divide by zero");
			continue;
		}
		else{
			centroids[j][0] = sums[j][0] / counts[j];
			centroids[j][1] = sums[j][1] / counts[j];
		}

	}
}

void LDQuantizer::LBGSplitCodebook(vector<vector<double>> words, vector<vector<double>> &centroids)
{
	unsigned char mean1 = 0;
	unsigned char mean2 = 0;

	unsigned int sum1 = 0;
	unsigned int sum2 = 0;
	int l = words.size();
	for (int i = 0; i < words.size(); i++){
		sum1 += words[i][0];
		sum2 += words[i][1];
	}
	mean1 = sum1 / l;
	mean2 = sum2 / l;

	unsigned char sd1 = 0;
	unsigned char sd2 = 0;
	unsigned int e1 = 0;
	unsigned int e2 = 0;
	for (int i = 0; i < words.size(); i++){
		e1 += pow((words[i][0] - mean1), 2);
		e2 += pow((words[i][1] - mean2), 2);
	}

	sd1 = sqrt(e1 / l);
	sd2 = sqrt(e2 / l);
	vector<double> *codeword = new vector<double>;
	codeword->push_back(mean1);
	codeword->push_back(mean2);
	centroids.push_back(*codeword);
	codeword = new vector < double>;
	codeword->push_back(mean1 + sd1);
	codeword->push_back(mean2 + sd2);
	centroids.push_back(*codeword);
}

void LDQuantizer::LBGSplit(vector<vector<double>> words, vector<vector<double>> &centroids, vector<int> &assignments)
{
	vector<vector<double>> *tempwords = new vector < vector<double> >;
	
	int numcells = centroids.size();
	centroids.clear();
	for (int j = 0; j < numcells; j++){
		for (int i = 0; i < words.size(); i++){
			if (assignments[i] == j){
				tempwords->push_back(words[i]);
			}
		}
		LBGSplitCodebook(*tempwords, centroids);
		tempwords->clear();
	}

}