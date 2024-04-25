#include <iostream>
#include <string>
#include <random> 
#include <vector>
#include <windows.h>
#include <cctype>
#include <conio.h>

// my libraries
#include "settingsFunctions.h"


using namespace std;

// This is used to clear console after the commands finish.  It is toggled by pressing any key after the command finishes.
void waitForUserToClearTerminal() {
	cout << "\nPress any key to continue . . .";
	_getch();  // Waits for a key press
	system("cls");
}


//This is a better approch to generating random numbers than rand()
int randomNumberGenerator(int lowRange, int highRange) {
	random_device rd; // obtain a random number from hardware
	mt19937 gen(rd()); // seed the generator
	uniform_int_distribution<> distr(lowRange, highRange); // define the range

	return distr(gen);
}

// This is to prevent errors in case a string is supposed to contain all numbers with the only ASCII values being from the numbers 0-9
bool stringContainsAllNumbers(const string& str) {
	bool allNumbers = true;
	for (char ch : str) {
		if (!(ch >= '0' && ch <= '9')) {
			allNumbers = false;
		}
	}
	return allNumbers;
}


// This is a useful function that takes a string, searches for a whitespace, then separates the first word from the rest of the string.
vector<string> separateFirstWordFromString(string originalString) {

	//Usage:
	//vector<string> seperatedStrings = separateFirstWordFromString(userInput);
	//FIRSTWORD = seperatedStrings[0];
	//THERESTOFTHESTRING = seperatedStrings[1];

	string firstWord, restOfString;
	int i = 0;
	while (i < originalString.length() && originalString[i] != ' ' && originalString.length() > 0) {
		firstWord = firstWord + originalString[i];
		i++;
	}

	if (originalString.length() > i) {
		restOfString = originalString.substr(i + 1);
	}

	return { firstWord, restOfString };
}

//moves the curser of the command window for the progress bar
void moveCursorTo(int x, int y) {
	COORD pos = { x, y };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
}

// If the input is valid, it will change the variable.  This is only programmed for Boolean and Integers at the time being 3-16-24.
vector<string> validInputOrError(const string& parameter1, const string& parameter2, string settingsFileContents, string errorOccurred) {

	char variableType = settingVariableType(parameter1, settingsFileContents);

	//Index: bool = b, int = i, double = d, char = c, string = s
	if (variableType == 'b') {

		// True, False, or invalid
		if (parameter2 == "true" || parameter2 == "1" || parameter2 == "enable") {

			//parameter2 true
			settingsFileContents = changeSetting(parameter1, settingsFileContents, "1");
		}
		else if (parameter2 == "false" || parameter2 == "0" || parameter2 == "disable") {

			//parameter2 false
			settingsFileContents = changeSetting(parameter1, settingsFileContents, "0");
		}
		else {

			//parameter2 ????
			errorOccurred = "Error";
		}
	}
	else if (variableType == 'i') {
		bool allNumbers = stringContainsAllNumbers(parameter2);

		if (allNumbers) {
			//parameter2 ###
			settingsFileContents = changeSetting(parameter1, settingsFileContents, parameter2);
		}
		else {
			//parameter2 ????
			errorOccurred = "Error!";
		}
	}


	vector<string> result = { settingsFileContents, errorOccurred };

	return result;
}


