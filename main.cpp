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
            else if(lineCount == nextSpace-3) {
                numOfPayouts = atoi(line.c_str()) + 1;
                lowStress.push_back(0);
                highStress.push_back(0);
                cost.push_back(0);
            }
            else if(lineCount == nextSpace-2) {
                stringstream s(line);
                while(!s.eof()) {
                    string tmp;
                    s >> tmp;
                    lowStress.push_back(atoi(tmp.c_str()));
                }
                readingLowStressComplete = 1;
            }
            else if(lineCount == nextSpace-1) {
                stringstream s(line);
                while(!s.eof()) {
                    string tmp;
                    s >> tmp;
                    highStress.push_back(atoi(tmp.c_str()));
                }
                readingHighStressComplete = 1; 
                cout << readingHighStressComplete << endl;              
            }
            if(readingHighStressComplete == 1 && readingLowStressComplete == 1) {

                for(i=1; i<numOfPayouts; i++) {
                    int highPrevCost;
                    if(i==1) highPrevCost = 0;
                    else highPrevCost = cost[i-2];

                    if(lowStress[i]+cost[i-1] > highStress[i]+highPrevCost) {
                        cost.push_back(lowStress[i] + cost[i-1]);
                        path.push_back('L');
                    }
                    else {
                        cost.push_back(highStress[i] + highPrevCost);
                        path.push_back('H');
                    }
            
                }
                cout << cost[numOfPayouts-1] << endl;
                for(i=0; i < numOfPayouts; i++)
                    if(path[i+1] == 'H') cout << "N ";
                    else cout << path[i] << " ";
                cout << endl;


                numofCompletedSets++;
                for(i = 1; i<numOfPayouts; i++){
                    cout << lowStress[i] << " ";
                }
                cout << endl;
                for(i = 1; i<numOfPayouts; i++){
                    cout << highStress[i] << " ";
                }
                if(numOfSets != numofCompletedSets) cout << "\n";
                cout << endl;                
                vector<int> newVector;
                vector<char> newVectorChar;
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