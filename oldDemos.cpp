#include <iostream>
#include <vector>
#include <windows.h>
#include <iomanip>
#include <algorithm>
#include <cctype>
// my libraries
#include "mmrDetails.h"
#include "createPlayerData.h"
#include "findAveAndAntiBoosting.h"
#include "settingsFunctions.h"
#include "settingsFunctions.h"
#include "utilities.h"

using namespace std;


//EVERYTHING IN THIS FILE IS LEGACY CODE!!! THIS CODE IS VERY MESSY AS IT WAS BUILT AND EXPANDED UPON OVERTIME WITHOUT BEING MAINTAINED!!!
//EVERYTHING IN THIS FILE IS LEGACY CODE!!! THIS CODE IS VERY MESSY AS IT WAS BUILT AND EXPANDED UPON OVERTIME WITHOUT BEING MAINTAINED!!!
//EVERYTHING IN THIS FILE IS LEGACY CODE!!! THIS CODE IS VERY MESSY AS IT WAS BUILT AND EXPANDED UPON OVERTIME WITHOUT BEING MAINTAINED!!!
//EVERYTHING IN THIS FILE IS LEGACY CODE!!! THIS CODE IS VERY MESSY AS IT WAS BUILT AND EXPANDED UPON OVERTIME WITHOUT BEING MAINTAINED!!!
//EVERYTHING IN THIS FILE IS LEGACY CODE!!! THIS CODE IS VERY MESSY AS IT WAS BUILT AND EXPANDED UPON OVERTIME WITHOUT BEING MAINTAINED!!!


// Find squad average MMR score with anti-boosting calculation.  //This is more for demo purposes now
int findAverageSquadMMRDemo(bool antiBoosting) {

	int numOfPlayersinSquad = 1;
	int playerMMRinput;
	int averageMMR = 0;
	int adjustedAverageMMR = 0;
	int largestMMRScore = -10000000; //Just a placeholder that wont ever be met.
	int MMRLimit = 700;
	vector<int> teamMMR{};

	cout << "Input number of players in squad: ";
	cin >> numOfPlayersinSquad;

	//Get input from user of what the MMR of each player is.
	for (int i = 0; i < numOfPlayersinSquad; i++) {

		cout << "Enter player MRR: ";
		cin >> playerMMRinput;
		teamMMR.push_back(playerMMRinput);


		if (largestMMRScore < playerMMRinput && antiBoosting) {

			largestMMRScore = playerMMRinput;
		}
	}
	if (numOfPlayersinSquad != 0) {

		//Calculate the average MMR of the squad.
		for (int i = 0; i < numOfPlayersinSquad; i++) {

			//Without boosting countermeasures
			averageMMR = averageMMR + teamMMR[i];

			//With boosting countermeasure
			if (teamMMR[i] > largestMMRScore - MMRLimit) {

				adjustedAverageMMR = adjustedAverageMMR + teamMMR[i];
			}
			else {

				adjustedAverageMMR = adjustedAverageMMR + (largestMMRScore - MMRLimit);
			}
		}
		averageMMR = averageMMR / numOfPlayersinSquad;
		adjustedAverageMMR = adjustedAverageMMR / numOfPlayersinSquad;
	}
	else {

		cout << "Error!";
	}

	//cout << "With boosting: " << averageMMR << "\nRank after anti-boosting: " << adjustedAverageMMR;
	return adjustedAverageMMR;
} // End of main

