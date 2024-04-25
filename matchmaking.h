#include <iostream>
#include <vector>

#ifndef MATCHMAKING_H
#define MATCHMAKING_H

using namespace std;

//findPlayersForLobbyWithFastMethods
vector<vector<int>> findSquadsForLobby(vector<vector<vector<int>>> squadsInQue, bool matchmakingSpeed);

void printTeamDetails(const vector<vector<int>>& team, const string& teamName, const string& adjustmentStatus);

void updateTeamStats(vector<vector<int>>& team, bool winStatus);

void matchmakeAndTally(bool slowMode);

void handleSimulateSubCommand(const string& command, bool rankLossPrevention, bool streakBonusEnabled);

// Declare more functions as needed

#endif
