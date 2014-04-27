#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <time.h>
#include "AVLT.h"
using namespace std;

// Sets up the structure used to hold the team information in a vector
struct teamProfile {
    teamProfile(int p, string n, int nP, int nG, int nS, int gd, float per)
    :points(p), name(n), numGamesPlayed(nG), numGoals(nG), numSuffered(nS), goalDifference(gd), percentage(per) {}
    int points;
    string name;
    float numGamesPlayed;
    int numGoals;
    int numSuffered;
    int goalDifference;
    float percentage;
};

// an overload operation used for comparing in stable sort. 
bool operator<(const teamProfile &lhs, const teamProfile &rhs) {
    if(lhs.points != rhs.points) return lhs.points > rhs.points;
    else {
        if(lhs.goalDifference != rhs.goalDifference) return lhs.goalDifference > rhs.goalDifference;
        else {
            return lhs.numGoals > rhs.numGoals;
        }
    }
}

int main() {
    // Keeps the number of the sets
    int numOfSets;
    // Used to keep tracked of which line we're assessing
    int lineCount = 0;
    // A flag used so it knows if the line that it's assessing is a space
    int nextSpace = 2;
    // used for text file reading
    int flag = 0;
    // Flag used so the program does not go into
    // the write when the program first starts
    int flag3 = 0;
    // Number of teams for the set
    int numOfTeams=0;
    // Number of games for the set
    int numOfGames=0;
    // Count of how many teams read
    int countOfTeams=0;
    // Count of how many games read
    int countOfGames=0;
    // Common iterator
    int i;
    // Fixes spacing issue
    int spacingIssue = 0;
    // Used as a linker between the BST and vector
    static int globalIndex = 0;
    // List of all the teams
    vector<teamProfile> teamList;
    // The line being read
    string line;
    // File being read
    ifstream myfile("input.txt");
    // file being  written
    ofstream writeFile("moran.txt");
    // My tree
    static BST myBST;
    //Checks to make sure the file is open
    if(myfile.is_open()) {
        // Reads the next line until the file is empty
        while(getline(myfile, line)) {
            lineCount++;
            // If it is the first line, then we know it's the number of sets
            if(lineCount == 1) numOfSets = atoi(line.c_str());
            // if the line count is the next space, then we know to do no reading
            else if(lineCount == nextSpace) {
                flag = 1;
                continue;
            }
            // The flag is set high after the space is read, so this set of code deals
            // with reading the number of teams and games of that set
            else if(flag == 1) {
                // sets the flag low
                flag = 0;
                flag3 = 1;
                // results of the read
                vector<int> results;
                // Puts the line in a stringstream which is used to tokenize the string by spaces
                stringstream s(line);
                while(!s.eof()) {
                    string tmp;
                    s >> tmp;
                    //puts the values into a vector
                    results.push_back(atoi(tmp.c_str()));
                }
                // the first element is the number of teams
                numOfTeams = results[0];
                // and the second is the number of games
                numOfGames = results[1];

                // Because each team and game takes their own line, 
                // the addition of the two plus the current line count gives us the next space
                nextSpace = numOfTeams+numOfGames+lineCount+1;
            }

            // The block of code is when the current line being read are the teams
            else if(countOfTeams != numOfTeams) {
                // If the countOfTeams does not equal the number of teams,
                // then the line must be a team
                // creates a new team using the construcor. 
                teamProfile temp(0, line, 0, 0, 0, 0, 100);
                // Pushes the team into the list
                teamList.push_back(temp);
                // Inserts the node into the tree, which the data consists of
                // the team name and the index that it holds in the vector
                myBST.insert(line, globalIndex);
                // Increment the index and count of the teams
                globalIndex++;
                countOfTeams++;
            }

            // This block of code is used when the current line being read are the games
            else if(countOfGames != numOfGames) {
                int j = 0;
                string tmp;
                vector<int> score;
                vector<string> playingTeams;
                // Tokenizes the string by spaces and pushes them into the vector playing teams and scores
                stringstream s(line);
                while(!s.eof()){
                    s >> tmp;
                    // if the count of the spaces is 0 or 4 it must be a team
                    if(j == 0 || j == 4) playingTeams.push_back(tmp);
                    // if it's 1 or 3, then it must be a score
                    if(j == 1 || j == 3) score.push_back(atoi(tmp.c_str()));
                    j++;
                }
                // Searches for the node in the BST which has the same team name as the team playing
                // Because it's a hight balancing tree, the search time is in Theta(t) where t is the number of teams
                nodePointer temp = myBST.searchForIndex(playingTeams[0]);
                // this variable is used to grab the index at which the team is located in the vector
                int team1Index = temp->index;
                // Now increaes the values of that team in the vector
                teamList[team1Index].numGamesPlayed += 1;
                teamList[team1Index].numGoals += score[0];
                teamList[team1Index].numSuffered += score[1];
                teamList[team1Index].goalDifference = teamList[team1Index].numGoals - teamList[team1Index].numSuffered;

                nodePointer temp2 = myBST.searchForIndex(playingTeams[1]);
                int team2Index = temp2->index;
                teamList[team2Index].numGamesPlayed += 1;
                teamList[team2Index].numGoals += score[1];
                teamList[team2Index].numSuffered += score[0];
                teamList[team2Index].goalDifference = teamList[team2Index].numGoals - teamList[team2Index].numSuffered;

                // Increases points according to score
                if(score[0] > score[1]) teamList[team1Index].points += 3;
                else if(score[1] > score[0]) teamList[team2Index].points += 3;
                else if(score[1] == score[0]) {
                    teamList[team1Index].points++;
                    teamList[team2Index].points++;
                }

                // Percentage needs to be calculated after the current points have been added
                teamList[team1Index].percentage = (teamList[team1Index].points/(3*teamList[team1Index].numGamesPlayed)) * 100;                
                teamList[team2Index].percentage = (teamList[team2Index].points/(3*teamList[team2Index].numGamesPlayed)) * 100;

                // Increase count of games
                countOfGames++;
            }
            // If the countOfTeams equals the number of teams and the coutn of games equals the number of games
            // then we are ready to sort the vector of teams based on their points
            // Flag3 is used because the initial count of teams, number of teams, count of games, and number of games are 0
            // After these are set, then flag3 will be set high
            if(countOfTeams == numOfTeams && (countOfGames) == numOfGames && flag3 == 1 ){
                // Global Points, goalDifference, and scoredGoals are all used for ranking
                int globalPoints;
                int globalGoalDifference;
                int globalScoredGoals;
                // Sets inital rank to 1
                int rank = 1;
                int oldRank;
                // Solves the spacing issue that occurs when we reach the last team and set
                spacingIssue++;
                // Stable sort solves in theta(tlg(t)) where t is the number of teams
                stable_sort(teamList.begin(), teamList.end());
                // Writes the number of teams to the file
                writeFile << numOfTeams << endl;
                // Now prints out all the teams
                for(i=0; i<countOfTeams; i++) {
                    // Logic used to print the correct rank
                    if(i>0) {
                        if(teamList[i].points == globalPoints &&
                            teamList[i].goalDifference == globalGoalDifference &&
                            teamList[i].numGoals == globalScoredGoals) {
                            rank = oldRank;
                        }
                        else rank = i+1;
                    }
                    // Used for formatting because nuumGamesPlayed was a float
                    int tempCast = teamList[i].numGamesPlayed;
                    // Writes to file
                    writeFile << (rank) << ". " << teamList[i].name << " " << teamList[i].points << " " << tempCast << " ";
                    writeFile << teamList[i].numGoals << " " << teamList[i].numSuffered << " " << teamList[i].goalDifference << " ";
                    writeFile << setprecision(2) << fixed << teamList[i].percentage;

                    // If we have not read all the sets and all the teams, add a space
                    if(spacingIssue != numOfSets || i != countOfTeams-1) {
                        writeFile <<  endl;
                    }
                    // Used for make sure the correct rank is displayed for the team
                    globalPoints = teamList[i].points;
                    globalScoredGoals = teamList[i].numGoals;
                    globalGoalDifference = teamList[i].goalDifference;
                    oldRank = rank;
                }
                // If all the sets have not been read, then display a new line
                if(spacingIssue != numOfSets) writeFile <<  endl;
               
               // Resets everything
                countOfGames = 0;
                countOfTeams = 0;
                // clears the tree to be empty
                myBST.deleteTree();
                // Creates a new team vector which is empty to override the list of teams
                vector<teamProfile> newTP;
                teamList = newTP;
                // resets the flag and index
                flag3 = 0;
                globalIndex = 0;
            }
        }
    }
    // Closes file
    myfile.close();
    writeFile.close();
    return 0;
}