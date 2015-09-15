/*
CSCE 2110.001 Group H
Jesse Boswell
Craig Nafus
Daniel Lee

*/

#pragma region Includes
#include "functions.h"
using namespace std;
#pragma endregion Add all other includes to functions.h

// Try to do as much as possible in functions
void main()
{
	#pragma region Variables
	int promptChoice = 0;
	bool quit = false;
	#pragma endregion

	CLEAR

	while (quit == false)
	{
		promptChoice = prompt();

		// Execute command from prompt (This may move to it's own function later). It is just a template right now.
		switch (promptChoice)
		{
			case 1: // Choice 1
			{
				break;
			}
			case 2: // Choice 2
			{
				
				break;
			}
			default:
			{
				quit = true;
				break;
			}
		}
	}
	cout<< "Hello" << endl;
	KEYPRESS
} 
