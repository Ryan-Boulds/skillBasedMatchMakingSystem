#include <iostream>
#include <string>
#include <vector>

#ifndef UTILITIES_H
#define UTILITIES_H

using namespace std;

void waitForUserToClearTerminal();

int randomNumberGenerator(int lowRange, int highRange);

bool stringContainsAllNumbers(const string& str);

vector<string> separateFirstWordFromString(string originalString);

void moveCursorTo(int x, int y);

vector<string> validInputOrError(const string& parameter1, const string& parameter2, string settingsFileContents, string errorOccurred);


// Declare more functions as needed

#endif
