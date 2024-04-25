#include <string>
#include <iostream>

using namespace std;

// This is a reference for what rank a player is
string rankChecker(int playerMMR, int matchesPlayed) {

	if (playerMMR >= 5000) return "Champion";
	if (playerMMR >= 4400) return "Diamond 1";
	if (playerMMR >= 4000) return "Platinum 1";
	if (playerMMR >= 3600) return "Platinum 2";
	if (playerMMR >= 3200) return "Platinum 3";
	if (playerMMR >= 3000) return "Gold 1";
	if (playerMMR >= 2800) return "Gold 2";
	if (playerMMR >= 2600) return "Gold 3";
	if (playerMMR >= 2500) return "Silver 1";
	if (playerMMR >= 2400) return "Silver 2";
	if (playerMMR >= 2300) return "Silver 3";
	if (playerMMR >= 2200) return "Silver 4";
	if (playerMMR >= 2100) return "Silver 5";
	if (playerMMR >= 2000) return "Bronze 1";
	if (playerMMR >= 1900) return "Bronze 2";
	if (playerMMR >= 1800) return "Bronze 3";
	if (playerMMR >= 1700) return "Bronze 4";
	if (playerMMR >= 1600) return "Bronze 5";
	if (playerMMR >= 1500) return "Copper 1";
	if (playerMMR >= 1400) return "Copper 2";
	if (playerMMR >= 1300) return "Copper 3";
	if (playerMMR >= 1200) return "Copper 4";
	return "Copper 5";
}