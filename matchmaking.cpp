#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <algorithm>
#include <cctype>

//My libraries
#include "settingsFunctions.h"
#include "findAveAndAntiBoosting.h"
#include "barChart.h"
#include "utilities.h"
#include "rankAdjustmentSystem.h"
#include "createPlayerData.h"
#include "whichTeamWins.h"
#include "musicThemes.h"

using namespace std;




/**
 * @brief Finds players for a lobby from a queue of squads.
 *
 * This function takes a 3D vector where each element represents a squad in the queue. Each squad is a 2D vector where the first element is a vector containing the squad's ID and average MMR, and the remaining elements are vectors representing the players in the squad. It also takes a boolean that determines the speed of the matchmaking process.
 *
 * If the matchmaking speed is false, the function will increase the range of acceptable MMR difference by a fixed value of 25 in each iteration (slowest). If the matchmaking speed is true, the function will adjust the range based on the difference between the average MMR of the lobby and the average MMR of the next squad (faster).
 *
 * The function returns a 2D vector representing the two teams of players.
 *
 * @param squadsInQue A 3D vector representing the queue of squads.
 * @param matchmakingSpeed A boolean representing the speed of the matchmaking process.
 * @return A 2D vector representing the two teams of players.
 */

 // This is the function that is doing the matchmaking by finding squads to put onto the team.
vector<vector<int>> findSquadsForLobby(vector<vector<vector<int>>> squadsInQue, bool matchmakingSpeed) {
    // Set initial parameters
    vector<vector<int>> team1;
    vector<vector<int>> team2;
    vector<vector<int>> lobby; //For calculating average mmr of team1 + team2

    int initialRange = pullVariableFromSettings("initialrange");
    int rangeOfMMR = initialRange;

    // Populate team1 with the first squad in the queue
    for (int i = 1; i < squadsInQue[0].size(); i++) {
        team1.push_back({ squadsInQue[0][i][0], squadsInQue[0][i][1], squadsInQue[0][i][2], squadsInQue[0][i][3], squadsInQue[0][i][4], squadsInQue[0][i][5] });
        lobby.push_back({ squadsInQue[0][i][0], squadsInQue[0][i][1], squadsInQue[0][i][2], squadsInQue[0][i][3], squadsInQue[0][i][4], squadsInQue[0][i][5] });
    }
    // Remove the first squad from the queue
    squadsInQue.erase(squadsInQue.begin());

    // Loop to fill both teams
    while (team1.size() < 5 || team2.size() < 5) {
        bool squadAdded = false; // Flag to track if any squad was added in this iteration
        int squadQueIndex = 0;
        while (squadQueIndex < squadsInQue.size() && !squadAdded) {
            // If matchmakingSpeed is true, calculate the difference between the average MMR of the lobby and the average MMR of the next squad
            if (matchmakingSpeed) {
                int diff = abs(squadsInQue[squadQueIndex][0][1] - findAverageSquadMMR(lobby, false));

                // If the difference is more than 25, set rangeOfMMR to this difference
                if (diff > 25) {
                    rangeOfMMR = diff + 1;
                }
                else {
                    // Reset rangeOfMMR to initialRange
                    rangeOfMMR = initialRange;
                }
            }

            // Check if the squad's average MMR is within the range
            if (!matchmakingSpeed || abs(squadsInQue[squadQueIndex][0][1] - findAverageSquadMMR(lobby, false)) < rangeOfMMR) {
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
                    // Remove the squad from the queue
                    squadsInQue.erase(squadsInQue.begin() + squadQueIndex);
                    squadAdded = true;
                    // Reset rangeOfMMR to initialRange
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
                    // Remove the squad from the queue
                    squadsInQue.erase(squadsInQue.begin() + squadQueIndex);
                    squadAdded = true;
                    // Reset rangeOfMMR to initialRange
                    rangeOfMMR = initialRange;
                }
            }

            squadQueIndex++;
        }

        // If no squad was added in this iteration, increase rangeOfMMR by a small fixed value if matchmakingSpeed is true, or by 25 if matchmakingSpeed is false
        if (!squadAdded) {
            rangeOfMMR += matchmakingSpeed ? 5 : 25;
        }
    }

    // Put both teams into a single vector to return
    vector<vector<int>> outPutOfBothTeams;
    outPutOfBothTeams.insert(outPutOfBothTeams.end(), team1.begin(), team1.end());
    outPutOfBothTeams.insert(outPutOfBothTeams.end(), team2.begin(), team2.end());
    return outPutOfBothTeams;
}


