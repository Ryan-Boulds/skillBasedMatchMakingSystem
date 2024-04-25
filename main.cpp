//Made by Ryan Boulds - lasted modified 2/25/2024

#include <iostream>
#include <vector>
#include <algorithm>
#include <algorithm>
#include <cctype>
#include <conio.h>

// My libraries
#include "utilities.h"
#include "oldDemos.h"
#include "mmrDetails.h"
#include "settingsFunctions.h"
#include "rankAdjustmentSystem.h"
#include "whichTeamWins.h"
#include "findAveAndAntiBoosting.h"
#include "createPlayerData.h"
#include "matchmaking.h"
#include "barChart.h"
#include "testDataBase.h"
#include "musicThemes.h"

using namespace std;








// Main function where you can control the program and what functions are run
int main() {
    // makes sure that settings.txt exists. If it does not, it will make the file with the default settings.
    checkIfSettingsFileExists();
    bool infiniteLoop = true;
    bool errorPreventionNeeded = false;
    while (infiniteLoop) {
        string command;
        vector<string> separatedStrings;
        string firstWordOfCommand;
        string theRestOfCommand;

        // I've had an issue with getline that makes it where the loop will repeat when I do not want it to.
        if (errorPreventionNeeded) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        errorPreventionNeeded = false;

        //helpful information"
        cout << "Type \"help\" to see the commands available" << endl;

        // Get input from the user
        cout << "What command would you like to run? ";
        getline(cin, command);

        // Makes user input all lower case
        transform(command.begin(), command.end(), command.begin(), ::tolower);

        //This processes the input so that the first word is separated from the rest of the string so that it can be used for going into sub commands.
        separatedStrings = separateFirstWordFromString(command);
        firstWordOfCommand = separatedStrings[0];
        theRestOfCommand = separatedStrings[1];

        // Functions are linked based off of which command was entered by the user
        if (firstWordOfCommand == "averagemmr") {
            system("cls");
            // Calculate the average MMR of a squad.  This function is mostly for demo purposes.
            cout << "Matchmaking will place you in: " << findAverageSquadMMRDemo(pullVariableFromSettings("antiboosting")) << " MMR." << endl;
            waitForUserToClearTerminal();
        }
        else if (firstWordOfCommand == "clear") {
            // Clear settings
            system("cls");
        }
        else if (firstWordOfCommand == "close" || firstWordOfCommand == "end") {
            // This will end the loop thus closing the program
            infiniteLoop = false;
        }
        else if (firstWordOfCommand == "help") {
            system("cls");
            // This will print the commands available
            listOfMainCommands();
        }
        else if (firstWordOfCommand == "settings") {
            // This will print the settings
            system("cls");
            cout << settingsFileContentsToString() << endl;
        }
        else if (firstWordOfCommand == "set") {
            system("cls");
            setOptions(theRestOfCommand);
            errorPreventionNeeded = true;
        }
        else if (firstWordOfCommand == "simulatedemo") {
            system("cls");
            //This is the old simulation, but it does have some purpose for demos
            handleSimulateDemoCommand(theRestOfCommand, pullVariableFromSettings("ranklossprevention"), pullVariableFromSettings("streakbonus"));
            errorPreventionNeeded = true;
        }
        else if (firstWordOfCommand == "simulate") {
            system("cls");
            //This is the main simulation
            handleSimulateSubCommand(theRestOfCommand, pullVariableFromSettings("ranklossprevention"), pullVariableFromSettings("streakbonus"));
            errorPreventionNeeded = true;

        }
        else if (firstWordOfCommand == "testingsql") {
            // This was not able to be completed for the senior project due date.
            system("cls");
            cout << "\nI was not able to get this done, as I ran out of time.\nThis is just for figuring out how to write to a database in C++.\n\n";
            cout << "The code to even enter the function for sql is commented out since it will just crash unless you type '0'";
            //cout << testMain();
            waitForUserToClearTerminal();
        }
        else if (std::all_of(command.begin(), command.end(), [](char c) { return std::isspace(c); })) {
            // Clear console to fix glitch caused by empty input. It might be fixed, but I have not been able to test it yet. Date: 4/24/24 10:23
            system("cls");
        }
        else {
            system("cls");
            cout << "Error!!!\nInvalid input!\n";
            //Shows list of commands
            listOfMainCommands();
        }
    }

    return 0;
}