// This is the part that calculates a player's MMR score after a game 
//This is an old demo that is out of data, but it serves for demo purposes.  It will eventually be replaced with the actual function.
vector<int> endOfMatchMMRTallyDemo(int playerMMR, int MMRChangeLevel, int winStreak, int loseStreak, bool gameWasWon,
	int doLongTest, int gamesPlayed, bool rankLossPrevention, bool streakBonusEnabled) {

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

		//comment for testing
		if (doLongTest == 0) {

			cout << "Added: " << amountMMRChanged << " bonus " << streakBonus << " MMR\n";
		}

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
			MMRChangeLevel) || !rankLossPrevention) {

			playerMMR = playerMMR - amountMMRChanged - streakBonus;

			//comment for testing
			if (doLongTest == 0) {

				cout << "subtracted: " << -amountMMRChanged << " bonus " << streakBonus << " MMR\n";
			}
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

				//comment for testing
				if (doLongTest == 0) {
					cout << "subtracted: " << playerMMR - tempVariable << " MMR   Rank protected!\n";

					cout << "subtracted: " << -amountMMRChanged << " bonus " << -streakBonus << " MMR\n";
				}
			}
			else { //rank loss prevented
				while (playerMMR % 100 != 0) {
					playerMMR--;
				}
				//comment for testing
				if (doLongTest == 0) {

					cout << "subtracted: " << playerMMR - tempVariable << " MMR   Rank protected!\n";
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
	return { playerMMR, MMRChangeLevel, winStreak, loseStreak, gamesPlayed };
}


// This was used for early testing.  However, it still serves as a good demo for how calculations work. But none of the code is used for the full simulation
void simulationSimulationsSimpleDemo(int whichTest, bool rankLossPrevention, bool streakBonusEnabled) {

	//clear console
	system("cls");

	int champion = 0, diamond1 = 0, diamond2 = 0, diamond3 = 0, platinum1 = 0, platinum2 = 0, platinum3 = 0,
		gold1 = 0, gold2 = 0, gold3 = 0, silver1 = 0, silver2 = 0, silver3 = 0, silver4 = 0, silver5 = 0, bronze1 = 0,
		bronze2 = 0, bronze3 = 0, bronze4 = 0, bronze5 = 0, copper1 = 0, copper2 = 0, copper3 = 0, copper4 = 0, copper5
		= 0;

	int playerMMR, MMRChangeLevel = 0, winStreak = 0, loseStreak = 0, gamesPlayed = 0;
	int keepTestingThisPlayer = 0;
	//This is how many games are played
	int totalNumberOfGamesToPlay = 100;
	bool keepTesting = true;
	int timerDuration = 0;
	double matchesWon = 0, matchesLost = 0;
	bool gameWasWon{};
	string userInputDuringTest;

	vector<int> output = {};


	while (keepTesting) {
		//reset variables

		playerMMR = pullVariableFromSettings("initialmmr");

		if (whichTest != 0) {
			totalNumberOfGamesToPlay = rand() % (200 - 15 + 1) + 15;
		}
		if (whichTest == 1) {
			Sleep(1000);
		}


		//This needs work
		while (keepTestingThisPlayer < totalNumberOfGamesToPlay && keepTesting) {
			keepTestingThisPlayer++;

			//Ask the user if the game was won or loss
			if (whichTest == 0) {
				cout << "Instructions: Please type y/n, 1/0, or win/loss\nTyping anything else will exit the simulation.\n\nWas the match won?" << endl;
				cin >> userInputDuringTest;
				transform(userInputDuringTest.begin(), userInputDuringTest.end(), userInputDuringTest.begin(), ::tolower);

				if (userInputDuringTest == "y" || userInputDuringTest == "win" || userInputDuringTest == "1") {
					gameWasWon = true;
				}
				else if (userInputDuringTest == "n" || userInputDuringTest == "loss" || userInputDuringTest == "0") {
					gameWasWon = false;
				}
				else {
					cout << "Ending test..." << endl;
					keepTesting = false;
				}
			}
			if (whichTest != 0) {
				gameWasWon = rand() & 1;
			}
			if (gameWasWon == 1) {
				matchesWon++;
			}
			else {
				matchesLost++;
			}


			//input: playerMMR, MMRChangeLevel, winStreak, loseStreak, bool gameWasWon, doLongTest gamesPlayed
			output = endOfMatchMMRTallyDemo(playerMMR, MMRChangeLevel, winStreak, loseStreak, gameWasWon, whichTest,
				gamesPlayed, rankLossPrevention, streakBonusEnabled);
			//output: playerMMR, MMRChangeLevel, winStreak, loseStreak, gamesPlayed

			playerMMR = output[0];
			MMRChangeLevel = output[1];
			winStreak = output[2];
			loseStreak = output[3];
			gamesPlayed = output[4];
			//output for testing


			if (whichTest == 0) {
				system("cls");
				cout << rankChecker(playerMMR, gamesPlayed) << "\nMMR:" << playerMMR << "\nMMR change level: " <<
					MMRChangeLevel << "\nwinstreak: " << winStreak
					<< "\nLosingstreak: " << loseStreak << "\nGames Played: " << gamesPlayed << endl << endl;
			}
		}
		if (whichTest == 1) {
			cout << rankChecker(playerMMR, gamesPlayed) << " " << playerMMR << " MMR. W/L " << setprecision(2) <<
				matchesWon / matchesLost << " Matches: " << totalNumberOfGamesToPlay << endl;
		}

		//Counts ranks
		if (whichTest == 2) {

			if (rankChecker(playerMMR, MMRChangeLevel) == "Champion") { champion++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Diamond 1") { diamond1++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Diamond 2") { diamond2++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Diamond 3") { diamond3++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Platinum 1") { platinum1++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Platinum 2") { platinum2++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Platinum 3") { platinum3++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Gold 1") { gold1++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Gold 2") { gold2++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Gold 3") { gold3++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Silver 1") { silver1++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Silver 2") { silver2++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Silver 3") { silver3++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Silver 4") { silver4++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Silver 5") { silver5++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Bronze 1") { bronze1++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Bronze 2") { bronze2++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Bronze 3") { bronze3++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Bronze 4") { bronze4++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Bronze 5") { bronze5++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Copper 1") { copper1++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Copper 2") { copper2++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Copper 3") { copper3++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Copper 4") { copper4++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Copper 5") { copper5++; }


			timerDuration++;
			if (timerDuration == 100) {
				system("cls");
				cout << "Champion: " << champion << "\nDiamond 1: " << diamond1 << "\nDiamond 2: " << diamond2 <<
					"\nDiamond 3: " << diamond3 << "\nPlatinum 1: " << platinum1 << "\nPlatinum 2: " << platinum2 <<
					"\nPlatinum 3: " << platinum3 << "\nGold 1: " << gold1 << "\nGold 2: " << gold2 << "\nGold 3: " <<
					gold3 << "\nSilver 1: " << silver1 << "\nSilver 2: " << silver2 << "\nSilver 3: " << silver3 <<
					"\nSilver 4: " << silver4 << "\nSilver 5: " << silver5 << "\nBronze 1: " << bronze1 <<
					"\nBronze 2: " << bronze2 << "\nBronze 3: " << bronze3 << "\nBronze 4: " << bronze4 <<
					"\nBronze 5: " << bronze5 << "\nCopper 1: " << copper1 << "\nCopper 2: " << copper2 <<
					"\nCopper 3: " << copper3 << "\nCopper 4: " << copper4 << "\nCopper 5: " << copper5 << endl << endl;
				Sleep(2000);
				timerDuration = 0;
			}
		} //End of doLongTest == 2


		MMRChangeLevel = 0, winStreak = 0, loseStreak = 0, gamesPlayed = 0, matchesWon = 0, matchesLost = 0,
			keepTestingThisPlayer = 0;

		if (whichTest == 0 && totalNumberOfGamesToPlay == 100) {
			keepTesting = false;
			system("cls");
		}


		if (GetAsyncKeyState(VK_ESCAPE)) {
			keepTesting = false;

		}
	}
} //End of Test

//Create teams using data from createSquads()
int squadTeamSetUp() {

	// Initial setup
	vector<vector<vector<int>>> squadsInQue;

	bool continueMakingSquads = true;
	while (continueMakingSquads) {

		if (squadsInQue.empty()) {

			squadsInQue = createSquads();
		}

		// Set initial parameters
		vector<vector<int>> team1;
		vector<vector<int>> team2;

		vector<vector<int>> lobby; //For calculating average mmr of team1 + team2
		int initialRange = 25;
		int rangeOfMMR = initialRange;


		// Populate team1
		for (int i = 1; i < squadsInQue[0].size(); i++) {

			team1.push_back({ squadsInQue[0][i][0], squadsInQue[0][i][1] });
			lobby.push_back({ squadsInQue[0][i][0], squadsInQue[0][i][1] });
		}
		squadsInQue.erase(squadsInQue.begin()); // Remove first squad from queue


		// Loop to fill both teams
		int dot = 0;
		while (team1.size() < 5 || team2.size() < 5) {






			bool squadAdded = false; // Flag to track if any squad was added in this iteration

			int squadQueIndex = 0;
			while (squadQueIndex < squadsInQue.size() && !squadAdded) {

				//right here is where we make sure that the squads added are within a certain range
				// Squad that might be added has an average mmr less than the range allowed for the lobby
				if (abs(squadsInQue[squadQueIndex][0][1] - findAverageSquadMMR(lobby, false)) < rangeOfMMR) {


					// Check if the squad can fit into either team
					// If it can fit into team1
					if (team1.size() + squadsInQue[squadQueIndex].size() - 1 <= 5) {


						// Add squad to team1
						for (int i = 1; i < squadsInQue[squadQueIndex].size(); i++) {

							team1.push_back({ squadsInQue[squadQueIndex][i][0], squadsInQue[squadQueIndex][i][1] });
							lobby.push_back({ squadsInQue[squadQueIndex][i][0], squadsInQue[squadQueIndex][i][1] });
						}
						squadsInQue.erase(squadsInQue.begin() + squadQueIndex);
						squadAdded = true;
						rangeOfMMR = initialRange;
					}
					// If it can fit into team2
					else if (team2.size() + squadsInQue[squadQueIndex].size() - 1 <= 5 && !squadAdded) {

						// Add squad to team2
						for (int i = 1; i < squadsInQue[squadQueIndex].size(); i++) {

							team2.push_back({ squadsInQue[squadQueIndex][i][0], squadsInQue[squadQueIndex][i][1] });
							lobby.push_back({ squadsInQue[squadQueIndex][i][0], squadsInQue[squadQueIndex][i][1] });
						}
						squadsInQue.erase(squadsInQue.begin() + squadQueIndex);
						squadAdded = true;
						rangeOfMMR = initialRange;
					}
				}

				squadQueIndex++;
			}
		}


		int ave1 = 0;
		int ave2 = 0;
		int adjustedAve1 = 0;
		int adjustedAve2 = 0;

		cout << endl;
		for (auto& i : team1) {

			cout << "1 PlayerID: " << i[0] << "  PlayerMMR: " << i[1] << endl;
			ave1 = ave1 + i[1];
		}
		ave1 = ave1 / 5;

		cout << endl;

		for (auto& i : team2) {

			cout << "2 PlayerID: " << i[0] << "  PlayerMMR: " << i[1] << endl;
			ave2 = ave2 + i[1];
		}
		ave2 = ave2 / 5;
		cout << endl << "Actual average T1: " << ave1 << endl;
		cout << "Actual average T2: " << ave2 << endl << endl;
		cout << "Adjusted average T1: " << findAverageSquadMMR(team1, false) << endl;
		cout << "Adjusted average T2: " << findAverageSquadMMR(team2, false) << endl << endl;
		cout << "Adjusted difference: " << abs(findAverageSquadMMR(team1, false) - findAverageSquadMMR(team2, false)) << endl;
		cout << "Actual difference: " << abs(ave1 - ave2) << endl;
		cout << "range: " << rangeOfMMR << endl;

		cout << "\n\n";

		string makeANewSquad;
		cout << "Do you want to find another squad? y/n, 1/0, yes,no\n";
		cin >> makeANewSquad;
		if (makeANewSquad == "1", makeANewSquad == "yes", makeANewSquad == "y") {
			continueMakingSquads = true;
		}
		else {
			return 0;
		}
	}
}




//Create teams using data from createSquads()
vector<vector<int>> findPlayersForLobby(vector<vector<vector<int>>> squadsInQue) {

	// Set initial parameters
	vector<vector<int>> team1;
	vector<vector<int>> team2;
	vector<vector<int>> lobby; //For calculating average mmr of team1 + team2


	int initialRange = pullVariableFromSettings("initialrange");;
	int rangeOfMMR = initialRange;


	// Populate team1
	for (int i = 1; i < squadsInQue[0].size(); i++) {

		team1.push_back({ squadsInQue[0][i][0], squadsInQue[0][i][1], squadsInQue[0][i][2],squadsInQue[0][i][3], squadsInQue[0][i][4], squadsInQue[0][i][5] });
		lobby.push_back({ squadsInQue[0][i][0], squadsInQue[0][i][1], squadsInQue[0][i][2],squadsInQue[0][i][3], squadsInQue[0][i][4], squadsInQue[0][i][5] });
	}
	squadsInQue.erase(squadsInQue.begin()); // Remove first squad from queue


	// Loop to fill both teams
	int dot = 0;

	while (team1.size() < 5 || team2.size() < 5) {



		//cout << "inside T1: " << team1.size() << " T2: " << team2.size() << endl;

		rangeOfMMR = rangeOfMMR + 25;
		dot = dot + 1;
		if (dot % 10 == 0) {

			cout << '.';
		}


		bool squadAdded = false; // Flag to track if any squad was added in this iteration

		int squadQueIndex = 0;
		while (squadQueIndex < squadsInQue.size() && !squadAdded) {

			//right here is where we make sure that the squads added are within a certain range
			// Squad that might be added has an average mmr less than the range allowed for the lobby
			if (abs(squadsInQue[squadQueIndex][0][1] - findAverageSquadMMR(lobby, false)) < rangeOfMMR) {



				// Check if the squad can fit into either team
				// If it can fit into team1
				if (team1.size() + squadsInQue[squadQueIndex].size() - 1 <= 5) {


					// Add squad to team1
					for (int i = 1; i < squadsInQue[squadQueIndex].size(); i++) {

						team1.push_back({ squadsInQue[squadQueIndex][i][0],
							squadsInQue[squadQueIndex][i][1],
							squadsInQue[squadQueIndex][i][2],
							squadsInQue[squadQueIndex][i][3],
							squadsInQue[squadQueIndex][i][4],
							squadsInQue[squadQueIndex][i][5] });

						lobby.push_back({ squadsInQue[squadQueIndex][i][0],
							squadsInQue[squadQueIndex][i][1],
							squadsInQue[squadQueIndex][i][2],
							squadsInQue[squadQueIndex][i][3],
							squadsInQue[squadQueIndex][i][4],
							squadsInQue[squadQueIndex][i][5] });
					}
					squadsInQue.erase(squadsInQue.begin() + squadQueIndex);
					squadAdded = true;
					rangeOfMMR = initialRange;
				}
				// If it can fit into team2
				else if (team2.size() + squadsInQue[squadQueIndex].size() - 1 <= 5 && !squadAdded) {

					// Add squad to team2
					for (int i = 1; i < squadsInQue[squadQueIndex].size(); i++) {

						team2.push_back({ squadsInQue[squadQueIndex][i][0],
							squadsInQue[squadQueIndex][i][1],
							squadsInQue[squadQueIndex][i][2],
							squadsInQue[squadQueIndex][i][3],
							squadsInQue[squadQueIndex][i][4],
							squadsInQue[squadQueIndex][i][5] });

						lobby.push_back({ squadsInQue[squadQueIndex][i][0],
							squadsInQue[squadQueIndex][i][1],
							squadsInQue[squadQueIndex][i][2],
							squadsInQue[squadQueIndex][i][3],
							squadsInQue[squadQueIndex][i][4],
							squadsInQue[squadQueIndex][i][5] });
					}
					squadsInQue.erase(squadsInQue.begin() + squadQueIndex);
					squadAdded = true;
					rangeOfMMR = initialRange;
				}
			}

			squadQueIndex++;
		}
	}



	//FASTER MAYBE?:

	//// Preprocessing
	//vector<int> squadSizes(squadsInQue.size());
	//for (int i = 0; i < squadsInQue.size(); ++i) {
	//	squadSizes[i] = squadsInQue[i].size();
	//}

	//while (team1.size() < 5 || team2.size() < 5) {
	//	rangeOfMMR += 25;
	//	if (++dot % 10 == 0) {
	//		cout << '.';
	//	}

	//	bool squadAdded = false;
	//	for (int squadQueIndex = 0; squadQueIndex < squadsInQue.size() && !squadAdded; ++squadQueIndex) {
	//		if (abs(squadsInQue[squadQueIndex][0][1] - findAverageSquadMMR(lobby, false)) < rangeOfMMR) {
	//			vector<vector<int>>& team = (team1.size() + squadSizes[squadQueIndex] - 1 <= 5) ? team1 : team2;
	//			if (team.size() + squadSizes[squadQueIndex] - 1 <= 5) {
	//				// Add squad to team
	//				for (int i = 1; i < squadsInQue[squadQueIndex].size(); i++) {
	//					team.push_back({ squadsInQue[squadQueIndex][i][0],
	//						squadsInQue[squadQueIndex][i][1],
	//						squadsInQue[squadQueIndex][i][2],
	//						squadsInQue[squadQueIndex][i][3],
	//						squadsInQue[squadQueIndex][i][4],
	//						squadsInQue[squadQueIndex][i][5] });

	//					lobby.push_back({ squadsInQue[squadQueIndex][i][0], squadsInQue[squadQueIndex][i][1] });
	//				}
	//				squadsInQue.erase(squadsInQue.begin() + squadQueIndex);
	//				squadSizes.erase(squadSizes.begin() + squadQueIndex);
	//				rangeOfMMR = initialRange;
	//				squadAdded = true;
	//			}
	//		}
	//	}
	//}


		//take the two teams, and make 0-4 the first team and 5-9 the second team
	vector<vector<int>> outPutOfBothTeams;
	for (int i = 0; i < team1.size(); i++) {
		outPutOfBothTeams.push_back({ team1[i] });
	}
	for (int i = 0; i < team2.size(); i++) {
		outPutOfBothTeams.push_back({ team2[i] });
	}

	//Note: lobby = team1 + team2;




//print out lobby for testing purposes  //This is commented out
#pragma region
		/*
	int ave1 = 0;
	int ave2 = 0;
	int adjustedAve1 = 0;
	int adjustedAve2 = 0;

	cout << endl;
	for (auto& i : team1) {

		cout << "1 PlayerID: " << i[0] << "  PlayerMMR: " << i[1] << endl;
		ave1 = ave1 + i[1];
	}
	ave1 = ave1 / 5;

	cout << endl;

	for (auto& i : team2) {

		cout << "2 PlayerID: " << i[0] << "  PlayerMMR: " << i[1] << endl;
		ave2 = ave2 + i[1];
	}
	ave2 = ave2 / 5;
	cout << endl << "Actual average T1: " << ave1 << endl;
	cout << "Actual average T2: " << ave2 << endl << endl;
	cout << "Adjusted average T1: " << findAverageSquadMMR(team1, false) << endl;
	cout << "Adjusted average T2: " << findAverageSquadMMR(team2, false) << endl << endl;
	cout << "Adjusted difference: " << abs(findAverageSquadMMR(team1, false) - findAverageSquadMMR(team2, false)) << endl;
	cout << "Actual difference: " << abs(ave1 - ave2) << endl;
	cout << "range: " << rangeOfMMR << endl;

	cout << "\n\n";

	*/
#pragma endregion

	return outPutOfBothTeams;

}


//This is set up for solo queuing right now.  //WIP unbandoned for now
void soloQueuingTeamSetup() {

	vector<int> playersMMR = { 2000, 2100, 2200, 2300, 2400, 2600, 2700, 2800, 2900, 3000 };
	vector<int> redTeam = { 0, 0, 0, 0, 0 };
	vector<int> blueTeam = { 0, 0, 0, 0, 0 };
	int i = 0;
	int j = 0;
	int redTally = 0, blueTally = 0;

	//To evenly distribute players between teams (Built for solo queuing players at the moment.)
	while (playersMMR.size() > i) {

		//The lowest 4 players are split evenly and the top 4 players are split evenly.
		if (i <= 2) {

			redTeam[j] = playersMMR[i];
			blueTeam[j] = playersMMR[i + 1];
		}
		else if (i >= 6) {

			blueTeam[j] = playersMMR[i];
			redTeam[j] = playersMMR[i + 1];
		}

		redTally = redTally + redTeam[j];
		blueTally = blueTally + blueTeam[j];

		i += 2;
		j++;
	}
	//Depending on which team is higher, the higher skilled middle player will go to the team with less average MMR to attempt to make the game more even.
	if (redTally / 4 < blueTally / 4) {

		redTeam[2] = playersMMR[4];
		blueTeam[2] = playersMMR[4 + 1];
	}
	else {

		redTeam[2] = playersMMR[4];
		blueTeam[2] = playersMMR[4 + 1];
	}
	redTally = redTally + redTeam[2];
	blueTally = blueTally + blueTeam[2];


	//displays the results in the console
#pragma region

	i = 0;
	while (blueTeam.size() > i) {

		cout << redTeam[i] << " " << blueTeam[i] << endl;
		i++;
	}
	cout << redTally / 5 << " " << blueTally / 5;
#pragma endregion
}

// This cleans up the code for letting you chose which simulation demo you want to run.  
// (This is using old demos that will eventually be phased out with the live builds for the main simulation.
// (However, it is great for demos.  Especially simulation 0)
void handleSimulateDemoCommand(const string& command, bool rankLossPrevention, bool streakBonusEnabled) {
	string theRestOfCommand = command;

	if (theRestOfCommand.empty()) {
		cout << "Which simulation would you like to run?\n";
		listOfSimulationDemos();
		cout << "\nsimulatedemo ";
		cin >> theRestOfCommand;
	}

	// This will choose which simulation is ran.
	if (theRestOfCommand == "0") {
		// Shows how an individual player's rank changes over time
		simulationSimulationsSimpleDemo(0, rankLossPrevention, streakBonusEnabled);
	}
	else if (theRestOfCommand == "1") {
		// Early simulation showing different players with a 50% win rate each
		cout << "Player with 50% win rate over time:\n";
		simulationSimulationsSimpleDemo(1, rankLossPrevention, streakBonusEnabled);
		system("cls");
	}
	else if (theRestOfCommand == "2") {
		// Early simulation tallying up the players with a 50% win rate each. Players do not verse each other.  Each win and loss is based on a random boolean number.
		cout << "Tally players with 50% win rate over time:\n";
		simulationSimulationsSimpleDemo(2, rankLossPrevention, streakBonusEnabled);
		system("cls");
	}
	else if (theRestOfCommand == "help") {
		system("cls");
		listOfSimulationDemos();
		cout << endl;

	}
	else {
		system("cls");
		cout << "Error! Invalid input!";
		cout << endl;
	}
}

