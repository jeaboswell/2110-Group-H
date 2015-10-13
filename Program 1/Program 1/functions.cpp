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

#ifdef __linux
Color::Modifier red(Color::FG_RED);
Color::Modifier green(Color::FG_GREEN);
Color::Modifier blue(Color::FG_BLUE);
Color::Modifier def(Color::FG_DEFAULT);
#elif _WIN32
#define red ""
#define green  ""
#define blue  ""
#define def  ""
#endif
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
		cout << red << "\a*** ERROR *** NO COMMAND ENTERED" << endl << def;
		NEWLINE
	}
	#pragma endregion

	#pragma region Node Handling
	else if (splitCommands.front() == "NODE")
	{
		if (splitCommands.size() < 3) // Verify parameter count
		{
			cout << red << "\a*** ERROR *** MISSING PARAMETERS" << endl << def;
			NEWLINE
		}
		else if (splitCommands.size() > 3)
		{
			cout << red << "\a*** ERROR *** TOO MANY PARAMETERS" << endl << def;
			NEWLINE
		}
		else if (splitCommands.at(1) == "ADD") // Add node
		{
			if (nodeExists(splitCommands.at(2))) // Make sure node doesn't exist
				cout << red << "\a*** ERROR *** DUPLICATE NODE: " << def << splitCommands.at(2) << endl;
			else
				nodeAdd(splitCommands.at(2));
			NEWLINE
		}
		else if (splitCommands.at(1) == "DELETE") // Delete node
		{
			if (!nodeExists(splitCommands.at(2))) // Make sure node exists
				cout << red << "\a*** ERROR *** NODE NOT FOUND FOR DELETION: " << def << splitCommands.at(2) << endl;
			else
				nodeDelete(splitCommands.at(2));
			NEWLINE
		}
		else if (splitCommands.at(1) == "SEARCH") // Search for node
		{
			if (nodeExists(splitCommands.at(2)))
				cout << def << "NODE " << splitCommands.at(2) << ": " << green << "FOUND" << endl << def;
			else
				cout << def << "NODE " << splitCommands.at(2) << ": " << red << "NOT FOUND" << endl << def;
			NEWLINE
		}
		else // Error handling
		{
			cout << red << "\a*** ERROR *** COMMAND NOT RECOGNIZED FOR NODE: " << blue << splitCommands.at(1) << endl << def;
			NEWLINE
		}
	}
	#pragma endregion

	#pragma region Edge Handling
	else if (splitCommands.front() == "EDGE")
	{
		if (splitCommands.size() < 4) // Verify parameter count
		{
			cout << red << "\a*** ERROR *** MISSING PARAMETERS" << endl << def;
			NEWLINE
		}
		else if (splitCommands.size() > 4)
		{
			cout << red << "\a*** ERROR *** TOO MANY PARAMETERS" << endl << def;
			NEWLINE
		}
		else if (splitCommands.at(1) == "ADD") // Add edge
		{
			if (edgeExists(splitCommands.at(2), splitCommands.at(3))) // Make sure edge doesn't already exist
				cout << red << "\a*** ERROR *** DUPLICATE EDGE: " << def << splitCommands.at(2) << "-" << splitCommands.at(3) << endl;
			else if (!nodeExists(splitCommands.at(2)) && !nodeExists(splitCommands.at(3))) // Error if start and end nodes don't exist
			{
				cout << red << "\a*** ERROR *** NODE " << def << splitCommands.at(2) << red << ": NOT FOUND" << endl << def;
				cout << red << "\a*** ERROR *** NODE " << def << splitCommands.at(3) << red << ": NOT FOUND" << endl << def;
			}
			else if (!nodeExists(splitCommands.at(2))) // Error if start node doesn't exist
				cout << red << "\a*** ERROR *** NODE " << def << splitCommands.at(2) << red << ": NOT FOUND" << endl << def;
			else if (!nodeExists(splitCommands.at(3))) // Error if end node doesn't exist
				cout << red << "\a*** ERROR *** NODE " << def << splitCommands.at(3) << red << ": NOT FOUND" << endl << def;
			else
				edgeAdd(splitCommands.at(2), splitCommands.at(3));
			NEWLINE
		}
		else if (splitCommands.at(1) == "DELETE") // Delete edge
		{
			if (!edgeExists(splitCommands.at(2), splitCommands.at(3))) // Make sure edge exists
				cout << red << "\a*** ERROR *** EDGE NOT FOUND FOR DELETION: " << def << splitCommands.at(2) << "-" << splitCommands.at(3) << endl;
			else
				edgeDelete(splitCommands.at(2), splitCommands.at(3));
			NEWLINE
		}
		else // Error handling
		{
			cout << red << "\a*** ERROR *** COMMAND NOT RECOGNIZED FOR EDGE: " << blue << splitCommands.at(1) << endl << def;
			NEWLINE
		}
	}
	#pragma endregion

	#pragma region Print Handling
	else if (splitCommands.front() == "PRINT")
	{
		if (splitCommands.size() < 2) // Verify parameter count
		{
			cout << red << "\a*** ERROR *** MISSING PARAMETERS" << endl << def;
			NEWLINE
		}
		else if (splitCommands.size() > 2)
		{
			cout << red << "\a*** ERROR *** TOO MANY PARAMETERS" << endl << def;
			NEWLINE
		}
		else if (Nodes.empty()) // Make sure there are nodes present
		{
			cout << red << "\a*** ERROR *** NO NODES FOUND" << endl << def;
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
			cout << red << "\a*** ERROR *** COMMAND NOT RECOGNIZED FOR PRINT: " << blue << splitCommands.at(1) << endl << def;
			NEWLINE
		}
	}
	#pragma endregion

	#pragma region CSV File Handling
	else if (splitCommands.front() == "FILE")
	{
		if (splitCommands.size() < 2) // Verify parameter count
		{
			cout << red << "\a*** ERROR *** MISSING PARAMETERS" << endl << def;
			NEWLINE
		}
		else if (splitCommands.size() > 2)
		{
			cout << red << "\a*** ERROR *** TOO MANY PARAMETERS" << endl << def;
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

	#pragma region Help Page
	else if (splitCommands.front() == "HELP")
		printHelp();
	#pragma endregion

	#pragma region Quit
	else if (splitCommands.front() == "QUIT")
		quit = true;
	#pragma endregion

	#pragma region Error Check First Command
	else
	{
		cout << red << "\a*** ERROR *** COMMAND NOT RECOGNIZED: " << def << splitCommands.front() << endl;
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

	cout << green << "ADDED" << def << ": NODE " << name << endl;
}

// Delete node from Nodes vector
void nodeDelete(string name)
{
	for (unsigned int i = 0; i < Nodes.size(); i++)
	{
		if (Nodes.at(i).name == name)
		{
			Nodes.erase(Nodes.begin() + i); // Delete node
			cout << red << "DELETED" << def << ": NODE " << name << endl;
			if (!Edges.empty())
			{
				for (unsigned int j = 0; j < Edges.size(); j++) // Look for edges containing node
				{
					if (Edges.at(j).start != Edges.at(j).end) // Edge does not start and end at same node
					{
						if (Edges.at(j).start == name)
						{
							cout << blue << "    EDGE AUTO-REMOVED BY NODE DELETION: " << def << Edges.at(j).start << "-" << Edges.at(j).end << endl;
							Edges.erase(Edges.begin() + j);
						}
						if (!Edges.empty())
						{
							if (Edges.at(j).end == name)
							{
								cout << blue << "    EDGE AUTO-REMOVED BY NODE DELETION: " << def << Edges.at(j).start << "-" << Edges.at(j).end << endl;
								Edges.erase(Edges.begin() + j);
								j -= 1;
							}
						}
					}
					else if (Edges.at(j).start == name) // Edge starts and ends at same node
					{
						cout << blue << "    EDGE AUTO-REMOVED BY NODE DELETION: " << def << Edges.at(j).start << "-" << Edges.at(j).end << endl;
						Edges.erase(Edges.begin() + j);
						j -= 1;
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
	cout << green << "ADDED" << def << ": EDGE " << start << "-" << end << endl;

	if (start.compare(end) != 0)
	{
		Edge temp2 = Edge(end, start);
		Edges.push_back(temp2);
		cout << green << "ADDED" << def << ": EDGE " << end << "-" << start << endl;
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
			cout << red << "DELETED" << def << ": EDGE " << start << "-" << end << endl;
		}
		if (!Edges.empty())
		{
			if (Edges.at(i).start == end && Edges.at(i).end == start)
			{
				Edges.erase(Edges.begin() + i);
				cout << red << "DELETED" << def << ": EDGE " << end << "-" << start << endl;
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
		cout << red << "\a*** ERROR *** FILE NOT FOUND: " << def << fileName << endl;
		if (!verifyExtension(fileName))
		{
			if (getExtension(fileName) != "NO FILE")
				cout << blue << "              CHECK FILE EXTENSION" << def << endl;
			else
				cout << blue << "              NO FILE IN PATH" << def << endl;
		}
		NEWLINE
	}
	else if (!verifyExtension(fileName)) // Error for bad extension or no file in path
	{
		if (getExtension(fileName) != "NO FILE")
			cout << red << "\a*** ERROR *** INCORRECT FILE EXTENSION: " << def << getExtension(fileName) << endl;
		else
			cout << red << "\a*** ERROR *** NO FILE IN PATH: " << def << fileName << endl;
		NEWLINE
	}
}
#pragma endregion

#pragma region Help Function
void printHelp()
{
	NEWLINE
		cout << left << setw(60) << setfill('_') << "Nodes" << endl;
		cout << blue << "Adding a node to the program:" << endl;
		cout << green << "\tnode add <NAME>" << endl;
		cout << blue << "Deleting a node from the program:" << endl;
		cout << green << "\tnode delete <NAME>" << endl;
		cout << blue << "Searching the program for an existing node:" << endl;
		cout << green << "\tnode search <NAME>" << endl;
	NEWLINE
		cout << def << left << setw(60) << "Edges" << endl;
		cout << blue << "Adding an edge to the program:" << endl;
		cout << green << "\tedge add <START NODE> <END NODE>" << endl;
		cout << blue << "Deleting an edge from the program:" << endl;
		cout << green << "\tedge delete <START NODE> <END NODE>" << endl;
	NEWLINE
		cout << def << left << setw(60) << "Printing" << endl;
		cout << blue << "Printing the adjacency matrix:" << endl;
		cout << green << "\tprint matrix" << endl;
		cout << blue << "Printing the adjacency list:" << endl;
		cout << green << "\tprint list" << endl;
	NEWLINE
		cout << def << left << setw(60) << "Command File" << endl;
		cout << blue << "Using a .csv file to execute commands:" << endl;
		cout << green << "\tfile <FILE PATH>" << endl;
		cout << def << "\tNote: The file path may" << red << " NOT " << def << "contain any spaces" << endl;
	NEWLINE
		cout << left << setw(60) << "Other Commands" << endl;
		cout << blue << "Clear the screen:" << endl;
		cout << green << "\tclear" << endl;
		cout << blue << "List the contents of the program folder:" << endl;
		cout << green << "\tls" << endl;
		cout << blue << "Get a list of all commands:" << endl;
		cout << green << "\thelp" << endl;
		cout << blue << "Exit the program:" << endl;
		cout << green << "\tquit" << endl << def;
	NEWLINE
	KEYPRESS
	CLEAR
}
#pragma endregion