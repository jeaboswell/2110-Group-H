#pragma region Includes/Externs
#include "functions.h"
#include <sstream>
#include <iterator>
using namespace std;

extern vector<Node> Nodes;
extern vector<Edge> Edges;
#pragma endregion

bool prompt()
{
	bool check, quit = false;
	string commandLine;
	vector<string> splitCommands;

	cout << "cmd> " << flush;
	getline(cin, commandLine);

	splitCommands = split(commandLine);

	#pragma region No Input Error Check
	if (splitCommands.empty() == true)
	{
		cout << "\a*** ERROR *** NO COMMAND ENTERED" << endl;
		NEWLINE
	}
	#pragma endregion

	#pragma region Node Handling
	else if (splitCommands.front() == "node")
	{
		if (splitCommands.size() != 3)
		{
			cout << "\a*** ERROR *** MISSING PARAMETERS" << endl;
			NEWLINE
		}
		else if (splitCommands.at(1) == "add") // Add node
		{
			cout << "Add Node" << endl;
			NEWLINE
		}
		else if (splitCommands.at(1) == "delete") // Delete node
		{
			cout << "Delete Node" << endl;
			NEWLINE
		}
		else if (splitCommands.at(1) == "search") // Search for node
		{
			cout << "Search Node" << endl;
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
	else if (splitCommands.front() == "edge")
	{
		if (splitCommands.size() != 4)
		{
			cout << "\a*** ERROR *** MISSING PARAMETERS" << endl;
			NEWLINE
		}
		else if (splitCommands.at(1) == "add") // Add edge
		{
			cout << "Add Edge" << endl;
			NEWLINE
		}
		else if (splitCommands.at(1) == "delete") // Delete edge
		{
			cout << "Delete Edge" << endl;
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
	else if (splitCommands.front() == "print")
	{
		if (splitCommands.size() != 2)
		{
			cout << "\a*** ERROR *** MISSING PARAMETERS" << endl;
			NEWLINE
		}
		else if (splitCommands.at(1) == "matrix") // Print Matrix
		{
			cout << "Print Matrix" << endl;
			NEWLINE
		}
		else if (splitCommands.at(1) == "list") // Print list
		{
			cout << "Print List" << endl;
			NEWLINE
		}
		else // Error handling
		{
			cout << "\a*** ERROR *** COMMAND NOT RECOGNIZED FOR PRINT: " << splitCommands.at(1) << endl;
			NEWLINE
		}
	}
	#pragma endregion

	#pragma region Command File
	else if (splitCommands.front() == "file")
	{
		if (splitCommands.size() != 2)
		{
			cout << "\a*** ERROR *** MISSING PARAMETERS" << endl;
			NEWLINE
		}
		else // Execute commands from CSV file
		{
			cout << "Parse File" << endl;
			NEWLINE
		}
	}
	#pragma endregion

	#pragma region Clear Screen
	else if (splitCommands.front() == "clear")
		CLEAR
	#pragma endregion

	#pragma region Quit
	else if (splitCommands.front() == "quit")
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

vector<string> split(string input)
{
	istringstream buf(input);
	istream_iterator<string> beg(buf), end;

	vector<string> output(beg, end);

	return output;
}