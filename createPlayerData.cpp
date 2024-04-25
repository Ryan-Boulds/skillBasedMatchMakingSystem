#include <vector>   
#include <cmath>    
#include <iostream> 
// my libraries
#include "settingsFunctions.h"
#include "oldDemos.h" //Figure this out
#include "findAveAndAntiBoosting.h"
#include "utilities.h"
using namespace std;



//Create players for test data until database is created.
vector<vector<int>> playersTestData(int numOfSquadsToCreate) {

	vector<vector<int>> players;

	//This generates the player's ID and MMR levels
	int i = 0;
	//In the case that all squads have 5 players, this makes sure that there will be enough players to fill each squad.
	while (i < numOfSquadsToCreate * 5) {
		//Make enough space for all of the players
		players.resize(i + 1);
		//Player Stats
		players[i].resize(6);
		players[i][0] = i; // PlayerID

		bool startingMMRIsRandom = pullVariableFromSettings("randomstartingmmr");
		if (startingMMRIsRandom) {
			players[i][1] = randomNumberGenerator(1500, 4500); //MMR
		}
		else {
			//This is the normal setting

			players[i][1] = pullVariableFromSettings("initialmmr"); //MMR
		}


		players[i][2] = 0; //MMRChangeLevel
		players[i][3] = 0; //winStreak
		players[i][4] = 0; //loseStreak
		players[i][5] = 0; //gamesPlayed

		i++;
	} //Format {PlayerID, MMR}

	return players;
}

//working on this right now:
vector<vector<vector<int>>> createSquads() {

	//test data
	//{PlayerID, MMR} (for testing. it should be from a database)
	int numOfSquads = pullVariableFromSettings("squads");;
	vector<vector<int>> squads;
	squads = playersTestData(numOfSquads);




	//Three dimensional array.  The first dimension is each squad queuing up.
	//The second dimension contains each player in the squad.
	//The third dimension is each player's playerID, MMR, MMRChangeLevel, winStreak, loseStreak, gamesPlayed
	vector<vector<vector<int>>> groupsInCue = {};

	//randomly put playersIDs in squads
	int i = 0;
	int j;
	int k = 0;
	int dot = 0;
	while (i < numOfSquads) {

		//This draws dots on screen to show that it is working.
		dot = dot + 1;
		if (dot % 1000 == 0) {
			cout << '.';
		}

		groupsInCue.resize(i + 1);
		j = 0;
		//randomly figure out how many people are squading up together

		int squadAverageMMR = 0;
		int numOfPlayersInSquad = trunc(rand() % 5);
		groupsInCue[i].push_back({ -1, 0 });
		while (j <= numOfPlayersInSquad) {

			//calculate the average before
			squadAverageMMR = squadAverageMMR + squads[k][1];

			groupsInCue[i].resize(j + 1);
			groupsInCue[i].push_back(squads[k]);
			j++;
			k++; // Goes  through each player
		}


		//Saves the average MMR of the squad for where the squad should be placed in matchmaking.  This can either have antiboosting on or off.
		groupsInCue[i][0][1] = findAverageSquadMMR(groupsInCue[i], true);

		i++;
	}
	//At this point, groupsInCue has a list of players cueing together for games. From here, it's a matter of matching them together





	cout << "\n";
	return groupsInCue;
}