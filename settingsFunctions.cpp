// myFunctions.cpp

#include "settingsFunctions.h"

#include <iostream>
#include <fstream> 
#include <sstream>  
#include <string>   
#include <vector>
#include <algorithm>
#include <cctype>


//my libraries
#include "utilities.h"


using namespace std;

// default settings are set here:
void createDefaultSettingsFile() {

	ofstream createSettingsFile("settings.txt");
	//information
	createSettingsFile << "Index: bool = b, int = i\n";
	//Default settings can be put here:
	createSettingsFile << "b antiboosting = 1 " << endl;
	createSettingsFile << "b randomstartingmmr = 0 " << endl;
	createSettingsFile << "b ranklossprevention = 1 " << endl;
	createSettingsFile << "b streakbonus = 1 " << endl;
	createSettingsFile << "b matchmakespeed = 1 " << endl;
	createSettingsFile << "i mmrlimit = 700 " << endl;
	createSettingsFile << "i squads = 1000 " << endl;
	createSettingsFile << "i initialrange = 25 " << endl;
	createSettingsFile << "i initialmmr = 2100 " << endl;
	createSettingsFile << "i games = 8500 " << endl;
	createSettingsFile << "i ranklosspreventiongames = 75 " << endl;

	// Usage: pullVariableFromSettings(setting name)
	//example" pullVariableFromSettings("ranklossprevention")

	// make sure you run "set reset" before running with a new variable to update the settings

	createSettingsFile.close();
}


void settingsUsageList() {

	cout << "Settings List:\n"
		<< "antiboosting <true/false>\n"
		<< "randomstartingmmr <true/false>\n"
		<< "ranklossprevention <true/false>\n"
		<< "streakbonus <true/false>\n"
		<< "matchmakespeed <true/false> \n";


	cout << "\nantiboosting <integer>\n"
		<< "initialmmr <integer>\n"
		<< "initialrange <integer>\n"
		<< "mmrlimit <integer>\n"
		<< "ranklossprevention <integer>\n"
		<< "ranklosspreventiongames <integer>\n"
		<< "squads <integer>\n"
		<< "streakbonus <integer>\n\n";


}


void listOfSimulations() {

	cout << "simulate <option> - Run simulations." << endl;
	cout << "     <help> - Show help with simulations." << endl;
	cout << "     <normal> or <standard> - Run normal simulation." << endl;
	cout << "     <singleplayer> -Run simulation for single games." << endl;
	cout << "     <slow> - This will simulate one match at a time." << endl;

}

void listOfSimulationDemos() {
	cout << "simulatedemo <#> -This will test the matchmaking algorithm." << endl;
	cout << "     help - Show help with simulationdemos." << endl;
	cout << "     <0> - Manually test wins and losses entering 1 for wins and 0 for losses." << endl;
	cout << "     <1> - Shows individual player data results from randomly generated match results. Win rates are 50%." << endl;
	cout << "     <2> - Same as test 1, but tallies final results over time with 10,000 players per cycle.\n\n";

}

void setOptions() {
	cout << "set <parameter> <value>\n"
		<< "     <settings> See the current settings\n"
		<< "     <help> - displays Help for how to use set.\n"
		<< "     <list> - Show the settings list and useage.\n"
		<< "     <reset> - Resets settings back to the default with confirmation.\n"
		<< "     <variable_name> <integer> - If the variable is an i value, you can change the integer value.\n"
		<< "     <variable_name> <true/false or 1/0> - If the variable is an b value, you can change the boolean value.\n\n";
		
}

//This function shows information to the user by showing a list of commands to control the program.
void listOfMainCommands() {
	cout << "List of commands:\n\n"
		<< "averagemmr - Find the average MMR of a squad\n"
		<< "clear - Clear the console.\n"
		<< "close - This will close the window and end the program.\n"
		<< "help - Show the list of commands\n"
		<< "settings - This will let you see what the value of each setting holds.\n";
	setOptions();
	listOfSimulations();
	listOfSimulationDemos();
	cout << "testingsql - This command was not completed for the senior project due date.\n\n";
}








// This function calls the settings.txt file and copies it to a string.
string settingsFileContentsToString() {

	//Copy settings file to string.
	ifstream copySettingsToString;
	copySettingsToString.open("settings.txt"); //open the input file
	//Copy the contents of the settings file to a string for changing the settings.
	stringstream strStream;
	strStream << copySettingsToString.rdbuf(); //read the file
	string settingsFileContents = strStream.str(); //str holds the content of the file
	copySettingsToString.close();

	return settingsFileContents;
}


// This function searches the settings.text file for a setting, then it will retrieve the value stored in that setting's variable.  This has problems when mmrlimit is 2 digits
int pullVariableFromSettings(const string& settingName) {

	//This assumes that the setting exists:
	string settings = settingsFileContentsToString();
	string outputString;
	int indexPosition;
	indexPosition = settings.find(settingName);
	int offset = settingName.length();
	int i = 0;

	bool keepLooping = true;
	while (settings[indexPosition + offset + 3 + i] != '\n' && keepLooping) {

		outputString = outputString + settings[indexPosition + offset + 3 + i];

		if (!stringContainsAllNumbers(outputString)) {

			keepLooping = false;
		}
		i++;
	}


	int intResult = stoi(outputString);
	return intResult;
}


