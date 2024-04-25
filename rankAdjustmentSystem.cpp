#include <iostream>
#include <vector>
#include <cmath>
//my libraries
#include "settingsFunctions.h"
#include "mmrDetails.h"

using namespace std;

//This can be cleaned up for sure.

// This is the part that calculates a player's MMR score after a game
vector<int> endOfMatchMMRTally(int playerID, int playerMMR, int MMRChangeLevel,
	int winStreak, int loseStreak, bool gameWasWon, int gamesPlayed, bool streakBonusEnabled) {
	bool rankLossPrevention = pullVariableFromSettings("ranklossprevention");
	int amountMMRChanged;
	int streakBonus = 0;
	int tempVariable = playerMMR;
	//simplifies for equation
	int x = MMRChangeLevel;

	//MMR curve that is not adjustable. limit x=110    // ( (x - 40.5) * (x - 180.5) / 123 ) + 64.9
	//This makes sure that the minimum gained/lost is 25 MMR
	//if (MMRChangeLevel > 110) { MMRChangeLevel = 110; }
	//amountMMRChanged = ((x - 40.5) * (x - 180.5) / 123) + 64.9;

	//This makes sure that the minimum gained/lost is 25 MMR
	if (MMRChangeLevel >= 75) { MMRChangeLevel = 75; }
	amountMMRChanged = -x + 100;

	if (gamesPlayed > 10 && streakBonusEnabled) {

		//Win streaks factored
		if (winStreak > 2) {

			//work on this part.
			streakBonus = (amountMMRChanged / 25) * sqrt(3 * winStreak);
		}
		else if (loseStreak > 2) {

			streakBonus = (amountMMRChanged / 25) * sqrt(3 * loseStreak);
		}
	}

	//This is where it changes the MMR level
	if (gameWasWon) {

		//End losing streak bonus
		if (loseStreak != 0)
		{
			streakBonus = 0;
		}
		loseStreak = 0; //resets lose streak to 0 after win.

		//Add MMR to player
		playerMMR = playerMMR + amountMMRChanged + streakBonus;


		winStreak++;
	}
	else {

		//if game was lost

		//End winning streak bonus
		if (winStreak != 0) {

			streakBonus = 0;
		}
		winStreak = 0; //resets win streak to 0 after loss.

		//subtract MMR to player:
		//
		//rank loss protection accounted for.

		//If player does not de-rank, MMR will be subtracted normally.
		if (rankChecker(playerMMR, MMRChangeLevel) == rankChecker(playerMMR - amountMMRChanged - streakBonus,
			MMRChangeLevel) || !rankLossPrevention || gamesPlayed > pullVariableFromSettings("ranklosspreventiongames")) {

			playerMMR = playerMMR - amountMMRChanged - streakBonus;

		}
		else {

			//If de-rank were to occur without rank loss prevention:
			//de-rank
			if (playerMMR % 100 == 0) {
				int i = 0;
				do
				{
					playerMMR--;
					i++;
				} while (playerMMR % 100 != 0 && i < amountMMRChanged + streakBonus);

			}
			else { //rank loss prevented
				while (playerMMR % 100 != 0) {
					playerMMR--;
				}
			}
		}
		loseStreak++;
	}

	//Work on this
	gamesPlayed++;
	//streak factored
	if (winStreak < 3 && loseStreak < 3 || gamesPlayed <= 10) {

		MMRChangeLevel++;
	}
	else if (winStreak % 3 == 0 && winStreak >= 4 || loseStreak % 3 == 0 && loseStreak >= 4) {
		MMRChangeLevel++;
	}

	//Return results
	return { playerID, playerMMR, MMRChangeLevel, winStreak, loseStreak, gamesPlayed };
}
