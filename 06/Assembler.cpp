/*
* Will Lacey - Nand2Tetris
* May 9th, 2018
* Assembler program written in c++ for Hack language
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <bitset>

using namespace std;

void readASMMFile(char*);
void initializeMaps();
void removeComments();
void removeWhiteSpace();
void deleteEmptyLines();
void replaceAddresses();
void replaceInstructions();
void outputHackFile(char*);

vector<string> linesOfCode;
map<string, string> destMap;
map<string, string> compMap;
map<string, string> jumpMap;
map<string, string> addressMap;
int availableAddress = 16;
int labelCount = 0;

// Main Runner for Assembler.cpp
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
	replaceAddresses();
	replaceInstructions();

	outputHackFile(inputFile[1]);
	return 0;
}

// Parses code inside the commandline argument 
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

// Initializes instruction maps 
void initializeMaps()
{
	destMap["null"] = 	"000";
	destMap["M"] = 		"001";
	destMap["D"] = 		"010";
	destMap["MD"] = 	"011";
	destMap["A"] = 		"100";
	destMap["AM"] = 	"101";
	destMap["AD"] = 	"110";
	destMap["AMD"] = 	"111";

	compMap["0"] = 		"0101010";
	compMap["1"] = 		"0111111";
	compMap["-1"] = 	"0111010";
	compMap["D"] = 		"0001100";
	compMap["A"] = 		"0110000";
	compMap["!D"] = 	"0001101";
	compMap["!A"] = 	"0110001";
	compMap["-D"] = 	"0001111";
	compMap["-A"] = 	"0110011";
	compMap["D+1"] = 	"0011111";
	compMap["A+1"] = 	"0110111";
	compMap["D-1"] = 	"0001110";
	compMap["A-1"] = 	"0110010";
	compMap["D+A"] = 	"0000010";
	compMap["D-A"] = 	"0010011";
	compMap["A-D"] = 	"0000111";
	compMap["D&A"] = 	"0000000";
	compMap["D|A"] = 	"0010101";

	compMap["M"] = 		"1110000";
	compMap["!M"] = 	"1110001";
	compMap["-M"] = 	"1110011";
	compMap["M+1"] = 	"1110111";
	compMap["M-1"] = 	"1110010";
	compMap["D+M"] = 	"1000010";
	compMap["D-M"] = 	"1010011";
	compMap["M-D"] = 	"1000111";
	compMap["D&A"] = 	"1000000";
	compMap["D|A"] = 	"1010101";

	jumpMap["null"] = 	"000";
	jumpMap["JGT"] = 	"001";
	jumpMap["JEQ"] = 	"010";
	jumpMap["JGE"] = 	"011";
	jumpMap["JLT"] = 	"100";
	jumpMap["JNE"] = 	"101";
	jumpMap["JNE"] = 	"110";
	jumpMap["JMP"] = 	"111";

	addressMap["SP"] = 		"0";
	addressMap["LCL"] = 	"1";
	addressMap["ARG"] = 	"2";
	addressMap["THIS"] = 	"3";
	addressMap["THAT"] = 	"4";

	addressMap["R0"] = 		"0";
	addressMap["R1"] = 		"1";
	addressMap["R2"] = 		"2";
	addressMap["R3"] = 		"3";
	addressMap["R4"] = 		"4";
	addressMap["R5"] = 		"5";
	addressMap["R6"] = 		"6";
	addressMap["R7"] = 		"7";
	addressMap["R8"] = 		"8";
	addressMap["R9"] = 		"9";
	addressMap["R10"] = 	"10";
	addressMap["R11"] = 	"11";
	addressMap["R12"] = 	"12";
	addressMap["R13"] = 	"13";
	addressMap["R14"] = 	"14";
	addressMap["R15"] = 	"15";
	addressMap["SCREEN"] = 	"16384";
	addressMap["KBD"] = 	"24576";

}

// Deletes all code that follows "//"" 
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

// Removes white space characters defined in the initial c++ library
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

// Deletes lines of code that have been stripped down to an empty instruction
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

// Code is temporary and incorrect, currently this function serves to handle labels and addresses
void replaceAddresses()
{
	for(int i = linesOfCode.size()-1; i >= 0; i--) // First loop filters out Unwanted Label Symbols
	{
		if(linesOfCode[i].at(0) == '(')	
		{	
			labelCount++;
		}
	}
	for(int i = linesOfCode.size()-1; i >= 0; i--)
	{
		bool isAddress = true;
		char operation = linesOfCode[i].at(0);
		string address = "";
		for(int j = 1; j < linesOfCode[i].length() && isAddress; j++)
		{
			char c = linesOfCode[i].at(j);
			if(operation == '@' || operation == '(')	
			{
				if(c != ')')
				{
					address = address + c;
				}
				else
				{ 
					int temp = i+1 - labelCount;
					addressMap[address] = to_string(temp); // Label Symbol
					linesOfCode.erase(linesOfCode.begin() + i);
					labelCount--;
					isAddress = false;
				}
			}
			else
			{
				linesOfCode[i] = "_" + linesOfCode[i]; // Temporary insertion to handle labels
				isAddress = false;
			}
		}
		if(isAddress)
		{
			if(isalpha(address.at(0)))
			{
				map<string, string>::iterator it;
				it = addressMap.find(address);
			  	if (it != addressMap.end())
			  	{
					address = addressMap.find(address)->second;
			  	}
				else
				{
					addressMap[address] = to_string(availableAddress); // variable symbol
					address = to_string(availableAddress);
					availableAddress++;
				}
			}
			int binary = stoi(address);
			string binaryCode = bitset<16>(binary).to_string(); //to binary
			linesOfCode[i] = binaryCode;
		}
	}
}

// Replaces Hack c instructions with binary equivalents
void replaceInstructions()
{
	for(int i = linesOfCode.size()-1; i >= 0; i--)
	{
		char c = linesOfCode[i].at(0); // Temporary insertion to handle labels
		if(c == '_') // Temporary insertion to handle labels
		{
			string dest = "";
			string comp = "";
			string jump = "";
			char operation = ' ';
			for(int j = 1; j < linesOfCode[i].length(); j++)
			{
				c = linesOfCode[i].at(j);
				if(operation == ' ' && c != '=' && c != ';')
				{
					dest = dest + c; // Assume destination leads the Hack instruction and operation will be '='
				}
				else if(operation == ' ')
				{
					operation = c;
					if(operation == '=')
					{
						jump = "null";
					}
					else
					{
						comp = dest; // If the operation is ';', then dest was improperly built
						dest = "null";
					}
				}
				else if(operation == '=')
				{

					comp = comp + c;
				}
				else if(operation == ';')
				{
					jump = jump + c;
				}
			}
			string binaryCode = "111";

			map<string, string>::iterator it;
			it = compMap.find(comp);
  			if (it != destMap.end())
				binaryCode = binaryCode + compMap.find(comp)->second;
			else
			{
				cout << "Error: C instruction not found" << endl;
				exit(1);
			}

			it = destMap.find(dest);
  			if (it != destMap.end())
				binaryCode = binaryCode + destMap.find(dest)->second;
			else
			{
				cout << "Error: C instruction not found" << endl;
				exit(1);
			}

			it = jumpMap.find(jump);
  			if (it != destMap.end())
				binaryCode = binaryCode + jumpMap.find(jump)->second;
			else
			{
				cout << "Error: C instruction not found" << endl;
				exit(1);
			}
			linesOfCode[i] = binaryCode;
		}
	}
}

// Output of Assembler.cpp, creates a .hack file containing binary hack code
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



