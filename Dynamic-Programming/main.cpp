#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <time.h>
using namespace std;

int main() {
    // Keeps the number of the sets
    int numOfSets;
    // Used to keep tracked of which line we're assessing
    int lineCount = 0;
    // A flag used so it knows if the line that it's assessing is a space
    int nextSpace = 2;
    // used for text file reading
    int i;
    int numOfPayouts;
    int numofCompletedSets = 0;
    int readingLowStressComplete = 0;
    int readingHighStressComplete = 0;
    int readCount = 0;
    string line;
    ifstream myfile("input.txt");
    ofstream writeFile("group12.txt");
    vector<int> lowStress;
    vector<int> highStress;
    vector<int> cost;
    vector<char> path;
    //Checks to make sure the file is open
    if(myfile.is_open()) {
        // Reads the next line until the file is empty
        while(getline(myfile, line)) {
            lineCount++;
            // If it is the first line, then we know it's the number of sets
            if(lineCount == 1) numOfSets = atoi(line.c_str());
            // if the line count is the next space, then we know to do no reading
            else if(lineCount == nextSpace) {
                nextSpace+=4;
            }
            // Calculates number of payouts
            // Also seeds 0's in both the arrays
            else if(lineCount == nextSpace-3) {
                readCount = atoi(line.c_str());
                numOfPayouts = readCount + 1;
                lowStress.push_back(0);
                highStress.push_back(0);
                cost.push_back(0);
            }
            // Reads in low stress jobs
            else if(lineCount == nextSpace-2) {
                stringstream s(line);
                while(!s.eof()) {
                    string tmp;
                    s >> tmp;
                    lowStress.push_back(atoi(tmp.c_str()));
                }
                readingLowStressComplete = 1;
            }
            // Reads in high stress jobs
            else if(lineCount == nextSpace-1) {
                stringstream s(line);
                while(!s.eof()) {
                    string tmp;
                    s >> tmp;
                    highStress.push_back(atoi(tmp.c_str()));
                }
                readingHighStressComplete = 1;             
            }
            // Once reading boths jobs, we can calculate the best possible payout
            if(readingHighStressComplete == 1 && readingLowStressComplete == 1) {
                // Algorithm starts from the front, keeping the best possible cost
                // from the week backwards
                for(i=1; i<numOfPayouts; i++) {
                    int highPrevCost;
                    // Deals with vector out of bounds
                    if(i==1) highPrevCost = 0;
                    else highPrevCost = cost[i-2];

                    // If the low stress job + the best possible payouts from the the week before
                    // is greater than the high stress job payout + the best possible payout from two
                    // weeks before, we know we should take the low stress job
                    if(lowStress[i]+cost[i-1] > highStress[i]+highPrevCost) {
                        cost.push_back(lowStress[i] + cost[i-1]);
                        path.push_back('L');
                    }
                    // Else take the high stress job
                    else {
                        cost.push_back(highStress[i] + highPrevCost);
                        path.push_back('H');
                    }
            
                }
                cout << cost[numOfPayouts-1] << endl;
                
                // Algorithm to make sure it prints out the jobs correct
                // with the N, where it should be.
                char printArray[readCount];
                for(i=numOfPayouts-2; i>=0; i--) {
                    if(path[i] == 'L')
                        printArray[i] = 'L';
                    if(path[i] == 'H') {
                        printArray[i] = 'H';
                        printArray[i-1] = 'N';
                        i--;
                    }
                }
                // Prints array
                for(i=0; i<numOfPayouts-1; i++) {
                    cout << printArray[i] << " ";
                }

                numofCompletedSets++;

                if(numOfSets != numofCompletedSets) cout << "\n";  
                cout << endl;             
                // Clears everything for the next set to be read
                vector<int> newVector;
                path.clear();
                highStress = lowStress = cost = newVector;
                readingHighStressComplete = readingLowStressComplete = 0;

            }
        }
    }
    myfile.close();
    writeFile.close();
    return 0;
}