#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>
#include <string>

//My libraies:
#include "mmrDetails.h"
using namespace std;

// Define colors
map<string, string> rankColors = {
    {"Champion", "\033[38;2;196;0;98m"},         // Red
    {"Diamond 1", "\033[38;2;232;105;255m"},        // Diamond
    {"Platinum 1", "\033[38;2;136;247;236m"}, // Bright blue
    {"Platinum 2", "\033[38;2;136;247;236m"}, // Bright blue
    {"Platinum 3", "\033[38;2;136;247;236m"}, // Bright blue
    {"Gold 1", "\033[38;2;255;234;0m"}, // Gold
    {"Gold 2", "\033[38;2;255;234;0m"}, // Gold
    {"Gold 3", "\033[38;2;255;234;0m"}, // Gold
    {"Silver 1", "\033[38;2;227;227;227m"},         // Silver
    {"Silver 2", "\033[38;2;227;227;227m"},         // Silver
    {"Silver 3", "\033[38;2;227;227;227m"},         // Silver
    {"Silver 4", "\033[38;2;227;227;227m"},         // Silver
    {"Silver 5", "\033[38;2;227;227;227m"},         // Silver
    {"Bronze 1", "\033[38;2;245;170;66m"}, // Bronze
    {"Bronze 2", "\033[38;2;245;170;66m"}, // Bronze
    {"Bronze 3", "\033[38;2;245;170;66m"}, // Bronze
    {"Bronze 4", "\033[38;2;245;170;66m"}, // Bronze
    {"Bronze 5", "\033[38;2;245;170;66m"}, // Bronze
    {"Copper 1", "\033[38;2;92;34;13m"}, // Copper
    {"Copper 2", "\033[38;2;92;34;13m"}, // Copper
    {"Copper 3", "\033[38;2;92;34;13m"}, // Copper
    {"Copper 4", "\033[38;2;92;34;13m"}, // Copper
    {"Copper 5", "\033[38;2;92;34;13m"}  // Copper
};

void printBars(const map<int, int>& frequency, int minPoint, int maxPoint, double scaleFactor) {
    for (int i = minPoint; i <= maxPoint; i += 100) {
        string currentRank = rankChecker(i, 0);
        auto color = rankColors.find(currentRank);
        if (color != rankColors.end()) {
            cout << color->second;
        }
        cout << currentRank << ": " << i << "-" << i + 99 << ": ";
        if (frequency.find(i) != frequency.end()) {
            int value = frequency.at(i);
            int barLength = static_cast<int>(value * scaleFactor);
            if (barLength == 0) {
                cout << "\n"; // Print newline if bar length is zero
            }
            else {
                int j = 0;
                while (j < barLength) {
                    cout << "=";
                    ++j;
                }
            }
        }
        cout << "\033[0m\n"; // Reset color to default and move to the next line
    }
}

void printStats(const vector<vector<vector<int>>>& squadsInQue) {
    // Initialize frequency map and MMR range variables
    map<int, int> frequency;
    int minPoint = INT_MAX;
    int maxPoint = INT_MIN;

    // Calculate frequency of MMR ranges and update minPoint and maxPoint
    for (const auto& squad : squadsInQue) {
        for (const auto& player : squad) {
            if (player.size() > 1) {
                int point = player[1];
                minPoint = min(minPoint, point);
                maxPoint = max(maxPoint, point);
                int normalizedPoint = point / 100 * 100;
                frequency[normalizedPoint]++;
            }
        }
    }

    // Round minPoint down and maxPoint up to nearest 100
    minPoint = minPoint / 100 * 100;
    maxPoint = (maxPoint / 100 + 1) * 100;

    // Iterate over all possible MMR ranges and print frequency with color
    for (int i = minPoint; i <= maxPoint; i += 100) {
        // Set color based on MMR range
        if (i >= 5000) {
            cout << "\033[31m"; // Red
        }
        else if (i >= 4400) {
            cout << "\033[95m"; // Purple
        }
        else if (i >= 3200) {
            cout << "\033[38;2;136;247;236m"; // Bright blue
        }
        else if (i >= 2600) {
            cout << "\033[38;2;255;234;0m"; // Gold color
        }
        else if (i >= 2100) {
            cout << "\033[37m"; // Grey
        }
        else if (i >= 1600) {
            cout << "\033[38;2;245;170;66m"; // Bronze color
        }
        else {
            cout << "\033[38;2;92;34;13m"; // Copper color
        }

        cout << rankChecker(i, 0) << ": " << i << "-" << i + 99 << ": ";
        if (frequency.find(i) != frequency.end()) {
            cout << frequency.at(i); // Print frequency
        }
        else {
            cout << "0"; // Print 0 frequency if not found
        }
        cout << "  \033[0m" << endl; // Reset color to default and move to the next line
    }
}

void createBarChart(const vector<vector<vector<int>>>& squadsInQue) {
    map<int, int> frequency;
    int minPoint = INT_MAX;
    int maxPoint = INT_MIN;

    for (const auto& a : squadsInQue) {
        for (const auto& b : a) {
            if (b.size() > 1) {
                int point = b[1];
                minPoint = min(minPoint, point);
                maxPoint = max(maxPoint, point);
                int normalizedPoint = point / 100 * 100;
                frequency[normalizedPoint]++;
            }
        }
    }

    minPoint = minPoint / 100 * 100;
    maxPoint = (maxPoint / 100 + 1) * 100;

    int maxFrequency = 0;

    for (int i = minPoint; i <= maxPoint; i += 100) {
        if (frequency.find(i) != frequency.end()) {
            maxFrequency = max(maxFrequency, frequency.at(i));
        }
    }

    double scaleFactor = 1.0;
    if (maxFrequency > 100) {
        scaleFactor = 100.0 / maxFrequency;
    }

    printBars(frequency, minPoint, maxPoint, scaleFactor);
}