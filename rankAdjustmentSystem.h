#include <iostream>
#include <vector>

using namespace std;

#ifndef RANKADJUSTMENTSYSTEM_H
#define RANKADJUSTMENTSYSTEM_H

using namespace std;

vector<int> endOfMatchMMRTally(int playerID, int playerMMR, int MMRChangeLevel,
	int winStreak, int loseStreak, bool gameWasWon, int gamesPlayed, bool streakBonusEnabled);

#endif
