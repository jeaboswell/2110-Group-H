#pragma region Includes/Globals
#include "functions.h"
#include <sstream>
#include <fstream>
#include <iterator>
using namespace std;

extern vector<Node> Nodes;
extern vector<Edge> Edges;
locale loc;
#pragma endregion

#pragma region Command Handling
string prompt()
{
	string commandLine;

	cout << "cmd> " << flush;
	getline(cin, commandLine);

	return commandLine;
}

vector<string> split(string input)
{
	string temp;
	for (int i = 0; i < input.length(); i++) // Convert input into all upper case
		temp += toupper(input[i], loc);
	istringstream buf(temp);
	istream_iterator<string> beg(buf), end;

	vector<string> output(beg, end);

	return output;
}

string getPath(string input)
{
	istringstream buf(input);
	istream_iterator<string> beg(buf), end;

	vector<string> output(beg, end);

	if (output.size() > 1)
		return output.at(1);
	else
		return "NULL";
}

bool runCommand(string command)
{
	bool quit = false;
	string filePath;
	vector<string> splitCommands;

	filePath = getPath(command);
	splitCommands = split(command);

	#pragma region No Input Error Check
	if (splitCommands.empty() == true)
	{
		cout << "\a*** ERROR *** NO COMMAND ENTERED" << endl;
		NEWLINE
	}
	#pragma endregion

	#pragma region Node Handling
	else if (splitCommands.front() == "NODE")
	{
		if (splitCommands.size() < 3) // Make sure parameters are met
		{
			cout << "\a*** ERROR *** MISSING PARAMETERS" << endl;
			NEWLINE
		}
		else if (splitCommands.size() > 3)
		{
			cout << "\a*** ERROR *** TOO MANY PARAMETERS" << endl;
			NEWLINE
		}
		else if (splitCommands.at(1) == "ADD") // Add node
		{
			// Add error checking and function call to add node
			NEWLINE
		}
		else if (splitCommands.at(1) == "DELETE") // Delete node
		{
			// Add error checking and function call to delete node
			NEWLINE
		}
		else if (splitCommands.at(1) == "SEARCH") // Search for node
		{
			// Add function call to search for node
			NEWLINE
		}
		else // Error handling
		{
			cout << "\a*** ERROR *** COMMAND NOT RECOGNIZED FOR NODE: " << splitCommands.at(1) << endl;
			NEWLINE
		}
	}
	#pragma endregion

	#pragma region Edge Handling
	else if (splitCommands.front() == "EDGE")
	{
		if (splitCommands.size() < 4) // Make sure parameters are met
		{
			cout << "\a*** ERROR *** MISSING PARAMETERS" << endl;
			NEWLINE
		}
		else if (splitCommands.size() > 4)
		{
			cout << "\a*** ERROR *** TOO MANY PARAMETERS" << endl;
			NEWLINE
		}
		else if (splitCommands.at(1) == "ADD") // Add edge
		{
			// Add error checking and function call to add edge
			NEWLINE
		}
		else if (splitCommands.at(1) == "DELETE") // Delete edge
		{
			// Add error checking and function call to delete node
			NEWLINE
		}
		else // Error handling
		{
			cout << "\a*** ERROR *** COMMAND NOT RECOGNIZED FOR EDGE: " << splitCommands.at(1) << endl;
			NEWLINE
		}
	}
	#pragma endregion

	#pragma region Print Handling
	else if (splitCommands.front() == "PRINT")
	{
		if (splitCommands.size() < 2) // Make sure parameters are met
		{
			cout << "\a*** ERROR *** MISSING PARAMETERS" << endl;
			NEWLINE
		}
		else if (splitCommands.size() > 2)
		{
			cout << "\a*** ERROR *** TOO MANY PARAMETERS" << endl;
			NEWLINE
		}
		else if (Nodes.empty()) // Make sure there is at least one node
		{
			cout << "\a*** ERROR *** NO NODES FOUND" << endl;
			NEWLINE
		}
		else if (splitCommands.at(1) == "MATRIX") // Print Matrix
		{
			// Add function call to print matrix
			NEWLINE
		}
		else if (splitCommands.at(1) == "LIST") // Print list
		{
			// Add function call to print list
			NEWLINE
		}
		else // Error handling
		{
			cout << "\a*** ERROR *** COMMAND NOT RECOGNIZED FOR PRINT: " << splitCommands.at(1) << endl;
			NEWLINE
		}
	}
	#pragma endregion

	#pragma region CSV File Handling
	else if (splitCommands.front() == "FILE")
	{
		if (splitCommands.size() < 2) // Make sure parameters are met
		{
			cout << "\a*** ERROR *** MISSING PARAMETERS" << endl;
			NEWLINE
		}
		else if (splitCommands.size() > 2)
		{
			cout << "\a*** ERROR *** TOO MANY PARAMETERS" << endl;
			NEWLINE
		}
		else // Execute commands from CSV file
			parseFile(filePath);
	}
	#pragma endregion

	#pragma region Clear Screen
	else if (splitCommands.front() == "CLEAR")
		CLEAR
	#pragma endregion

	#pragma region Quit
	else if (splitCommands.front() == "QUIT")
	{
		quit = true;
		CLEAR
	}
	#pragma endregion

	#pragma region Error Check First Command
	else
	{
		cout << "\a*** ERROR *** COMMAND NOT RECOGNIZED: " << splitCommands.front() << endl;
		NEWLINE
	}
	#pragma endregion

	return quit;
}
#pragma endregion

#pragma region File Functions
bool fileExists(string fileName)
{
	ifstream test;
	test.open(fileName.c_str());
	if (test.fail())
	{
		test.close();
		return false;
	}
	else
	{
		test.close();
		return true;
	}
}

bool verifyExtension(string filename)
{
	int period;

	for (int i = 0; i < filename.length(); i++)
	{
		if (filename[i] == '.')
			period = i;
	}

	string extension;

	for (int i = period; i < filename.length(); i++)
		extension += filename[i];

	if (extension == ".csv")
		return true;
	else
		return false;
}

string getExtension(string filename)
{
	int period = 0;

	for (int i = 0; i < filename.length(); i++)
	{
		if (filename[i] == '.')
			period = i;
	}

	string extension;

	for (int i = period; i < filename.length(); i++)
		extension += filename[i];

	return extension;
}

void parseFile(string fileName)
{
	if (fileExists(fileName) && verifyExtension(fileName))
	{
		int loop = 0;
		ifstream file;
		file.open(fileName.c_str());
		string passCommand;
		NEWLINE
		while (file.good())
		{
			string temp;

			getline(file, passCommand, '\n');
			for (int i = 0; i < passCommand.length(); i++)
			{
				if (passCommand[i] != ',')
					temp += passCommand[i];
				else
					temp += " ";
			}

			if (temp != "\0")
			{
				cout << "file cmd> " << temp << endl;
				runCommand(temp);
			}
		}
		file.close();
	}
	else if (!fileExists(fileName))
	{
		cout << "\a*** ERROR *** FILE NOT FOUND: " << fileName << endl;
		if (!verifyExtension(fileName))
			cout << "              CHECK FILE EXTENSION" << endl;
	}
	else if (!verifyExtension(fileName))
		cout << "\a*** ERROR *** INCORRECT FILE EXTENSION: " << getExtension(fileName) << endl;
}
#pragma endregion