// saves space in the code for printing the details of each player by placing it into its own function
void printTeamDetails(const vector<vector<int>>& team, const string& teamName, const string& adjustmentStatus) {
    cout << "\n" << adjustmentStatus << " rank adjustments for " << teamName << "\n";
    for (const auto& i : team) {
        cout << "#" << teamName << " PlayerID: " << i[0] << "  PlayerMMR: " << i[1] << "  MMRChangeLevel: "
            << i[2] << "  winStreak: " << i[3] << "  loseStreak: " << i[4] << "  gamesPlayed: " << i[5] << "     " << endl;
    }
}


// updates the stats for the team
void updateTeamStats(vector<vector<int>>& team, bool winStatus) {
    for (auto& member : team) {
        member = endOfMatchMMRTally(member[0], member[1], member[2], member[3], member[4], winStatus, member[5], pullVariableFromSettings("streakbonus"));
    }
}


// This is for letting the user decide what is shown in the console during the simulation
bool getUserPreference(const string& prompt) {
    string userInput;
    cout << prompt << "\n(Slower if enabled) y/n\n";
    cin >> userInput;
    transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);
    if (userInput == "y" || userInput == "1" || userInput == "yes") {
        return true;
    }
    else if (userInput == "n" || userInput == "0" || userInput == "no") {
        return false;
    }
    else {
        cout << "Error!\n Settings the settings to true.";
        cout << "\nPress any key to continue . . .";
        _getch();  // Waits for a key press
        system("cls");
        return true;
    }
}

