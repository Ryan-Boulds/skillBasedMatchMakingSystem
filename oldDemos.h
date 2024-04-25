#include <vector>

using namespace std;


#ifndef OLDDEMOS_H
#define OLDDEMOS_H

int findAverageSquadMMRDemo(bool antiBoosting);

vector<int> endOfMatchMMRTallyDemo(int playerMMR, int MMRChangeLevel, int winStreak, int loseStreak, bool gameWasWon,
	int doLongTest, int gamesPlayed, bool rankLossPrevention, bool streakBonusEnabled);


void simulationSimulationsSimpleDemo(int whichTest, bool rankLossPrevention, bool streakBonusEnabled);

int squadTeamSetUp();

void soloQueuingTeamSetup();

vector<vector<int>> findPlayersForLobby(vector<vector<vector<int>>> squadsInQue);

void handleSimulateDemoCommand(const string& command, bool rankLossPrevention, bool streakBonusEnabled);

#endif