// This runs anytime that the program wants to confirm if the settings file exists.
int checkIfSettingsFileExists() {

	//The settings file contents are copied to the string: settingsFileContents
	ifstream settingsFileExists;
	settingsFileExists.open("settings.txt");
	if (!settingsFileExists) {

		//create a settings file
		createDefaultSettingsFile();
	}
	settingsFileExists.close();
	return 0;
}


// This tells the program what kind of setting it is going to change.  Example: It is true/false, or a int variable
char settingVariableType(const string& settingName, string settingsFileContents) {

	//Index: bool = b, int = i, double = d, char = c, string = s
	char variableType;
	int indexPosition;
	indexPosition = settingsFileContents.find(settingName);
	int cnt = settingName.length();
	variableType = settingsFileContents[indexPosition - 2];

	return variableType;
}


// This writes the new value for a variable into the settings
string changeSetting(const string& settingName, string settingsFileContents, string newValue) {
	int indexPosition;
	indexPosition = settingsFileContents.find(settingName);
	int cnt = settingName.length();

	// Find the start of the current value
	int startOfCurrentValue = settingsFileContents.find_first_not_of(' ', indexPosition + cnt + 3);

	// Find the end of the current value
	int endOfCurrentValue = settingsFileContents.find_first_of(" \n", startOfCurrentValue);

	// Replace the current value with the new value
	settingsFileContents.replace(startOfCurrentValue, endOfCurrentValue - startOfCurrentValue, newValue);

	return settingsFileContents;
}


// This is the function that is used for when settings are changed.
int setSettings(string userInput) {
	// Check if userInput is empty
	if (userInput.empty()) {
		cout << "Error: Invalid input." << endl;
		settingsUsageList();
		return 1; // Return an error code indicating empty input
	}

	string parameter1, parameter2;
	string errorOccurred = "No Error";

	// Create settings file if it does not already exist
	checkIfSettingsFileExists();

	// Get settings file contents
	string settingsFileContents = settingsFileContentsToString();

	// Separate userInput into parameter1 and parameter2
	vector<string> separatedStrings = separateFirstWordFromString(move(userInput));
	parameter1 = separatedStrings[0];
	parameter2 = separatedStrings[1];

	// If the setting exists, change setting or error for invalid input
	if (settingsFileContents.find(parameter1) != string::npos) {
		vector<string> output = validInputOrError(parameter1, parameter2, settingsFileContents, errorOccurred);
		settingsFileContents = output[0];
		errorOccurred = output[1];
	}
	// If user wants to reset settings
	else if (parameter1 == "reset") {
		string resetConfirmation;
		cout << "Are you sure that you want to revert all settings back to defaults? y/n" << endl;
		cin >> resetConfirmation;
		transform(resetConfirmation.begin(), resetConfirmation.end(), resetConfirmation.begin(), ::tolower);

		if (resetConfirmation == "y" || resetConfirmation == "yes") {
			createDefaultSettingsFile();
			cout << "\nThe settings were reverted back to default." << endl;
		}
		else {
			cout << "\nThe settings were not changed back to default." << endl;
		}
		cin.ignore(); // Clear input buffer
	}
	// If user asks for help
	else if (parameter1 == "help") {
		settingsUsageList();
		return 2;
	}
	else {
		errorOccurred = "Error";
	}

	// Check for errors in input parameters
	if (parameter1 != "reset" && errorOccurred != "Error") {
		// Save by replacing old settings file with updated settings in String
		ofstream updateSettingsFile("settings.txt");
		updateSettingsFile << settingsFileContents;
		updateSettingsFile.close();
	}

	// If an invalid input or help is requested, provide assistance
	if (errorOccurred == "Error" || parameter1 == "help") {
		if (errorOccurred == "Error") {
			cout << "Error!\n" << endl;
		}
		settingsUsageList();
		errorOccurred = "No Error";
	}

	// Output settings file contents to show what the settings.txt file says
	system("cls");
	cout << "Settings:\n" << settingsFileContentsToString() << endl;

	return 0;
}




void setOptions(string command) {

	string theRestOfCommand = command;
	if (theRestOfCommand.empty()) {
		setOptions();
		cout << "\nset ";
		cin >> theRestOfCommand;
	}



	if (theRestOfCommand == "help" || theRestOfCommand == "") {
		system("cls");
		setOptions();
	}
	else if (theRestOfCommand == "list") {
		system("cls");
		settingsUsageList();
	}
	else if (theRestOfCommand == "currentsettings") {
		system("cls");
		cout << settingsFileContentsToString() << endl;
	}
	else {
		system("cls");
		// Change the settings
		setSettings(theRestOfCommand);
	}




}
