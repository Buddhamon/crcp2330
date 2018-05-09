#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

void readASMMFile(char*);
void initializeMaps();
void removeComments();
void removeWhiteSpace();
void deleteEmptyLines();
void replaceLabels();
void outputHackFile(char*);

vector<string> linesOfCode;
map<string, string> dest;
map<string, string> comp;
map<string, string> jump;
map<string, string> addressMap;

int main(int argc, char* inputFile[])
{
	if(argc < 2)
	{
		cout << "Error: No File Argument Found" << endl;
		exit(1);
	}
	readASMMFile(inputFile[1]);
	initializeMaps();
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

void initializeMaps()
{
	dest["null"] = 	"000";
	dest["M"] = 	"001";
	dest["D"] = 	"010";
	dest["MD"] = 	"011";
	dest["A"] = 	"100";
	dest["AM"] = 	"101";
	dest["AD"] = 	"110";
	dest["AMD"] = 	"111";

	comp["0"] = 	"0101010"
	comp["1"] = 	"0111111"
	comp["-1"] = 	"0111010"
	comp["D"] = 	"0001100"
	comp["A"] = 	"0110000"
	comp["!D"] = 	"0001101"
	comp["!A"] = 	"0110001"
	comp["-D"] = 	"0001111"
	comp["-A"] = 	"0110011"
	comp["D+1"] = 	"0011111"
	comp["A+1"] = 	"0110111"
	comp["D-1"] = 	"0001110"
	comp["A-1"] = 	"0110010"
	comp["D+A"] = 	"0000010"
	comp["D-A"] = 	"0010011"
	comp["A-D"] = 	"0000111"
	comp["D&A"] = 	"0000000"
	comp["D|A"] = 	"0010101"

	comp["M"] = 	"1110000"
	comp["!M"] = 	"1110001"
	comp["-M"] = 	"1110011"
	comp["M+1"] = 	"1110111"
	comp["M-1"] = 	"1110010"
	comp["D+M"] = 	"1000010"
	comp["D-M"] = 	"1010011"
	comp["M-D"] = 	"1000111"
	comp["D&A"] = 	"1000000"
	comp["D|A"] = 	"1010101"

	jump["null"] = 	"000";
	jump["JGT"] = 	"001";
	jump["JEQ"] = 	"010";
	jump["JGE"] = 	"011";
	jump["JLT"] = 	"100";
	jump["JNE"] = 	"101";
	jump["JNE"] = 	"110";
	jump["JMP"] = 	"111";
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
				string shortenedLine = linesOfCode[i].substr(0, newLineLength);
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

void replaceLabels()
{

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



