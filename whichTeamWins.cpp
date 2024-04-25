#include <vector>
//My libraries
#include "utilities.h"

using namespace std;


/**
 * @function whoWins
 *
 * @brief This function simulates a match between two teams and determines the winner based on the average skill level of each team.
 *
 * @param teams A 2D vector where each inner vector represents a team member and their skill level. The first element of the inner vector is the team member's ID, and the second element is their skill level.
 *
 * @return An integer representing the winning team. If the value is 0, it means team1 wins. If the value is 1, it means team2 wins.
 */

int whoWins(vector<vector<int>> teams) {
	vector<vector<int>> team1, team2;
	vector<int> teamThatWon;
	int winner;
	int team1Average = 0, team2Average = 0;
	//separte team1 and team2
	for (int i = 0; i < 5; i++) {
		team1.push_back(teams[i]);
		team1Average = team1Average + teams[i][1];
	}
	for (int i = 5; i < 10; i++) {
		team2.push_back(teams[i]);
		team2Average = team2Average + teams[i][1];
	}
	team1Average = team1Average / 5;
	team2Average = team2Average / 5;


	//chance of winning
	if (team1Average + 1500 < team2Average) {
		teamThatWon = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	}
	else if (team1Average + 1000 < team2Average) {
		teamThatWon = { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 };
	}
	else if (team1Average + 400 < team2Average) {
		teamThatWon = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 };
	}
	else if (team1Average < team2Average) {
		teamThatWon = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 };
	}
	else if (team2Average + 1500 < team1Average) {
		teamThatWon = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
	}
	else if (team2Average + 1000 < team1Average) {
		teamThatWon = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 };
	}
	else if (team2Average + 400 < team1Average) {
		teamThatWon = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 };
	}
	else if (team2Average < team1Average) {
		teamThatWon = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 };
	}
	else {
		teamThatWon = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 };
	}


	winner = teamThatWon[randomNumberGenerator(0, 9)];

	return winner;
}