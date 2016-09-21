#include"LDQuantizer.h"

int main()
{
	LDQuantizer ldquantizer;
	
	int n = 3;
	ldquantizer.Initialize(pow(2, n), Scaler);
	ldquantizer.ScalerTraining("data/chem.256");
	ldquantizer.ScalerTraining("data/house.256");
	ldquantizer.ScalerTraining("data/moon.256");
	ldquantizer.ScalerQuantize("data/chem.256","Results/3chem.256");
	ldquantizer.ScalerQuantize("data/house.256", "Results/3house.256");
	ldquantizer.ScalerQuantize("data/moon.256","Results/3moon.256");
	ldquantizer.ScalerQuantize("data/couple.256", "Results/3couple.256");
	ldquantizer.ScalerQuantize("data/elaine.256", "Results/3elaine.256");
	ldquantizer.ScalerQuantize("data/f16.256", "Results/3f16.256");
	ldquantizer.ComputeEntropy("Results/3chem.256", 3);
	ldquantizer.ComputeEntropy("Results/3house.256", 3);
	ldquantizer.ComputeEntropy("Results/3moon.256", 3);
	ldquantizer.ComputeEntropy("Results/3couple.256", 3);
	ldquantizer.ComputeEntropy("Results/3elaine.256", 3);
	ldquantizer.ComputeEntropy("Results/3f16.256", 3);

	ldquantizer.ComputeEntropy("Results/5chem.256", 5);
	ldquantizer.ComputeEntropy("Results/5house.256", 5);
	ldquantizer.ComputeEntropy("Results/5moon.256", 5);
	ldquantizer.ComputeEntropy("Results/5couple.256", 5);
	ldquantizer.ComputeEntropy("Results/5elaine.256", 5);
	ldquantizer.ComputeEntropy("Results/5f16.256", 5);
	
	
	FileManager file("data/training.dat", Read);
	string str = file.ReadStrings();
	str = str.substr(0, 256);
	ldquantizer.VectorQuantize(str);
	getchar();

}
