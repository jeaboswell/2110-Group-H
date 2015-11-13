#pragma region Includes/Globals
#include <algorithm>
#include <fstream>
#include "GroupH_10_P4_Header.h"
#include <iterator>
#include <sstream>
using namespace std;

#define vstring vector<string>
#define vedge vector<Edge>
#define vnode vector<Node>

extern vnode Nodes;
extern vedge Edges;
locale loc;
bool allowKruskal = true;

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

Edge::Edge(string newStart, string newEnd, string newName, double newDistance)
{
	start = newStart;
	end = newEnd;
	name = newName;
	distance = newDistance;
}

Vertex::Vertex(string newName, double newWeight)
{
	name = newName;
	weight = newWeight;
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
vstring split(string input)
{
	string temp;
	for (unsigned int i = 0; i < input.length(); i++) // Convert input into all upper case
		temp += toupper(input[i], loc);
	istringstream buf(temp);
	istream_iterator<string> beg(buf), end;

	vstring output(beg, end);

	return output;
}

// Get possible file path for .csv file (So that its case does not get changed)
string getFIlePath(string input)
{
	istringstream buf(input);
	istream_iterator<string> beg(buf), end;

	vstring output(beg, end);

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
	vstring splitCommands;
	#pragma endregion

	filePath = getFIlePath(command); // Get file path
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
		else if (splitCommands.size() > 6)
		{
			cout << red << "\a*** ERROR *** TOO MANY PARAMETERS" << endl << def;
			NEWLINE
		}
		else if (splitCommands.at(1) == "ADD") // Add edge
		{
			if (splitCommands.size() < 6) // Verify parameter count
				cout << red << "\a*** ERROR *** MISSING PARAMETERS" << endl << def;
			else if (splitCommands.size() > 6)
				cout << red << "\a*** ERROR *** TOO MANY PARAMETERS" << endl << def;
			else if (edgeExists(splitCommands.at(2), splitCommands.at(3))) // Update edge
				edgeUpdate(splitCommands.at(2), splitCommands.at(3), splitCommands.at(4), atof(splitCommands.at(5).c_str()));
			else if (!nodeExists(splitCommands.at(2)) && !nodeExists(splitCommands.at(3))) // Error if start and end nodes don't exist
			{
				cout << red << "\a*** ERROR *** NODE " << def << splitCommands.at(2) << red << ": NOT FOUND" << endl << def;
				cout << red << "\a*** ERROR *** NODE " << def << splitCommands.at(3) << red << ": NOT FOUND" << endl << def;
			}
			else if (!nodeExists(splitCommands.at(2))) // Error if start node doesn't exist
				cout << red << "\a*** ERROR *** NODE " << def << splitCommands.at(2) << red << ": NOT FOUND" << endl << def;
			else if (!nodeExists(splitCommands.at(3))) // Error if end node doesn't exist
				cout << red << "\a*** ERROR *** NODE " << def << splitCommands.at(3) << red << ": NOT FOUND" << endl << def;
			else if (!isNumber(splitCommands.at(5))) // Error if distance is not a number
				cout << red << "\a*** ERROR *** DISTANCE INVALID: " << def << splitCommands.at(5) << endl;
			else
				edgeAdd(splitCommands.at(2), splitCommands.at(3), splitCommands.at(4), atof(splitCommands.at(5).c_str()));
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

	#pragma region Kruskal Handling
	else if (splitCommands.front() == "KRUSKAL")
	{
		if (splitCommands.size() > 1) // Verify parameter count
			cout << red << "\a*** ERROR *** TOO MANY PARAMETERS" << endl << def;
		else if (Edges.empty()) // Make sure there are edges in the program
			cout << red << "\a*** ERROR *** NO EDGES PRESENT" << endl << def;
		else if (!allowKruskal) // Make sure no edges were deleted
			cout << red << "\a*** ERROR *** Krushkal's Algorithm is only available for UNDIRECTED graphs" << def << endl;
		else // Print kruskal adjacency list
			printKruskal();
		NEWLINE
	}
	#pragma endregion

	#pragma region Route Handling
	else if (splitCommands.front() == "DISTANCE")
	{
		if (splitCommands.size() < 3)
		{
			cout << red << "\a*** ERROR *** MISSING PARAMETERS" << endl << def;
			NEWLINE
		}
		else if (splitCommands.size() > 3)
		{
			cout << red << "\a*** ERROR *** TOO MANY PARAMETERS" << endl << def;
			NEWLINE
		}
		else if (Edges.empty())
		{
			cout << red << "\a*** ERROR *** NO EDGES FOUND" << endl << def;
			NEWLINE
		}
		else if (!nodeExists(splitCommands.at(1)) && !nodeExists(splitCommands.at(2)))
		{
			cout << red << "\a*** ERROR *** NODE NOT FOUND: " << def << splitCommands.at(1) << endl;
			cout << red << "\a*** ERROR *** NODE NOT FOUND: " << def << splitCommands.at(2) << endl;
			NEWLINE
		}
		else if (!nodeExists(splitCommands.at(1)))
		{
			cout << red << "\a*** ERROR *** NODE NOT FOUND: " << def << splitCommands.at(1) << endl;
			NEWLINE
		}
		else if (!nodeExists(splitCommands.at(2)))
		{
			cout << red << "\a*** ERROR *** NODE NOT FOUND: " << def << splitCommands.at(2) << endl;
			NEWLINE
		}
		else
		{
			route(splitCommands.at(1), splitCommands.at(2));
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
		else if (splitCommands.at(1) == "EDGES") // Print list
		{
			printEdges();
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

	#pragma region Clear Data
	else if (splitCommands.front() == "CLEAN")
	{
		NEWLINE
		cout << "\a\tAre you sure you want to " << red << "delete" << def << " all program data? (y/n): " << flush;
		string response;
		bool run = true;
		while (run)
		{
			getline(cin, response);
			NEWLINE
			string temp;
			for (unsigned int i = 0; i < response.length(); i++)
				temp += toupper(response[i], loc);
			if (temp == "Y" || temp == "YES")
			{
				Nodes.clear();
				Edges.clear();
				cout << "All data deleted successfully" << endl;
				run = false;
			}
			else if (temp == "N" || temp == "NO")
			{
				cout << "Cleaning canceled" << endl;
				run = false;
			}
			else
			{
				cout << red << "\a*** ERROR *** INVALID RESPONSE" << def << endl;
				NEWLINE
				cout << "\a\tAre you sure you want to " << red << "delete" << def << " all program data? (y/n): " << flush;
			}
		}
	}
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
				for (int j = 0; j < Edges.size(); j++) // Look for edges containing node
				{
					if (Edges.at(j).start != Edges.at(j).end) // Edge does not start and end at same node
					{
						if (Edges.at(j).start == name)
						{
							cout << blue << "    EDGE AUTO-REMOVED BY NODE DELETION: " << def << Edges.at(j).start << "-" << Edges.at(j).end << endl;
							Edges.erase(Edges.begin() + j);
						}
						if (!Edges.empty() && j != Edges.size())
						{
							if (Edges.at(j).end == name || Edges.at(j).start == name)
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
void edgeAdd(string start, string end, string name, double distance)
{
	Edge temp1 = Edge(start, end, name, distance);
	Edges.push_back(temp1);
	cout << green << "ADDED" << def << ": EDGE " << start << "-" << end << " Name: " << name << " Distance: " << distance << endl;

	if (start.compare(end) != 0)
	{
		Edge temp2 = Edge(end, start, name, distance);
		Edges.push_back(temp2);
		cout << green << "ADDED" << def << ": EDGE " << end << "-" << start << " Name: " << name << " Distance: " << distance << endl;
	}
}

// Update existing edge
void edgeUpdate(string start, string end, string name, double distance)
{
	for (unsigned int i = 0; i < Edges.size(); i++)
	{
		if (Edges.at(i).start == start && Edges.at(i).end == end)
		{
			Edges.at(i).name = name;
			Edges.at(i).distance = distance;
			cout << "EDGE " << start << "-" << end << " and " << end << "-" << start << green << " UPDATE" << def << flush;
			cout << ": NAME: " << blue << name << def << " DIST: " << blue << distance << def << endl;
		}
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
			allowKruskal = false;
		}
		// The following commented out code ensures an undirected graph removing both directions of an edge.
		/*
		if (!Edges.empty())
		{
		if (Edges.at(i).start == end && Edges.at(i).end == start)
		{
		Edges.erase(Edges.begin() + i);
		cout << red << "DELETED" << def << ": EDGE " << end << "-" << start << endl;
		i -= 1;
		}
		}
		*/
	}
}

bool isNumber(string distance)
{
	bool decimal = false;
	for (unsigned int i = 0; i < distance.length(); i++)
	{
		if (!isdigit(distance.at(i)))
		{
			if (distance.at(i) == '.' && decimal == false)
				decimal = true;
			else
				return false;
		}
	}
	return true;
}

Edge getEdge(string a, string b)
{
	for (unsigned int i = 0; i < Edges.size(); i++)
	{
		if (Edges.at(i).start == a && Edges.at(i).end == b)
			return Edges.at(i);
	}
	return Edge("start", "end", "N/A", 0);
}
#pragma endregion

#pragma region Kruskal Functions
vedge kruskalAlgorithm()
{
	#pragma region Variables
	vedge sortedEdges;
	vedge kruskals;
	vstring kruskalNodes;
	sortedEdges = Edges;
	#pragma endregion

	#pragma region Sort edges
	// Remove loops and duplicate edges
	for (unsigned int i = 0; i < sortedEdges.size(); i++)
	{
		if (sortedEdges.at(i).start == sortedEdges.at(i).end)
		{
			sortedEdges.erase(sortedEdges.begin() + i);
			i -= 1;
		}
		if (i < sortedEdges.size() - 1)
		{
			if (sortedEdges.at(i).start == sortedEdges.at(i + 1).end && sortedEdges.at(i).end == sortedEdges.at(i + 1).start)
				sortedEdges.erase(sortedEdges.begin() + i + 1);
		}
	}

	// Sort edges by distance, least to greatest
	sort(sortedEdges.begin(), sortedEdges.end(), edgeDistanceCompare);
	#pragma endregion

	#pragma region Add edges to MST
	// Add smallest edge to kruskals list and remove from sorted list
	kruskals.push_back(sortedEdges.front());
	updateKruskalNodes(sortedEdges.front(), &kruskalNodes);
	sortedEdges.erase(sortedEdges.begin());

	if (!sortedEdges.empty())
	{
		kruskals.push_back(sortedEdges.front());
		updateKruskalNodes(sortedEdges.front(), &kruskalNodes);
		sortedEdges.erase(sortedEdges.begin());

		// Add subsequent edges
		for (unsigned int i = 0; i < Nodes.size() - 3; i++)
		{
			if (!sortedEdges.empty())
			{
				if (!loopExist(sortedEdges.front(), kruskalNodes, kruskals))
				{
					kruskals.push_back(sortedEdges.front());
					updateKruskalNodes(sortedEdges.front(), &kruskalNodes);
					sortedEdges.erase(sortedEdges.begin());
				}
				else
				{
					i -= 1;
					sortedEdges.erase(sortedEdges.begin());
				}
			}
		}
	}
	#pragma endregion

	// Add duplicate edges previously removed for printing ease
	undirectKruskal(&kruskals);

	return kruskals;
}

// Compare distances of two edges
bool edgeDistanceCompare(Edge a, Edge b)
{
	if (a.distance < b.distance)
		return true;
	else
		return false;
}

// Check current edge against current nodes in kruskals for loop
bool loopExist(Edge a, vstring nodeList, vedge kruskal)
{
	for (unsigned int i = 0; i < nodeList.size(); i++)
	{
		if (a.start == nodeList.at(i)) // Check for start node in MST
		{
			for (unsigned int j = 0; j < nodeList.size(); j++)
			{
				if (a.end == nodeList.at(j)) // Check for end node in MST
				{
					vstring startTree, endTree;

					// Add all nodes linked to start and end nodes of current edge
					connectedNodes(a.start, &startTree, kruskal);
					connectedNodes(a.end, &endTree, kruskal);

					// Check for overlapping nodes
					for (unsigned int k = 0; k < startTree.size(); k++)
					{
						for (unsigned int l = 0; l < endTree.size(); l++)
						{
							if (startTree.at(k) == endTree.at(l))
								return true;
						}
					}
				}
			}
		}
	}
	return false;
}

// Find all nodes connected to a single node
void connectedNodes(string node, vstring *list, vedge kruskal)
{
	for (unsigned int k = 0; k < kruskal.size(); k++)
	{
		// If passed node is the start of an edge
		if (kruskal.at(k).start == node)
		{
			bool create = true, add = true;
			for (unsigned int i = 0; i < list->size(); i++)
			{
				// Check if the counterpoint of start node is already in list
				if (list->at(i) == kruskal.at(k).end)
					create = false;
			}
			// If counter point is not in list, add node to list and look for it's connected nodes
			if (create)
			{
				list->push_back(node);
				connectedNodes(kruskal.at(k).end, list, kruskal);
			}
		}
		// If passed node is the end of an edge
		else if (kruskal.at(k).end == node)
		{
			bool create = true;
			for (unsigned int i = 0; i < list->size(); i++)
			{
				// Check if the counterpoint of end node is already in list
				if (list->at(i) == kruskal.at(k).start)
					create = false;
			}
			// If counter point is not in list, add node to list and look for it's connected nodes
			if (create)
			{
				list->push_back(node);
				connectedNodes(kruskal.at(k).start, list, kruskal);
			}
		}
	}
}

// Update the nodes currently in the kruskal tree
void updateKruskalNodes(Edge edge, vstring *list)
{
	bool startExist = false;
	bool endExist = false;
	for (unsigned int i = 0; i < list->size(); i++)
	{
		if (edge.start == list->at(i))
			startExist = true;
		if (edge.end == list->at(i))
			endExist = true;
	}
	if (!startExist)
		list->push_back(edge.start);
	if (!endExist)
		list->push_back(edge.end);
}

// Replace all removed duplicate edges when edges was sorted
void undirectKruskal(vedge *list)
{
	unsigned int size = list->size();
	for (unsigned int i = 0; i < size; i++)
	{
		Edge temp = Edge(list->at(i).end, list->at(i).start, list->at(i).name, list->at(i).distance);
		list->push_back(temp);
	}
}

void printKruskal()
{
	// Perform Kruskal's Algorithm
	vedge kruskal = kruskalAlgorithm();
	// Alphabatize nodes temporarily
	vnode alphabetical;
	alphabetical = Nodes;
	sort(alphabetical.begin(), alphabetical.end(), nodeCompare);

	unsigned int nameLength = 0, distLength = 0;
	for (unsigned int i = 0; i < alphabetical.size(); i++)
	{
		for (unsigned int j = 0; j < alphabetical.size(); j++)
		{
			if (nameLength < getEdge(alphabetical.at(i).name, alphabetical.at(j).name).name.length())
				nameLength = getEdge(alphabetical.at(i).name, alphabetical.at(j).name).name.length();

			if (distLength < dtos(getEdge(alphabetical.at(i).name, alphabetical.at(j).name).distance).length())
				distLength = dtos(getEdge(alphabetical.at(i).name, alphabetical.at(j).name).distance).length();
		}
	}

	for (unsigned int i = 0; i < alphabetical.size(); i++)
	{
		cout << alphabetical.at(i).name << flush;
		for (unsigned int j = 0; j < alphabetical.size(); j++)
		{
			stringstream s;

			s << "(" << setw(nameLength) << left << getEdge(alphabetical.at(i).name, alphabetical.at(j).name).name << ", "
				<< setw(distLength) << right << getEdge(alphabetical.at(i).name, alphabetical.at(j).name).distance << ")";

			if (edgeExists(alphabetical.at(i).name, alphabetical.at(j).name, kruskal))
				cout << "  -" << green << s.str() << def << "->  " << alphabetical.at(j).name << flush;
		}
		NEWLINE
	}
}

bool edgeExists(string start, string end, vedge list)
{
	if (!list.empty())
	{
		for (unsigned int i = 0; i < list.size(); i++)
		{
			if (list.at(i).start == start && list.at(i).end == end)
				return true;
		}
	}
	return false;
}
#pragma endregion

#pragma region Route Functions
#pragma region Route Variables
vstring visited;
vector<Vertex> nodeCost;
#pragma endregion

// Route path handler
void route(string a, string b)
{
	if (isConnected(a, b))
	{
		vedge dijkstra, path;
		iniVertices(a);
		dijkstraAlgorithm(a, &dijkstra);
		shortestPath(a, b, dijkstra, &path);
		reversePath(&path);
		printShortestPath(getOutput(path));
	}
	else
		cout << red << "\a*** ERROR *** NO PATHS EXIST BETWEEN THOSE NODES" << def << endl << endl;
}

// Run Dijkstra's Algorithm
void dijkstraAlgorithm(string source, vedge *dijkstra)
{
	if (source == "")
		return;
	vstring adjSource = adjacentNodes(source);
	visited.push_back(source);

	for (unsigned int i = 0; i < Edges.size(); i++)
	{
		for (unsigned int j = 0; j < adjSource.size(); j++)
		{
			if (Edges.at(i).start == source && Edges.at(i).end == adjSource.at(j))
			{
				for (unsigned int k = 0; k < nodeCost.size(); k++)
				{
					if (nodeCost.at(k).name == source)
					{
						for (unsigned int l = 0; l < nodeCost.size(); l++)
						{
							if (nodeCost.at(l).name == adjSource.at(j) && nodeCost.at(l).weight >(Edges.at(i).distance + nodeCost.at(k).weight))
							{
								nodeCost.at(l).weight = Edges.at(i).distance + nodeCost.at(k).weight;
								nodeCost.at(l).path.push_back(Edges.at(i));
							}
						}
					}
				}
			}
		}
	}

	string minName;
	double minDist = (double)INT_MAX;
	for (unsigned int i = 0; i < Nodes.size(); i++)
	{
		if (!checkVisited(Nodes.at(i).name))
		{
			for (unsigned int j = 0; j < nodeCost.size(); j++)
			{
				if (nodeCost.at(j).name == Nodes.at(i).name)
				{
					if (nodeCost.at(j).weight < minDist)
					{
						minDist = nodeCost.at(j).weight;
						minName = Nodes.at(i).name;
					}
				}
			}
		}
	}
	for (unsigned int i = 0; i < nodeCost.size(); i++)
	{
		if (nodeCost.at(i).name == minName)
			dijkstra->push_back(nodeCost.at(i).path.back());
	}
	dijkstraAlgorithm(minName, dijkstra);
}

// Initialize global variables for route
void iniVertices(string source)
{
	visited.clear();
	nodeCost.clear();
	for (unsigned int i = 0; i < Nodes.size(); i++)
	{
		if (Nodes.at(i).name == source)
		{
			Vertex temp = Vertex(Nodes.at(i).name, (double)0);
			nodeCost.push_back(temp);
		}
		else
		{
			Vertex temp = Vertex(Nodes.at(i).name, (double)INT_MAX);
			nodeCost.push_back(temp);
		}
	}
}

// Find adjacent nodes to passed node in Edges
vstring adjacentNodes(string node)
{
	vstring adjList;
	for (unsigned int i = 0; i < Edges.size(); i++)
	{
		if (Edges.at(i).start == node)
			adjList.push_back(Edges.at(i).end);
	}
	return adjList;
}

// Find adjacent nodes to passed node in passed list
vstring adjacentNodes(string node, vedge dijkstra)
{
	vstring adjList;
	for (unsigned int i = 0; i < dijkstra.size(); i++)
	{
		if (dijkstra.at(i).start == node)
			adjList.push_back(dijkstra.at(i).end);
	}
	return adjList;
}

// Check if passed node has already been visited
bool checkVisited(string a)
{
	for (unsigned int i = 0; i < visited.size(); i++)
	{
		if (visited.at(i) == a)
			return true;
	}
	return false;
}

// Find the shortest path between the two nodes
void shortestPath(string start, string end, vedge list, vedge *returnList)
{
	for (unsigned int j = 0; j < list.size(); j++)
	{
		if (list.at(j).end == end)
		{
			returnList->push_back(list.at(j));
			if (list.at(j).start != start)
				shortestPath(start, list.at(j).start, list, returnList);
		}
	}
}

// Reverse the order of the edges
void reversePath(vedge *path)
{
	vedge copy = *path, returnPath;

	unsigned int size = copy.size();
	for (unsigned int i = 0; i < size; i++)
	{
		returnPath.push_back(copy.back());
		copy.erase(copy.end() - 1);
	}
	path->clear();
	*path = returnPath;
}

// Get the output to print
string getOutput(vedge path)
{
	vstring temp;
	double totalDistance = 0;
	unsigned int width = 0, maxDistLength = 0, maxNodeLength = 0, maxStreetLength = 0;

	for (unsigned int i = 0; i < path.size(); i++)
	{
		if (maxDistLength < dtos(path.at(i).distance).length())
			maxDistLength = dtos(path.at(i).distance).length();
		if (maxNodeLength < (path.at(i).start.length() + path.at(i).end.length() + 1))
			maxNodeLength = (path.at(i).start.length() + path.at(i).end.length() + 1);
		if (maxStreetLength < path.at(i).name.length())
			maxStreetLength = path.at(i).name.length();
	}

	for (unsigned int i = 0; i < path.size(); i++)
	{
		stringstream s;
		stringstream nodes;
		nodes << path.at(i).start << "-" << path.at(i).end;
		s << "       " << setw(maxNodeLength) << left << nodes.str() << " Name: " << setw(maxStreetLength) << left << path.at(i).name << " Distance: " << setw(maxDistLength) << right << path.at(i).distance;
		totalDistance += path.at(i).distance;
		if (width < s.str().length())
			width = s.str().length();
		temp.push_back(s.str());
	}
	stringstream output;
	for (unsigned int i = 0; i < temp.size(); i++)
	{
		if (i == 0)
			output << green << "Start: " << def << path.at(i).start << endl;
		output << setw(width) << setfill('-') << "" << setfill(' ') << endl;
		output << setw(width) << left << temp.at(i) << endl;
		if (i == temp.size() - 1)
		{
			output << setw(width) << setfill('-') << "" << setfill(' ') << endl;
			stringstream totalDistanceString;
			totalDistanceString << "Total Distance: " << totalDistance;
			output << green << "  End: " << def << path.at(i).end << setw(width - (7 + path.at(i).end.length())) << right << totalDistanceString.str() << endl;
		}
	}
	return output.str();
}

// Print the output
void printShortestPath(string s)
{
	NEWLINE
		cout << s;
	NEWLINE
}

// Make sure there is a connection between the start and end node
bool isConnected(string a, string b)
{
	vstring connections;

	shortestConnections(a, &connections);
	
	for (unsigned int i = 0; i < connections.size(); i++)
	{
		if (connections.at(i) == b)
			return true;
	}
	return false;
}

// Find the shortest connection
void shortestConnections(string node, vstring *list)
{
	for (unsigned int k = 0; k < Edges.size(); k++)
	{
		// If passed node is the start of an edge
		if (Edges.at(k).start == node)
		{
			bool cont = true, add = true;
			for (unsigned int i = 0; i < list->size(); i++)
			{
				// Check if the counterpoint of start node is already in list
				if (list->at(i) == Edges.at(k).end)
					cont = false;
				if (list->at(i) == node)
					add = false;
			}
			// If counter point is not in list, add node to list and look for it's connected nodes
			if (add)
				list->push_back(node);
			if (cont)
				shortestConnections(Edges.at(k).end, list);
		}
	}
}
#pragma endregion

#pragma region Print Functions
// Print Adjacency Matrix
void printMatrix()
{
	// Alphabatize nodes temporarily
	vnode alphabetical;
	alphabetical = Nodes;
	sort(alphabetical.begin(), alphabetical.end(), nodeCompare);

	pair<int, int> width = getW();

	cout << setw(getW().first + 1) << " " << flush;
	for (unsigned int i = 0; i < alphabetical.size(); i++)
		cout << left << blue << setw(getW().second + 1) << alphabetical.at(i).name << def << flush;
	NEWLINE

	unsigned int nameLength = 0, distLength = 0;
	for (unsigned int i = 0; i < alphabetical.size(); i++)
	{
		for (unsigned int j = 0; j < alphabetical.size(); j++)
		{
			if (nameLength < getEdge(alphabetical.at(i).name, alphabetical.at(j).name).name.length())
				nameLength = getEdge(alphabetical.at(i).name, alphabetical.at(j).name).name.length() - 1;

			if (distLength < dtos(getEdge(alphabetical.at(i).name, alphabetical.at(j).name).distance).length())
				distLength = dtos(getEdge(alphabetical.at(i).name, alphabetical.at(j).name).distance).length();
		}
	}

	for (unsigned int i = 0; i < alphabetical.size(); i++)
	{
		cout << left << blue << setw(getW().first + 1) << alphabetical.at(i).name << def << flush;
		for (unsigned int j = 0; j < alphabetical.size(); j++)
		{
			stringstream s;

			s << "(" << setw(nameLength) << left << getEdge(alphabetical.at(i).name, alphabetical.at(j).name).name << ", "
				<< setw(distLength) << right << getEdge(alphabetical.at(i).name, alphabetical.at(j).name).distance << ")";
			if (edgeExists(alphabetical.at(i).name, alphabetical.at(j).name))
				cout << green << left << setw(getW().second + 1) << s.str() << def << flush;
			else
				cout << red << left << setfill('-') << setw(getW().second) << "" << " " << setfill(' ') << def << flush;
		}
		NEWLINE
	}
}

// Print adjacency list
void printList()
{
	// Alphabatize nodes temporarily
	vnode alphabetical;
	alphabetical = Nodes;
	sort(alphabetical.begin(), alphabetical.end(), nodeCompare);

	unsigned int nameLength = 0, distLength = 0;
	for (unsigned int i = 0; i < alphabetical.size(); i++)
	{
		for (unsigned int j = 0; j < alphabetical.size(); j++)
		{
			if (nameLength < getEdge(alphabetical.at(i).name, alphabetical.at(j).name).name.length())
				nameLength = getEdge(alphabetical.at(i).name, alphabetical.at(j).name).name.length();

			if (distLength < dtos(getEdge(alphabetical.at(i).name, alphabetical.at(j).name).distance).length())
				distLength = dtos(getEdge(alphabetical.at(i).name, alphabetical.at(j).name).distance).length();
		}
	}

	for (unsigned int i = 0; i < alphabetical.size(); i++)
	{
		cout << alphabetical.at(i).name << flush;
		for (unsigned int j = 0; j < alphabetical.size(); j++)
		{
			stringstream s;

			s << "(" << setw(nameLength) << left << getEdge(alphabetical.at(i).name, alphabetical.at(j).name).name << ", "
				<< setw(distLength) << right << getEdge(alphabetical.at(i).name, alphabetical.at(j).name).distance << ")";

			if (edgeExists(alphabetical.at(i).name, alphabetical.at(j).name))
				cout << "  -" << green << s.str() << def << "->  " << alphabetical.at(j).name << flush;
		}
		NEWLINE
	}
}

void printEdges()
{
	// Alphabatize edges temporarily
	vedge alphabetical;
	alphabetical = Edges;
	sort(alphabetical.begin(), alphabetical.end(), edgeNameCompare);

	for (unsigned int i = 0; i < alphabetical.size(); i++)
	{
		cout << blue << "  " << i + 1 << ") " << def << "Name: " << alphabetical.at(i).name << " Distance: " << alphabetical.at(i).distance << flush;
		cout << " Origin: " << alphabetical.at(i).start << " Destination: " << alphabetical.at(i).end << endl;
	}
}

// Used to alphabatize list
bool nodeCompare(Node a, Node b)
{
	if (a.name.compare(b.name) < 0)
		return true;
	else
		return false;
}

bool edgeNameCompare(Edge a, Edge b)
{
	if (a.name.compare(b.name) < 0)
		return true;
	else
		return false;
}

pair<int, int> getW()
{
	unsigned int city = 1, edge = 1;
	for (unsigned int i = 0; i < Nodes.size(); i++)
	{
		if (city < Nodes.at(i).name.length())
			city = Nodes.at(i).name.length();
	}
	for (unsigned int i = 0; i < Edges.size(); i++)
	{
		if (edge < dtos(Edges.at(i).distance).length() + Edges.at(i).name.length() + 4)
			edge = dtos(Edges.at(i).distance).length() + Edges.at(i).name.length() + 4;
	}
	return make_pair(city, edge);
}

string dtos(double a)
{
	stringstream s;
	s << a;
	return s.str();
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
	CLEAR
		cout << left << setw(58) << setfill('_') << "Nodes" << endl;
		cout << blue << "Adding a node to the program:" << endl;
		cout << green << "\tnode add <NAME>" << endl;
		cout << blue << "Deleting a node from the program:" << endl;
		cout << green << "\tnode delete <NAME>" << endl;
		cout << blue << "Searching the program for an existing node:" << endl;
		cout << green << "\tnode search <NAME>" << endl;
	NEWLINE
		cout << def << left << setw(58) << "Edges" << endl;
		cout << blue << "Adding an edge to the program:" << endl;
		cout << green << "\tedge add <START NODE> <END NODE> <NAME> <DISTANCE>" << endl;
		cout << blue << "Deleting an edge from the program:" << endl;
		cout << green << "\tedge delete <START NODE> <END NODE>" << endl;
	NEWLINE
		cout << def << left << setw(58) << "Minimum Spanning Tree" << endl;
		cout << blue << "Print the adjacency list for MST:" << endl;
		cout << green << "\tkruskal" << endl;
	NEWLINE
		cout << def << left << setw(58) << "Shortest Path" << endl;
		cout << blue << "Print the shortest path between two nodes:" << endl;
		cout << green << "\tdistance <START NODE> <END NODE>" << endl;
	NEWLINE
		cout << def << left << setw(58) << "Printing" << endl;
		cout << blue << "Print the adjacency matrix:" << endl;
		cout << green << "\tprint matrix" << endl;
		cout << blue << "Print the adjacency list:" << endl;
		cout << green << "\tprint list" << endl;
		cout << blue << "Print a list of all edges:" << endl;
		cout << green << "\tprint edges" << endl;
	NEWLINE
		cout << def << left << setw(58) << "Command File" << endl;
		cout << blue << "Using a .csv file to execute commands:" << endl;
		cout << green << "\tfile <FILE PATH>" << endl;
		cout << def << "\tNote: The file path may" << red << " NOT " << def << "contain any spaces" << endl;
	NEWLINE
		cout << left << setw(58) << "Other Commands" << endl;
		cout << blue << "Delete all current data:" << endl;
		cout << green << "\tclean" << endl;
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