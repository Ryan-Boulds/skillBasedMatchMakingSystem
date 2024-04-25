//settingsFunctions
#include <iostream>
#include <string>

#ifndef SETTINGSFUNCTIONS_H
#define SETTINGSFUNCTIONS_H



using namespace std;

void createDefaultSettingsFile();

void listOfSimulations();

void listOfSimulationDemos();

void listOfMainCommands();

string settingsFileContentsToString();

int pullVariableFromSettings(const string& settingName);

int checkIfSettingsFileExists();

char settingVariableType(const string& settingName, string settingsFileContents);

string changeSetting(const string& settingName, string settingsFileContents, string newValue);

int setSettings(string userInput);


// Declare more functions as needed

#endif