// This is the main function that puts everything together for matchmaking and talling MMR points
void matchmakeAndTally(bool slowMode) {

    //for createSquads:
    //The first dimension is each squad queuing up.
    //The second dimension contains each player in the squad.
    //The third dimension is each player's playerID, MMR, MMRChangeLevel, winStreak, loseStreak, gamesPlayed 
    vector<vector<vector<int>>> squadsInQue;
    vector<vector<int>> teams, team1, team2;
    vector<int> updatedSquads;
    int teamThatWins;
    squadsInQue = createSquads();
    //The rate at which the progress us updated
    int updateRate = 10;

    system("cls");
    bool showBarGraph = getUserPreference("Do you want to see BAR GRAPH during the simulation?");
    bool liveTeamStats = getUserPreference("Do you want to see LIVE TEAM STATS during the simulation?");
    // Asks how many games are going to be played
    int cycles = pullVariableFromSettings("games");


    // This loop finds a match, then repeats to find another game.
    for (int i = 0; i < cycles; i++) {


        // Shows the progress of the simulation       
        if (liveTeamStats || showBarGraph || slowMode) {
            system("cls");
            cout << "Progress: " << i << " / " << cycles << endl;
        }
        else {
            if (i % updateRate == 0) {
                system("cls");
                cout << "Progress: " << i << " / " << cycles << endl;
            }
        }


        // Gives the user the option of if they want to see the live stats on a bar graph
        if (showBarGraph) {
            createBarChart(squadsInQue);
        }

        // matchmakespeed is a variable that changes the algorithm to make matchmaking faster
        teams = findSquadsForLobby(squadsInQue, pullVariableFromSettings("matchmakespeed"));
        team1 = vector<vector<int>>(teams.begin(), teams.begin() + 5);
        team2 = vector<vector<int>>(teams.begin() + 5, teams.end());


        // Gives the user the option of if they want to see the live stats on a players before MMR changes
        if (liveTeamStats) {
            printTeamDetails(team1, "1", "Before");
            printTeamDetails(team2, "2", "Before");
        }

        //This will update the stats of the players after they win or lose a game.
        teamThatWins = whoWins(teams);
        updateTeamStats(team1, teamThatWins == 0);
        updateTeamStats(team2, teamThatWins == 1);

        // Gives the user the option of if they want to see the live stats on a players after MMR changes
        if (liveTeamStats) {
            printTeamDetails(team1, "1", "After");
            printTeamDetails(team2, "2", "After");
        }


        // Prints instructions for how to end the simulation with the ESC key, or how to pause it (in slowMode=false mode)
        if (slowMode) {
            cout << "\nPress ESC to end simulation. . .\nOr press any other key to continue . . .\n";
            char ch = _getch();  // Waits for a key press

            if (ch == 27) {  // ASCII value of ESC is 27
                i = cycles;
            }
        }
        else {
            if (liveTeamStats || showBarGraph || i % updateRate == 0) {
                cout << "\nPress ESC to end simulation\nPress ENTER to pause and show current stats. . .\n\n";
                if (GetAsyncKeyState(VK_ESCAPE)) {
                    i = cycles;
                }
                if (GetAsyncKeyState(VK_RETURN) && i > 5) {

                    //This shows the bar graph when the simulation is paused if the bar graph is disabled for faster processing
                    if (!showBarGraph) {
                        createBarChart(squadsInQue);
                    }
                    //This shows the stats of each bar which is the distribution of each player per 100 MMR
                    printStats(squadsInQue);

                    Sleep(100);
                    cout << "\nSimulation paused.\nPress ENTER to resume. . .\nPress ESC to end simulation. . .";
                    while (!GetAsyncKeyState(VK_RETURN) && i != cycles) {
                        // Wait until Enter is pressed again
                        Sleep(100); // Sleep for a short time to prevent high CPU usage
                        if (GetAsyncKeyState(VK_ESCAPE)) {
                            i = cycles;
                        }
                    }
                }
            }
        }

        //Next Step:
        // The players after their MMR has changed are put into updatedSquads, 
        // then they are placed at the end of the queuesquadsInQue with their old entries removed.

        for (const auto& team : { team1, team2 }) {
            for (const auto& member : team) {
                for (int a = 0; a < squadsInQue.size(); a++) {
                    for (int b = 0; b < squadsInQue[a].size(); b++) {
                        if (squadsInQue[a][b][0] == member[0]) {
                            squadsInQue[a][b] = member;
                            updatedSquads.push_back(a);
                        }
                    }
                }
            }
        }

        //Finds the new squad average for updating the squads
        for (int i = 0; i < squadsInQue.size(); i++) {
            squadsInQue[i][0][1] = findAverageSquadMMR(squadsInQue[i], true);
        }

        //This is done for efficiency
        sort(updatedSquads.begin(), updatedSquads.end());
        updatedSquads.erase(unique(updatedSquads.begin(), updatedSquads.end()), updatedSquads.end());

        // Adds squads back into the matchmaking queue
        for (int i = updatedSquads.size() - 1; i >= 0; --i) {
            squadsInQue.push_back(squadsInQue[updatedSquads[i]]);
            squadsInQue.erase(squadsInQue.begin() + updatedSquads[i]);
        }

        //Clears values for the time the loop repeats
        teams.clear();
        team1.clear();
        team2.clear();
        updatedSquads.clear();




    }//End of for loop that is for each team set up

    //Prints the final results of the simulation
    if (!liveTeamStats) {
        system("cls");
    }
    createBarChart(squadsInQue);
    printStats(squadsInQue);


    // Notification sound to let the user know that the simulation has completed
    starWars();
    string input = "n";
    while (input != "yes") {
        cout << "\nDo you want to go back to the menu?\nType \"yes\" to confirm: ";
        cin >> input;
    }
    system("cls");
}

