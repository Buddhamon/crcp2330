#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

void readASMMFile(char*);
void removeComments();
void removeWhiteSpace();
void deleteEmptyLines();
void outputHackFile(char*);

vector<string> linesOfCode;

int main(int argc, char* inputFile[])
{
	if(argc < 2)
	{
		cout << "Error: No File Argument Found" << endl;
		exit(1);
	}
	readASMMFile(inputFile[1]);

	removeComments();
	removeWhiteSpace();
	deleteEmptyLines();

	outputHackFile(inputFile[1]);
	return 0;
}

void readASMMFile(char* inputFile)
{
	string line = "";
	ifstream file(inputFile);
	while(file)
	{
		getline(file, line);
		linesOfCode.push_back(line);
	}
}

void removeComments()
{
	string comments = "//";
	for(int i = 0; i < linesOfCode.size()-1; i++)
	{
		bool noCommentFound = true;
		int lineLength = linesOfCode[i].length();
		for(int j = 0; j < lineLength && noCommentFound; j++)
		{
			string potentialComments = linesOfCode[i].substr(j, 2);
			if(comments.compare(potentialComments) == 0)
			{
				noCommentFound = false;
				int newLineLength = lineLength - (lineLength - j);
				string shortenedLine = linesOfCode[i].substr(j, newLineLength);
				linesOfCode[i] = shortenedLine;
			}
		}
	}
}

void removeWhiteSpace()
{
	for(int i = 0; i < linesOfCode.size(); i++)
	{
		for(int j = 0; j < linesOfCode[i].length(); j++)
		{
			char c = linesOfCode[i].at(j);
			if(isspace(c))
			{
				linesOfCode[i].erase(remove(linesOfCode[i].begin(), linesOfCode[i].end(), c), linesOfCode[i].end());
			}
		}
	}
}

void deleteEmptyLines()
{
	for(int i = linesOfCode.size()-1; i >= 0; i--)
	{
		if(linesOfCode[i].length() == 0)
		{
			linesOfCode.erase(linesOfCode.begin() + i);
		}
	}
}

void outputHackFile(char* inputFile)
{
	string outputFile = inputFile;
	outputFile = outputFile.substr(0, outputFile.length()-3) + "hack";
	ofstream file(outputFile);
	for(int i = 0; i < linesOfCode.size()-1; i++)
	{
		file << linesOfCode[i] << endl;
	}
	file << linesOfCode[linesOfCode.size()-1];
}



