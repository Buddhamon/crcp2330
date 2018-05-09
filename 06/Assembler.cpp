#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

void readInputFile(char*);
void outputFile(char*);

vector<string> linesOfCode;

int main(int argc, char* inputFile[])
{
	readInputFile(inputFile[1]);
	outputFile(inputFile[1]);
	return 0;
}

void readInputFile(char* inputFile)
{
	string line = "";
	ifstream file(inputFile);
	while(file)
	{
		getline(file, line);
		linesOfCode.push_back(line);
	}
}

void outputFile(char* inputFile)
{
	string outputFile = inputFile;
	outputFile = outputFile.substr(0, outputFile.length()-3) + "hack";
	ofstream file(outputFile);
	for(int i = 0; i < linesOfCode.size(); i++)
	{
		file << linesOfCode[i] << endl;
	}
}



