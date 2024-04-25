#include <iostream>
#include <vector>
// my libraries

#include "settingsFunctions.h"

using namespace std;


// Find squad average MMR score with anti-boosting calculation.
int findAverageSquadMMR(vector<vector<int>> squad, bool removeFirstEntry) {

	if (removeFirstEntry) {

		squad.erase(squad.begin() + 0);
	}
	vector<int> squadMMR;
	squadMMR.resize(squad.size());
	for (int i = 0; i < squad.size(); i++) { squadMMR[i] = squad[i][1]; }
	int numOfPlayersInSquad = squadMMR.size();

	int averageMMR = 0;
	int adjustedAverageMMR = 0;
	int largestMMRScore = -10000000; //Just a placeholder that wont ever be met.
	int MMRLimit = pullVariableFromSettings("mmrlimit");
	bool antiBoosting = pullVariableFromSettings("antiboosting");

	//Get input from user of what the MMR of each player is.
	for (int i = 0; i < numOfPlayersInSquad; i++) {

		if (largestMMRScore < squadMMR[i] && antiBoosting) {

			largestMMRScore = squadMMR[i];
		}
	}

	//Calculate the average MMR of the squad.
	for (int i = 0; i < squad.size(); i++) {

		//Without boosting countermeasures
		averageMMR = averageMMR + squadMMR[i];

		//With boosting countermeasure
		if (squadMMR[i] > largestMMRScore - MMRLimit) {

			adjustedAverageMMR = adjustedAverageMMR + squadMMR[i];
		}
		else {

			adjustedAverageMMR = adjustedAverageMMR + (largestMMRScore - MMRLimit);
		}
	}

	if (numOfPlayersInSquad != 0) {

		averageMMR = averageMMR / numOfPlayersInSquad;
		adjustedAverageMMR = adjustedAverageMMR / numOfPlayersInSquad;
	}
	else {

		adjustedAverageMMR = 0; //error prevention while testing
	}
	//IDK whats going on here.  Ill have to look into it
	if (removeFirstEntry || antiBoosting) {

		return adjustedAverageMMR;
	}
	return averageMMR;

}