#pragma region Includes/Globals
#include <algorithm>
#include <fstream>
#include "functions.h"
#include <iterator>
#include <sstream>
using namespace std;

extern vector<Node> Nodes;
extern vector<Edge> Edges;
locale loc;
#pragma endregion

#pragma region Constructors
Node::Node(string newName)
{
	name = newName;
}

Edge::Edge(string newStart, string newEnd)
{
	start = newStart;
	end = newEnd;
	length = 0; // In preparation for later programs
}
#pragma endregion

#pragma region Input Processing
// Prompt user to enter command
string prompt()
{
	string commandLine;

	cout << "cmd> " << flush;
	getline(cin, commandLine); // Get command from user
	
	return commandLine;
}

// Split command string into substrings
vector<string> split(string input)
{
	string temp;
	for (unsigned int i = 0; i < input.length(); i++) // Convert input into all upper case
		temp += toupper(input[i], loc);
	istringstream buf(temp);
	istream_iterator<string> beg(buf), end;

	vector<string> output(beg, end);

	return output;
}

// Get possible file path for .csv file (So that its case does not get changed)
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

// Execute command entered by user
bool runCommand(string command)
{
	#pragma region Variables
	bool quit = false;
	string filePath;
	vector<string> splitCommands;
	#pragma endregion

	filePath = getPath(command); // Get file path
	splitCommands = split(command); // Get command substrings

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
		if (splitCommands.size() < 3) // Verify parameter count
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
			if (nodeExists(splitCommands.at(2))) // Make sure node doesn't exist
				cout << "\a*** ERROR *** DUPLICATE NODE: " << splitCommands.at(2) << endl;
			else
				nodeAdd(splitCommands.at(2));
			NEWLINE
		}
		else if (splitCommands.at(1) == "DELETE") // Delete node
		{
			if (!nodeExists(splitCommands.at(2))) // Make sure node exists
				cout << "\a*** ERROR *** NODE NOT FOUND FOR DELETION: " << splitCommands.at(2) << endl;
			else
				nodeDelete(splitCommands.at(2));
			NEWLINE
		}
		else if (splitCommands.at(1) == "SEARCH") // Search for node
		{
			if (nodeExists(splitCommands.at(2)))
				cout << "NODE " << splitCommands.at(2) << ": FOUND" << endl;
			else
				cout << "NODE " << splitCommands.at(2) << ": NOT FOUND" << endl;
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
		if (splitCommands.size() < 4) // Verify parameter count
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
			if (edgeExists(splitCommands.at(2), splitCommands.at(3))) // Make sure edge doesn't already exist
				cout << "\a*** ERROR *** DUPLICATE EDGE: " << splitCommands.at(2) << "-" << splitCommands.at(3) << endl;
			else if (!nodeExists(splitCommands.at(2)) && !nodeExists(splitCommands.at(3))) // Error if start and end nodes don't exist
			{
				cout << "\a*** ERROR *** NODE " << splitCommands.at(2) << ": NOT FOUND" << endl;
				cout << "\a*** ERROR *** NODE " << splitCommands.at(3) << ": NOT FOUND" << endl;
			}
			else if (!nodeExists(splitCommands.at(2))) // Error if start node doesn't exist
				cout << "\a*** ERROR *** NODE " << splitCommands.at(2) << ": NOT FOUND" << endl;
			else if (!nodeExists(splitCommands.at(3))) // Error if end node doesn't exist
				cout << "\a*** ERROR *** NODE " << splitCommands.at(3) << ": NOT FOUND" << endl;
			else
				edgeAdd(splitCommands.at(2), splitCommands.at(3));
			NEWLINE
		}
		else if (splitCommands.at(1) == "DELETE") // Delete edge
		{
			if (!edgeExists(splitCommands.at(2), splitCommands.at(3))) // Make sure edge exists
				cout << "\a*** ERROR *** EDGE NOT FOUND FOR DELETION: " << splitCommands.at(2) << "-" << splitCommands.at(3) << endl;
			else
				edgeDelete(splitCommands.at(2), splitCommands.at(3));
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
		if (splitCommands.size() < 2) // Verify parameter count
		{
			cout << "\a*** ERROR *** MISSING PARAMETERS" << endl;
			NEWLINE
		}
		else if (splitCommands.size() > 2)
		{
			cout << "\a*** ERROR *** TOO MANY PARAMETERS" << endl;
			NEWLINE
		}
		else if (Nodes.empty()) // Make sure there are nodes present
		{
			cout << "\a*** ERROR *** NO NODES FOUND" << endl;
			NEWLINE
		}
		else if (splitCommands.at(1) == "MATRIX") // Print Matrix
		{
			printMatrix();
			NEWLINE
		}
		else if (splitCommands.at(1) == "LIST") // Print list
		{
			printList();
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
		if (splitCommands.size() < 2) // Verify parameter count
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

	#pragma region List Contents of current folder
	else if (splitCommands.front() == "LS")
	{
		system("ls");
		NEWLINE
	}
	#pragma endregion

	#pragma region Quit
	else if (splitCommands.front() == "QUIT")
		quit = true;
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

#pragma region Node Functions
// Check if node exists
bool nodeExists(string name)
{
	if (!Nodes.empty())
	{
		for (unsigned int i = 0; i < Nodes.size(); i++)
		{
			if (Nodes.at(i).name == name)
				return true;
		}
	}
	return false;
}

// Add node to Nodes vector
void nodeAdd(string name)
{
	Node temp = Node(name);
	Nodes.push_back(temp);

	cout << "ADDED: NODE " << name << endl;
}

// Delete node from Nodes vector
void nodeDelete(string name)
{
	for (unsigned int i = 0; i < Nodes.size(); i++)
	{
		if (Nodes.at(i).name == name)
		{
			Nodes.erase(Nodes.begin() + i); // Delete node
			cout << "DELETED: NODE " << name << endl;
			if (!Edges.empty())
			{
				for (unsigned int j = 0; j < Edges.size(); j++) // Look for edges containing node
				{
					if (Edges.at(j).start != Edges.at(j).end) // Edge does not start and end at same node
					{
						if (Edges.at(j).start == name)
						{
							cout << "EDGE AUTO-REMOVED BY NODE DELETION: " << Edges.at(j).start << "-" << Edges.at(j).end << endl;
							Edges.erase(Edges.begin() + j);
						}
						if (!Edges.empty())
						{
							if (Edges.at(j).end == name)
							{
								cout << "EDGE AUTO-REMOVED BY NODE DELETION: " << Edges.at(j).start << "-" << Edges.at(j).end << endl;
								Edges.erase(Edges.begin() + j);
								j -= 1;
							}
						}
					}
					else if (Edges.at(j).start == name) // Edge starts and ends at same node
					{
						cout << "EDGE AUTO-REMOVED BY NODE DELETION: " << Edges.at(j).start << "-" << Edges.at(j).end << endl;
						Edges.erase(Edges.begin() + j);
					}
				}
			}
		}
	}
}
#pragma endregion

#pragma region Edge Functions
// Check if edge exists
bool edgeExists(string start, string end)
{
	if (!Edges.empty())
	{
		for (unsigned int i = 0; i < Edges.size(); i++)
		{
			if (Edges.at(i).start == start && Edges.at(i).end == end)
				return true;
		}
	}
	return false;
}

// Add new edge to edge list
void edgeAdd(string start, string end)
{
	Edge temp1 = Edge(start, end);
	Edges.push_back(temp1);
	cout << "ADDED: EDGE " << start << "-" << end << endl;

	if (start.compare(end) != 0)
	{
		Edge temp2 = Edge(end, start);
		Edges.push_back(temp2);
		cout << "ADDED: EDGE " << end << "-" << start << endl;
	}
}

// Remove edge from edge list
void edgeDelete(string start, string end)
{
	for (unsigned int i = 0; i < Edges.size(); i++)
	{
		if (Edges.at(i).start == start && Edges.at(i).end == end)
		{
			Edges.erase(Edges.begin() + i);
			cout << "DELETED: EDGE " << start << "-" << end << endl;
		}
		if (!Edges.empty())
		{
			if (Edges.at(i).start == end && Edges.at(i).end == start)
			{
				Edges.erase(Edges.begin() + i);
				cout << "DELETED: EDGE " << end << "-" << start << endl;
				i -= 1;
			}
		}
	}
}
#pragma endregion

#pragma region Print Functions
// Print Adjacency Matrix
void printMatrix()
{
	// Alphabatize nodes temporarily
	vector<Node> alphabetical;
	alphabetical = Nodes;
	sort(alphabetical.begin(), alphabetical.end(), nameCompare);

	cout << "  " << flush;
	for (unsigned int i = 0; i < alphabetical.size(); i++)
		cout << left << setw(3) << alphabetical.at(i).name << flush;
	NEWLINE

	for (unsigned int i = 0; i < alphabetical.size(); i++)
	{
		cout << alphabetical.at(i).name << " " << flush;
		for (unsigned int j = 0; j < alphabetical.size(); j++)
		{
			if (edgeExists(alphabetical.at(i).name, alphabetical.at(j).name))
				cout << left << setw(3) << "1" << flush;
			else
				cout << left << setw(3) << "0" << flush;
		}
		NEWLINE
	}
}

// Print adjacency list
void printList()
{
	// Alphabatize nodes temporarily
	vector<Node> alphabetical;
	alphabetical = Nodes;
	sort(alphabetical.begin(), alphabetical.end(), nameCompare);

	for (unsigned int i = 0; i < alphabetical.size(); i++)
	{
		cout << alphabetical.at(i).name << flush;
		for (unsigned int j = 0; j < alphabetical.size(); j++)
		{
			if (edgeExists(alphabetical.at(i).name, alphabetical.at(j).name))
				cout << "  ->  " << alphabetical.at(j).name << flush;
		}
		NEWLINE
	}
}

// Used to alphabatize list
bool nameCompare(Node a, Node b)
{
	if (a.name.compare(b.name) < 0)
		return true;
	else
		return false;
}
#pragma endregion

#pragma region File Functions
// Make sure file exists
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

// Make sure file has correct extension
bool verifyExtension(string filename)
{
	int period = 0;

	for (unsigned int i = 0; i < filename.length(); i++)
	{
		if (filename[i] == '.')
			period = i;
	}

	string extension;

	for (unsigned int i = period; i < filename.length(); i++)
		extension += filename[i];

	if (extension == ".csv")
		return true;
	else
		return false;
}

// Get the file extension
string getExtension(string filename)
{
	int period = 0;

	for (unsigned int i = 0; i < filename.length(); i++)
	{
		if (filename[i] == '.')
			period = i;
	}

	string extension;

	if (period != 0)
	{
		for (unsigned int i = period; i < filename.length(); i++)
			extension += filename[i];
	}
	else
		extension = "NO FILE";

	return extension;
}

// Read lines from file and execute
void parseFile(string fileName)
{
	if (fileExists(fileName) && verifyExtension(fileName))
	{
		int loop = 0;
		ifstream fs;
		fs.open(fileName.c_str());
		string passCommand;
		NEWLINE

		while (fs.good())
		{
			string temp;
			
			getline(fs, passCommand, '\n');
			for (unsigned int i = 0; i < passCommand.length(); i++)
			{
				if (passCommand[i] != ',')
					temp += passCommand[i];
				else
					temp += " ";
			}

			if (temp != "\0")
			{
				cout << "file cmd> " << temp << endl;
				if (runCommand(temp))
					exit(0);
			}
		}
		fs.close();
	}
	else if (!fileExists(fileName)) // Error for non-existant file
	{
		cout << "\a*** ERROR *** FILE NOT FOUND: " << fileName << endl;
		if (!verifyExtension(fileName))
		{
			if (getExtension(fileName) != "NO FILE")
				cout << "              CHECK FILE EXTENSION" << endl;
			else
				cout << "              NO FILE IN PATH" << endl;
		}
		NEWLINE
	}
	else if (!verifyExtension(fileName)) // Error for bad extension or no file in path
	{
		if (getExtension(fileName) != "NO FILE")
			cout << "\a*** ERROR *** INCORRECT FILE EXTENSION: " << getExtension(fileName) << endl;
		else
			cout << "\a*** ERROR *** NO FILE IN PATH: " << fileName << endl;
		NEWLINE
	}
}
#pragma endregion